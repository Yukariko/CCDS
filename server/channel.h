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
	bool send_message(const string& msg);
private:
	static vector<thread> clientThreads;
	static vector<pair<string,int>> clientStatus;
	static int sock;
	static int port = 7878;
};

#endif