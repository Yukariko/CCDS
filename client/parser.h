#ifndef PARSER_H
#define PARSER_H

#include <iostream>

using namespace std;

class Parser
{
public:
	Parser(const string& buf);
	Parser(const string& protocol, const string& value);

	const string& get_protocol() const;
	const string& get_value() const;
	const string& get_buf();

	void set_protocol(const string& protocol);
	void set_value(const string& value);

private:
	string buf;
	string protocol;
	string value;
	size_t offset;
	bool sync;
};

#endif
