#include <fstream>
#include <cstring>
#include <unistd.h>
#include "nbd.h"

#define NBD_CONFIG_PATH "./nbd.conf"

NBD::NBD()
{
	config["nbd_device"] = "nbd0";

	ifstream conf(NBD_CONFIG_PATH, ifstream::in);	

	if(conf.is_open())
	{
		char buf[256];
		while(conf.getline(buf, sizeof(buf)))
		{
			char key[256], value[256];
			if(sscanf(buf, "%s%s",key,value) != 2)
			{
				cout << "[Error] ccdc.conf read error" << endl;
				exit(1);
			}

			auto iter = config.find(key);
			if(iter != config.end())
				iter->second = value;
		}
	}
	else
		cout << "[Warning] ccdc.conf not found" << endl;
}

void NBD::set_port(int port)
{
	this->port = port;
}

void NBD::start()
{
	char buf[4096];
	sprintf(buf, "nbd-client %s %d /dev/%s -b 4096 &", ip.c_str(), nbd_port, config["nbd_device"].c_str());
	if(system(buf) != 0)
	{
		cout << "[Error] nbd refresh error" << endl;
	}
}