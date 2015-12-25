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

	while(1)
	{
		sleep(5);

		vector<Status>& status = channel->get_client_status();
		int N = status.size();

		for(int i=0; i < N; i++)
		{
			if(status[i].status >= 1 && status[i].size < 50)
			{
				printf("client %s 's status upper %d\n", status[i].volume.c_str(), status[i].status);
				cache_up(status[i]);
			}
		}

		for(int i=0; i < N; i++)
		{
			channel->send_message(status[i].sock, check_status);
		}
	}
}

void CCDS::cache_up(Status& status)
{
	//to do
	if(lvm.lv_size_up(status.volume, 50) == false)
		return; 
	status.size += 50;
}
