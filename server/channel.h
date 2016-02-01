#ifndef CHANNEL_H
#define CHANNEL_H

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "parser.h"

using namespace std;

#define NOTHING 0
#define READY_UP 1
#define READY_DOWN 2
#define START 1
#define STOP 0

struct Status
{
	Status(int sock, const string& lv_name);
	int sock;
	int running;
	int state;
	unordered_map<string, int> status;
	uint64_t size = 0;
	string lv_name;
};

class Channel
{
public:
	Channel(int port);
	void init_socket();
	void start();
	void run_client(int client_sock, int idx);
	
	vector<Status>& get_client_status();

	static bool send_message(int client_sock, Parser& msg);

private:
	static vector<thread> client_threads;
	static vector<Status> client_status;
	static int sock;
	static int port;
};

#endif
