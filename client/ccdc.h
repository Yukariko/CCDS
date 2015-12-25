#ifndef CCDC_H
#define CCDC_H

#include <iostream>
#include "msg_queue.h"
#include "parser.h"

using namespace std;

class CCDC
{
public:
	CCDC(const string& ip, int port, const string& volume);
	void start();

protected:
	string get_status();
	void proc_status(const Parser& cmd);
	void proc_change(const Parser& cmd);

	void init_socket();
	bool send_message(const Parser& msg);

private:
	string ip;
	int port;
	string volume;

	MessageQueue msg_queue;

	int sock;
};

#endif
