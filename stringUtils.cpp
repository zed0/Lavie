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

int stringUtils::parseTime(string time)
{
	int result = 0;
	string current = time;
	int multiplier = 0;
	while(current.length())
	{
		char lastChar = current.at(current.length()-1);
		if(charIsNum(lastChar))
		{
			int i = 1;
			while(i <= current.length() && charIsNum(current.at(current.length()-i)))
			{
				++i;
			}
			string number = current.substr(current.length()+1-i,i);
			current.erase(current.length()+1-i,i);
			result += multiplier * stringUtils::fromString<int>(number);
		}
		else
		{
			if(lastChar == 's')
			{
				multiplier = 1;
			}
			else if(lastChar == 'm')
			{
				multiplier = 60;
			}
			else if(lastChar == 'h')
			{
				multiplier = 3600;
			}
			else if(lastChar == 'd')
			{
				multiplier = 86400;
			}
			else
			{
				return 0;
			}
			current.erase(current.end()-1);
		}
	}
	return result;
}

bool stringUtils::charIsNum(char character)
{
	if(character >= '0' && character <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

string stringUtils::joinWords(vector<string> words)
{
	string result = "";
	for(int i=0; i<words.size(); ++i)
	{
		result += words.at(i);
		if(i < words.size() - 1)
		{
			result += " ";
		}
	}
	return result;
}
