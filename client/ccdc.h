#ifndef CCDC_H
#define CCDC_H

#include <iostream>
#include "msg_queue.h"

using namespace std;

class CCDC
{
public:
	CCDC(const string& ip, int port, const string& volume);
	void init_socket();
	void start();

	void get_status(string& msg);
	bool send_message(const string& msg);


private:
	string ip;
	int port;
	string volume;

	MessageQueue msg_queue;

	int sock;
};

#endif
