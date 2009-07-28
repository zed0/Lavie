#include "stringUtils.h"

string stringUtils::msgNick(string message)
{
	string result;
	result = message.substr(1,message.find("!")-1);
	return result;
}

string stringUtils::msgChannel(string message)
{
	string result;
	size_t pos = message.find("PRIVMSG ")+8;
	result = message.substr(pos,message.find(" ", pos)-pos);
	if(result.at(0) != '#')
	{
		result = msgNick(message);
	}
	return result;
}

vector<string> stringUtils::msgWords(string message)
{
	size_t pos = message.find(" :") + 2;
	return tokenize(message.substr(pos));
}

vector<string> stringUtils::tokenize(string message)
{
	vector<string> result;
	istringstream iss(message);
	string current;
	while(iss >> current)
	{
		result.push_back(current);
	}
	return result;
}
