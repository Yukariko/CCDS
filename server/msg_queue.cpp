#include "msg_queue.h"

void MessageQueue::push(const char *buf, int len)
{
	for(int i=0; i < len; i++)
	{
		if(buf[i] == '\n')
		{
			msg_queue.push(last_msg);
			last_msg = "";
		}
		else
			last_msg += buf[i];
	}
}

void MessageQueue::pop()
{
	msg_queue.pop();
}

bool MessageQueue::empty()
{
	return msg_queue.empty();
}

const string& MessageQueue::front()
{
	return msg_queue.front();
}