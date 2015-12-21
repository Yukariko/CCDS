#ifndef CHANNEL_H
#define CHANNEL_H

#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

using namespace std;

class Channel
{
public:
	Channel(int port);
	void init_socket();
	void start();
	void run_client(int client_sock, int idx);
	
	const vector<pair<string,int>>& get_client_status();

	bool send_message(const string& msg);

private:
	static vector<thread> client_threads;
	static vector<pair<string,int>> client_status;
	static int sock;
	static int port;
};

#endif
