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

		sleep(10);

		for(int i=0; i < N; i++)
		{
			if(status[i].size == 0)
				continue;
			if(status[i].size <= 50 && status[i].status["nr_blocks"] > 1)
			{
				cout << "client " << status[i].lv_name << "'s cache size up" << endl;
				cache_up(status[i]);
			}
		}


	}
}

void CCDS::create(Status& status)
{
	if(lvm.lv_create(status.lv_name, 50) == false)
	{
		// lv already exist
		//return;
	}
	
	status.size = 50;

	stringstream ss;
	ss << 9000 + status.sock;
	ss << status.size;

	Parser msg("create", ss.str());
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
