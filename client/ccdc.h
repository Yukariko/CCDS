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
	~CCDS();
	void start();

protected:
	string get_status();
	void proc_status(Parser& cmd);
	void proc_change(Parser& cmd);

	void init_socket();
	bool send_message(Parser& msg);

private:
	string ip;
	int port;
	string volume;

	MessageQueue msg_queue;

	int sock;
};

#endif
