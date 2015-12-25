#include "parser.h"

Parser::Parser(const string& buf)
{
	this->buf = buf;
	offset = buf.find(" ");
	if(offset != string::npos)
	{
		this->protocol = buf.substr(0, offset);
		this->value.str(buf.substr(offset+1));
	}
	else
		this->protocol = buf;

	this->buf += "\n";
	sync = true;
}

Parser::Parser(const string& protocol, const string& value)
{
	this->protocol = protocol;
	this->value.str(value);
	sync = false;
}

const string& Parser::get_protocol() const
{
	return protocol;
}

stringstream& Parser::get_value() const
{
	return value;
}

const string& Parser::get_buf()
{
	if(sync)
		return buf;
	buf = protocol;
	if(value.length() > 0)
	{
		buf += " ";
		buf += value.str();
	}
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