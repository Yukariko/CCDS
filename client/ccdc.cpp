#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ifstream>
#include <sstream>
#include "ccdc.h"

#define CONFIG_PATH "./ccdc.conf"

CCDC::CCDC(const string& ip, int port, const string& lv_name)
{
	this->ip = ip;
	this->port = port;
	this->lv_name = lv_name;


	init_config();
	init_eio_config();
	init_socket();

}

void CCDC::init_config()
{
	config["eio_cache_name"] = "cache1";
	config["nbd_device"] = "nbd0";

	ifstream conf(CONFIG_PATH, ifstream::in);	

	if(conf.is_open())
	{
		string str;
		while(conf.getline(str))
		{
			stringstream ss(str);

			string key, value;
			if(ss >> key >> value)
			{
				cout << "[Error] config file read error" << endl;
				exit(1);
			}

			auto iter = config.find(key);
			if(iter != config.end())
				iter->second = value;
		}
	}
	else
		cout << "[Warning] config file not found" << endl;

	eio_path = "/proc/enhanceio/" + config["eio_cache_name"] + "/stats";

	conf.close();
}

void CCDC::init_eio_config()
{
	eio_config["nr_blocks"] = 0;
	eio_config["nr_dirty"] = 0;
	eio_config["nr_sets"] = 0;
	eio_config["nr_blocks"] = 0;
	eio_config["reads"] = 0;
	eio_config["read_hits"] = 0;
	eio_config["writes"] = 0;
	eio_config["write_hits"] = 0;
}

void CCDC::init_socket()
{
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror("[Error] sock open");
		exit(1);
	}

	struct sockaddr_in clnt_adr;

	memset(&clnt_adr,0,sizeof(clnt_adr));
	clnt_adr.sin_family = AF_INET;
	clnt_adr.sin_addr.s_addr = inet_addr(ip.c_str());
	clnt_adr.sin_port = htons(port);

	if(connect(sock, (struct sockaddr *)&clnt_adr, sizeof(clnt_adr)) == -1)
	{
		perror("[Error] connect");
		exit(1);
	}
}

bool CCDC::send_message(Parser& msg)
{
	const string& buf = msg.get_buf();
	bool res = send(sock, buf.c_str(), buf.length(), 0) > 0;
	if(!res)
	{
		string error = "[Error] send_message_fail " + buf;
		perror(error.c_str());
	}
	return res;
}

void CCDC::start()
{
	Parser cmd("create", lv_name);
	send_message(cmd);

	char buf[4096];

	while(1)
	{
		int len;
		if((len = recv(sock, buf, sizeof(buf), 0)) < 1)
			break;

		msg_queue.push(buf, len);

		while(!msg_queue.empty())
		{
			Parser cmd(msg_queue.front());
			msg_queue.pop();
			if(cmd.get_protocol() == "status")
				proc_status(cmd);
			else if(cmd.get_protocol() == "change")
				proc_change(cmd);
			else if(cmd.get_protocol() == "create")
				proc_create(cmd);

		}
	}

	perror("connection closed");
	close(sock);
}

void CCDC::get_status(Parser& msg)
{
	ifstream eio_status(eio_path, ifstream::in);
	if(!eio_status.is_open())
	{
		cout << "[Error] eio config file open error" << endl;
		status = "-1";
		return;
	}

	string str;
	while(eio_status.getline(str))
	{
		stringstream ss(str);

		string key;
		int value;
		if(ss >> key >> value)
		{
			cout << "[Error] eio config file read error" << endl;
			status = "-1";
			return;
		}

		auto iter = eio_config.find(key);
		if(iter != eio_config.end())
			iter->second = value;
	}

	stringstream& status = msg.get_value();
	for(auto &pick : eio_config)
		status << pick.first << " " << pick.second << " ";
}

void CCDC::proc_create(Parser& cmd)
{
	// to do
	stringstream& ss = cmd.get_value();
	ss >> size;
	cout << "[Notice] Create Size to " << size << endl;
}

void CCDC::proc_status(Parser& cmd)
{
	Parser msg("status", "");
	get_status(msg);
	send_message(msg);
}

void CCDC::proc_change(Parser& cmd)
{
	// to do
	stringstream& ss = cmd.get_value();
	ss >> size;

	cout << "[Notice] Change Size to " << size << endl;
}
