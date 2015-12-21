#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <iostream>
#include <queue>

using namespace std;

class MessageQueue
{
public:
	void push(const char *buf, int len);
	void pop();
	bool empty();

	const string& front();


private:
	queue<string> msg_queue;
	string last_msg;
};

#endif