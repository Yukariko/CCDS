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

void CCDS::start()
{
	channelThread = thread(&Channel::start, channel);

	while(1)
	{
		sleep(5);

		auto &status = channel->clientStatus;
		int N = status.size();
		for(int i=0; i < N; i++)
		{
			if(status[i].second >= 1)
			{
				printf("client %s 's status upper %d\n", status[i].first.c_str(), status[i].second);
				cache_up();
			}
		}
	}
}

void CCDS::cache_up()
{
	//to do
}