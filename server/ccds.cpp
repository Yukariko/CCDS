#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
		for(int i=0; i < N; i++)
		{
			if(status[i].size == 0)
				continue;
			Channel::send_message(status[i].sock, check_status);
		}

		sleep(10);

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
			if(status[i].status >= 1 && status[i].size <= 50)
			{
				printf("client %s 's status upper %d\n", status[i].lv_name.c_str(), status[i].status);
				cache_up(status[i]);
			}
		}


	}
}

void CCDS::create(Status& status)
{
	if(lvm.lv_create(status.lv_name, 50) == false)
		return;
	
	status.size = 50;

	stringstream ss;
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
