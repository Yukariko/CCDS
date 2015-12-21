#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "msg_queue.h"
#include "channel.h"

int Channel::sock;
int Channel::port;
vector<thread> Channel::client_threads;
vector<pair<string,int>> Channel::client_status;

Channel::Channel(int port)
{
	this->port = port;
	init_socket();
}

void Channel::init_socket()
{
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror("socket");
		exit(1);
	}

	int option;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option, 4);

	struct sockaddr_in ref_adr;
	memset(&ref_adr, 0, sizeof(ref_adr));
	ref_adr.sin_family = PF_INET;
	ref_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	ref_adr.sin_port = htons(port);

	if(bind(sock, (struct sockaddr*)&ref_adr, sizeof(ref_adr)))
	{
		perror("bind");
		exit(1);
	}

	if(listen(sock, 0xFFF))
	{
		perror("listen");
		exit(1);
	}
}

void Channel::start()
{
	int ns;
	struct sockaddr_in cli;
	int clientlen;

	int idx = 0;
	while((ns = accept(sock, (struct sockaddr *)&cli,  (socklen_t *)&clientlen)) != -1)
	{
		client_status.push_back({"", 0});
		client_threads.push_back(thread(&Channel::run_client, this, ns, idx));
		idx++;
	}
}

const vector<pair<string,int>>&  Channel::get_client_status()
{
	return client_status;
}

void Channel::run_client(int client_sock, int idx)
{
	char buf[4096];

	MessageQueue msg_queue;

	while(1)
	{
		int len;
		if((len = recv(client_sock, buf, sizeof(buf), 0)) < 1)
			break;

		msg_queue.push(buf, len);

		while(!msg_queue.empty())
		{
			const string& msg = msg_queue.front();
			if(msg == "create")
			{
				client_status[idx].first = msg.c_str() + 7;
			}
			else if(msg == "status")
			{
				client_status[idx].first = atoi(msg.c_str() + 7);
			}

			msg_queue.pop();
		}
		while(10);
		send_message("status");
	}
}

bool Channel::send_message(const string& msg)
{
	bool res = send(sock, msg.c_str(), msg.length(), 0) > 0;
	if(!res)
	{
		string error = "send_message_fail " + msg;
		perror(error.c_str());
	}
	return res;
}
