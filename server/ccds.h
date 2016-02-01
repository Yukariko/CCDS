#ifndef CCDS_H
#define CCDS_H

#include <thread>
#include <atomic>

#include "lvm.h"
#include "channel.h"

#define LIMIT 1000

class CCDS
{
public:
	CCDS(int port = 7878);
	~CCDS();
	void start();

	double getLoadRate(Status& status);
	double getCacheHit(Status& status);

	bool cache_up(Status& status);
	bool cache_down(Status& status);
	bool cache_down();
	bool create(Status& status);

private:
	LVM lvm;
	int port;
	size_t pool_size = 1000;
	thread channelThread;
	Channel *channel;
};

#endif
