#ifndef EIO_H
#define EIO_H

#include <iostream>
#include <sstream>
#include <unordered_map>

using namespace std;

class _EIO
{
public:
	_EIO();
	void get_status(stringstream& status);
	void refresh();
private:
	unordered_map<string, string> config;
	unordered_map<string, int> eio_config;
	string eio_path;
};

#endif