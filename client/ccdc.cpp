#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>
#include "ccdc.h"

#define CCDC_CONFIG_PATH "./ccdc.conf"

CCDC::CCDC(const string& ip, int port)
{
	this->ip = ip;
	this->port = port;

	init_config();
	init_socket();
}

void CCDC::init_config()
{
	nbd.set_ip(ip);
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
	Parser cmd("create", "");
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
			else if(cmd.get_protocol() == "stop")
				proc_stop(cmd);
		}
	}

	perror("[Notice] connection closed");
	close(sock);
}

void CCDC::cache_refresh()
{
	cache_stop();
	cache_start();
}

void CCDC::cache_stop()
{
	eio.stop();
	sleep(5);
	nbd.stop();
	sleep(1);
}

void CCDC::cache_start()
{
	nbd.start();
	sleep(5);
	eio.start();
	sleep(5);
}

void CCDC::proc_create(Parser& cmd)
{
	// to do
	stringstream& ss = cmd.get_value();

	int nbd_port;
	ss >> nbd_port;
	ss >> size;

	nbd.set_port(nbd_port);

	cache_refresh();

	Parser msg("start", "");
	send_message(msg);

	cout << "[Notice] Create Size to " << size << endl;
}

void CCDC::proc_status(Parser& cmd)
{
	Parser msg("status", "");
	eio.get_status(msg.get_value());
	send_message(msg);
}

void CCDC::proc_change(Parser& cmd)
{
	// to do
	stringstream& ss = cmd.get_value();
	ss >> size;

	cache_start();

	Parser msg("start", "");
	send_message(msg);

	cout << "[Notice] Change Size to " << size << endl;
}

void CCDC::proc_stop(Parser& cmd)
{
	cache_stop();

	Parser msg("stop", "");
	send_message(msg);
}
