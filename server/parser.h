#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <sstream>

using namespace std;

class Parser
{
public:
	Parser(const string& buf);
	Parser(const string& protocol, const string& value);

	const string& get_protocol() const;
	stringstream& get_value();
	const string& get_buf();

	void set_protocol(const string& protocol);
	void set_value(const string& value);

private:
	string buf;
	string protocol;
	stringstream value;
	size_t offset;
	bool sync;
};

#endif
