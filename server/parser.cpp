#include "parser.h"

Parser::Parser(const string& buf)
{
	this->buf = buf;
	offset = buf.find(" ");
	if(offset != string::npos)
	{
		this->protocol = buf.substr(offset);
		this->value = buf.substr(offset+1, buf.size());
	}
	else
		this->protocol = buf;

	this->buf += "\n";
	sync = true;
}

Parser::Parser(const string& protocol, const string& value)
{
	this->protocol = protocol;
	this->value = value;
	sync = false;
}

const string& Parser::get_protocol() const
{
	return protocol;
}

const string& Parser::get_value() const
{
	return value;
}

const string& Parser::get_buf()
{
	if(sync)
		return buf;
	buf = protocol;
	buf += " ";
	buf += value;
	buf += "\n";
	sync = true;
	return buf;
}

void Parser::set_protocol(const string& protocol)
{
	this->protocol = protocol;
	sync = false;
}

void Parser::set_value(const string& value)
{
	this->value = value;
	sync = false;
}