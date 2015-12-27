#ifndef CCDC_H
#define CCDC_H

#include <iostream>
#include <unordered_map>
#include "msg_queue.h"
#include "parser.h"
#include "eio.h"
#include "nbd.h"

using namespace std;

class CCDC
{
public:
	CCDC(const string& ip, int port);
	void start();

protected:
	void get_status(Parser& msg);

	void proc_create(Parser& cmd);
	void proc_status(Parser& cmd);
	void proc_change(Parser& cmd);

	void refresh();

	void init_config();
	void init_socket();
	bool send_message(Parser& msg);

private:
	string ip;
	int port;

	uint64_t size = 0;

	NBD nbd;
	_EIO eio;

	unordered_map<string, string> config;
	MessageQueue msg_queue;

	int sock;
};

#endif
