#ifndef NBD_H
#define NBD_H

#include <iostream>
#include <sstream>
#include <unordered_map>

using namespace std;

class NBD
{
public:
	NBD();
	void start();
	void set_port(int port);
private:
	int port;
	unordered_map<string, string> config;
};


#endif