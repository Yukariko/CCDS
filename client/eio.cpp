#include <fstream>
#include <cstring>
#include <unistd.h>
#include "eio.h"

#define EIO_CONFIG_PATH "./eio.conf"

_EIO::_EIO()
{
	eio_config["nr_blocks"] = 0;
	eio_config["nr_dirty"] = 0;
	eio_config["nr_sets"] = 0;
	eio_config["nr_blocks"] = 0;
	eio_config["reads"] = 0;
	eio_config["read_hits"] = 0;
	eio_config["writes"] = 0;
	eio_config["write_hits"] = 0;

	config["eio_cache_name"] = "cache1";
	config["create_eio_path"] = "./create_eio.sh";
	config["delete_eio_path"] = "./delete_eio.sh";

	ifstream conf(EIO_CONFIG_PATH, ifstream::in);	

	if(conf.is_open())
	{
		char buf[256];
		while(conf.getline(buf, sizeof(buf)))
		{
			char key[256], value[256];
			if(sscanf(buf, "%s%s",key,value) != 2)
			{
				cout << "[Error] eio.conf read error" << endl;
				exit(1);
			}

			auto iter = config.find(key);
			if(iter != config.end())
				iter->second = value;
		}
	}
	else
		cout << "[Warning] eio.conf not found" << endl;

	eio_path = "/proc/enhanceio/" + config["eio_cache_name"] + "/stats";

	conf.close();
}

void _EIO::get_status(stringstream& status)
{
	ifstream eio_status(eio_path, ifstream::in);
	if(!eio_status.is_open())
	{
		cout << "[Error] eio stats file open error" << endl;
		status << "-1";
		return;
	}

	char buf[256];
	while(eio_status.getline(buf, sizeof(buf)))
	{
		char key[256];
		int value;
		if(sscanf(buf, "%s %d",key, &value) != 2)
		{
			cout << "[Error] eio stats file read error" << endl;
			status << "-1";
			return;
		}

		auto iter = eio_config.find(key);
		if(iter != eio_config.end())
			iter->second = value;
	}

	eio_status.close();

	for(auto &pick : eio_config)
		status << pick.first << " " << pick.second << " ";
}

void _EIO::refresh()
{
	if(system(config["delete_eio_path"].c_str()) != 0)
	{
		cout << "[Error] delete eio error" << endl;
	}
	if(system(config["create_eio_path"].c_str()) != 0)
	{
		cout << "[Error] create eio error" << endl;
	}
}