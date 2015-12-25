#ifndef CCDS_H
#define CCDS_H

#include <thread>
#include <atomic>

#include "lvm.h"
#include "channel.h"


class CCDS
{
public:
	CCDS(int port = 7878);
	~CCDS();
	void start();

	void cache_up(Status& status);
	void create(Status& status);

private:
	LVM lvm;
	int port;
	thread channelThread;
	Channel *channel;
};

#endif
