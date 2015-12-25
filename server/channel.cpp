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
vector<Status> Channel::client_status;


Status::Status(int sock, const string& lv_name) : sock(sock), lv_name(lv_name)
{
	status["nr_blocks"] = 0;
	status["nr_dirty"] = 0;
	status["nr_sets"] = 0;
	status["nr_blocks"] = 0;
	status["reads"] = 0;
	status["read_hits"] = 0;
	status["writes"] = 0;
	status["write_hits"] = 0;
}


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
		perror("[Error] socket");
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
		perror("[Error] bind");
		exit(1);
	}

	if(listen(sock, 0xFFF))
	{
		perror("[Error] listen");
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
		printf("[Notice] client %d connected!\n", idx);
		client_status.push_back(Status(ns, 0, ""));
		client_threads.push_back(thread(&Channel::run_client, this, ns, idx));
		idx++;
	}
}

vector<Status>& Channel::get_client_status()
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
			Parser msg(msg_queue.front());
			msg_queue.pop();

			cout << msg.get_protocol() << endl;

			if(msg.get_protocol() == "create")
			{
				msg.get_value() >> client_status[idx].lv_name;
				printf("[Notice] client %d name %s\n", idx, client_status[idx].lv_name.c_str());
			}
			else if(msg.get_protocol() == "status")
			{
				stringstream& ss = msg.get_value();
				string key;
				int value;
				while(ss >> key >> value)
				{
					auto iter = client_status[idx].find(key);
					if(iter != client_status[idx].end())
						iter->second = value;
					cout << key << " " << value << ", ";
				}
				cout << endl;
			}
		}
	}

	printf("[Notice] client %d connection close\n", idx);
	client_status[idx].size = 0;
	close(client_sock);
}

bool Channel::send_message(int client_sock, Parser& msg)
{
	const string& buf = msg.get_buf();
	bool res = send(client_sock, buf.c_str(), buf.length(), 0) > 0;
	if(!res)
	{
		string error = "[Error] send_message_fail " + buf;
		perror(error.c_str());
	}
	return res;
}