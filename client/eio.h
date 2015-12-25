#ifndef EIO_H
#define EIO_H

#include <iostream>
#include <unordered_map>

using namespace std;

class EIO
{
public:
	EIO();
	void get_status(stringstream& status);
	void refresh();
private:
	unordered_map<string, string> config;
	unordered_map<string, int> eio_config;
	string eio_path;
};

#endif