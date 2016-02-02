#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include "ccds.h"

CCDS::CCDS(int port)
{
	this->port = port;
	channel = new Channel(port);
}

CCDS::~CCDS()
{
	delete channel;
}

void CCDS::start()
{
	channelThread = thread(&Channel::start, channel);

	Parser check_status("status");

	while(1)
	{
		vector<Status>& status = channel->get_client_status();
		int client_num = status.size();
		while(N < client_num)
		{
			if(create(status[N]))
				N++;
			else
				break;
		}

		for(int i=0; i < N; i++)
		{
			if(status[i].state == READY_UP)
			{
				if(cache_up(status[i]))
					cout << "client " << status[i].lv_name << "'s cache size up" << endl;
			}
			else if(status[i].state == READY_DOWN)
			{
				if(cache_down(status[i]))
					cout << "client " << status[i].lv_name << "'s cache size down" << endl;
			}
		}

		for(int i=0; i < N; i++)
		{
			if(status[i].size == 0 || status[i].running != START || status[i].state != NOTHING)
				continue;
			Channel::send_message(status[i].sock, check_status);
		}

		sleep(30);

		for(int i=0; i < N; i++)
		{
			if(status[i].size == 0 || status[i].running != START || status[i].state != NOTHING)
				continue;
			if(getLoadRate(status[i]) >= 10 && getCacheHit(status[i]) <= 10)
			{
				if(cache_up(status[i]))
					cout << "client " << status[i].lv_name << "'s cache size up" << endl;
				break;
			}
		}
	}
}

double CCDS::getLoadRate(Status& status) 
{
	int nr_blocks = status.status["nr_blocks"];
	int used_blocks = status.status["nr_dirty"] + status.status["nr_sets"];
	return (double)(used_blocks * 100) / nr_blocks;
}
double CCDS::getCacheHit(Status& status) 
{
	int total = status.status["writes"] + status.status["reads"];
	int hits = status.status["write_hits"] + status.status["read_hits"];
	return (double)(hits * 100) / total;
}

bool CCDS::create(Status& status)
{
	if(pool_size < 50)
	{
		cache_down();
		return false;
	}

	if(lvm.lv_create(status.lv_name, 50) == false)
	{
		// lv already exist
		//return;
	}

	pool_size -= 50;
	
	status.size = 50;

	int status_port = 9000 + status.sock;

	stringstream ss;
	ss << status_port << " ";
	ss << status.size;

	Parser msg("create", ss.str());

	char buf[4096];
	sprintf(buf, "nbd-server -C /dev/null %d /dev/vg0/%s", status_port, status.lv_name.c_str());
	if(system(buf) != 0)
	{
		cout << "[Error] nbd-server create error" << endl;
		return false;
	}
	Channel::send_message(status.sock, msg);
	return true;
}

bool CCDS::cache_up(Status& status)
{
	if(pool_size < 50)
	{
		cache_down();
		return false;
	}

	if(status.running == STOP && status.state == READY_UP)
	{
		if(lvm.lv_size_up(status.lv_name, 50) == false)
			return false; 

		status.state = NOTHING;

		pool_size -= 50;
		status.size += 50;

		stringstream ss;
		ss << status.size;

		Parser msg("change", ss.str());
		Channel::send_message(status.sock, msg);
		return true;
	}

	status.state = READY_UP;
	Parser msg("stop", "");
	Channel::send_message(status.sock, msg);

	return true;
}

bool CCDS::cache_down()
{
	vector<Status>& status = channel->get_client_status();
	for(int i=0; i < N; i++)
		if(status[i].state == READY_DOWN)
			return false;

	for(int i=0; i < N; i++)
	{
		if(status[i].size > 50 && getLoadRate(status[i]) <= 10)
		{
			cache_down(status[i]);
			return true;
		}
	}

	return true;
}

bool CCDS::cache_down(Status& status)
{
	if(status.size <= 50)
		return false;

	if(status.running == STOP && status.state == READY_DOWN)
	{
		if(lvm.lv_size_down(status.lv_name, 50) == false)
			return false; 

		status.state = NOTHING;

		pool_size += 50;
		status.size -= 50;

		stringstream ss;
		ss << status.size;

		Parser msg("change", ss.str());
		Channel::send_message(status.sock, msg);
	}

	status.state = READY_DOWN;
	Parser msg("stop", "");
	Channel::send_message(status.sock, msg);
	return true;
}
