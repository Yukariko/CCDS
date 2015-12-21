#ifndef CCDC_H
#define CCDC_H

#include <iostream>
#include "msg_queue"

using namespace std;

class CCDC
{
public:
	CCDC(const string& ip, int port = 7878, const string& volume);
	void init_socket();
	void start();

	void get_status(string& msg);

private:
	string ip;
	int port;
	string volume;

	MessageQueue msg_queue;

	int sock;
};

#endif