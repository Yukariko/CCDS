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

	int N = 0;

	while(1)
	{
		vector<Status>& status = channel->get_client_status();
		int client_num = status.size();
		while(N < client_num)
		{
			create(status[N]);
			N++;
		}

		for(int i=0; i < N; i++)
		{
			if(status[i].size == 0)
				continue;
			Channel::send_message(status[i].sock, check_status);
		}

		sleep(30);

		for(int i=0; i < N; i++)
		{
			if(status[i].size == 0)
				continue;
			if(getLoadRate(status[i]) >= 95 && getCacheHit(status[i]) <= 10)
			{
				cout << "client " << status[i].lv_name << "'s cache size up" << endl;
				cache_up(status[i]);
			}
		}
	}
}

double CCDS::getLoadRate(const Status& status) const
{
	int nr_blocks = status.status["nr_blocks"];
	int used_blocks = status.status["nr_dirty"] + status.status["nr_sets"];
	return (double)(used_blocks * 100) / nr_blocks;
}
double CCDS::getCacheHit(const Status& status) const
{
	int total = status.status["writes"] + status.status["reads"];
	int hits = status.status["write_hits"] + status.status["read_hits"];
	return (double)(hits * 100) / total;
}

void CCDS::create(Status& status)
{
	if(lvm.lv_create(status.lv_name, 50) == false)
	{
		// lv already exist
		//return;
	}
	
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
		return;
	}
	Channel::send_message(status.sock, msg);
}

void CCDS::cache_up(Status& status)
{
	if(lvm.lv_size_up(status.lv_name, 50) == false)
		return; 
	status.size += 50;

	stringstream ss;
	ss << status.size;

	Parser msg("change", ss.str());
	Channel::send_message(status.sock, msg);

}
