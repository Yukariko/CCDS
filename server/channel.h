#ifndef CHANNEL_H
#define CHANNEL_H

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include "parser.h"

using namespace std;

struct Status
{
	Status(int sock, int status, const string& volume) : sock(sock), status(status), volume(volume) {}
	int sock;
	int status;
	int size = 0;
	string volume;
};

class Channel
{
public:
	Channel(int port);
	void init_socket();
	void start();
	void run_client(int client_sock, int idx);
	
	vector<Status>& get_client_status();

	bool send_message(int client_sock, Parser& msg);

private:
	static vector<thread> client_threads;
	static vector<Status> client_status;
	static int sock;
	static int port;
};

#endif
