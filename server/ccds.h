#ifndef CCDS_H
#define CCDS_H

#include <thread>
#include <atomic>

#include "channel.h"


class CCDS
{
public:
	CCDS(int port = 7878);
	void start();

private:
	int port;
	thread channelThread;
	Channel *channel;
};

#endif