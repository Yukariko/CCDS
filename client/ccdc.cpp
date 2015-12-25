#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "ccdc.h"

CCDC::CCDC(const string& ip, int port, const string& volume)
{
	this->ip = ip;
	this->port = port;
	this->volume = volume;

	init_socket();

}

void CCDC::init_socket()
{
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror("sock open");
		exit(1);
	}

	struct sockaddr_in clnt_adr;

	memset(&clnt_adr,0,sizeof(clnt_adr));
	clnt_adr.sin_family = AF_INET;
	clnt_adr.sin_addr.s_addr = inet_addr(ip.c_str());
	clnt_adr.sin_port = htons(port);

	if(connect(sock, (struct sockaddr *)&clnt_adr, sizeof(clnt_adr)) == -1)
	{
		perror("connect");
		exit(1);
	}
}

bool CCDC::send_message(const Parser& msg)
{
	const string& buf = msg.get_buf();
	bool res = send(sock, buf.c_str(), buf.length(), 0) > 0;
	if(!res)
	{
		string error = "send_message_fail " + buf;
		perror(error.c_str());
	}
	return res;
}

void CCDC::start()
{
	send_message(Parser("create", volume));

	char buf[4096];

	while(1)
	{
		int len;
		if((len = recv(sock, buf, sizeof(buf), 0)) < 1)
			break;

		msg_queue.push(buf, len);

		while(!msg_queue.empty())
		{
			Parser cmd = Parser(msg_queue.front());
			msg_queue.pop();
			if(cmd.get_protocol() == "status")
				proc_status(cmd);
			else if(cmd.get_protocol() == "change")
				proc_change(cmd);
		}
	}

	perror("connection closed");
	close(sock);
}

string CCDC::get_status()
{
	return "1";
}

void CCDC::proc_status(const Parser& cmd)
{
	send_message(Parser("status", get_status()));
}

void CCDC::proc_change(const Parser& cmd)
{

}
