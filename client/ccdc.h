#ifndef CCDC_H
#define CCDC_H

#include <iostream>
#include <unordered_map>
#include "msg_queue.h"
#include "parser.h"
#include "eio.h"

using namespace std;

class CCDC
{
public:
	CCDC(const string& ip, int port, const string& lv_name);
	void start();

protected:
	void get_status(Parser& msg);

	void proc_create(Parser& cmd);
	void proc_status(Parser& cmd);
	void proc_change(Parser& cmd);

	void nbd_refresh(int nbd_port);

	void init_config();
	void init_socket();
	bool send_message(Parser& msg);

private:
	string ip;
	int port;
	string lv_name;

	uint64_t size = 0;

	int nbd_port;

	_EIO eio;

	unordered_map<string, string> config;
	MessageQueue msg_queue;

	int sock;
};

#endif
