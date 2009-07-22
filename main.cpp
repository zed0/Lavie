#define BOT_NICK "Lavie"
#define BOT_DESC "An IRC bot made by zed0 and Zaer"
#define COMMAND_CHAR '!'

#include <cstdlib>
#include <ctime>
#include <sstream>
#include "irc.h"

using namespace std;

vector<string> msgWords(string message);
vector<string> tokenize(string message);
string msgNick(string message);
string msgChannel(string message);
int setTimedMsg(string message, vector<string> words, int seconds);
int handleCommand(string message, vector<string> words);
template<class T> T fromString(const string& s);
template<class T> string toString(const T& t);

struct timedMsg
{
	int id;
	time_t time;
	string message;
	vector<string> words;
};

irc ircNet;
vector<timedMsg> timedMessages;

int main(int argc, char *argv[])
{
	if(argc <3)
	{
		cerr << "usage: " + string(argv[0]) + " hostname port [options]" << endl;
		return 1;
	}
	ircNet.setNick("Lavie");
	ircNet.setDesc("An IRC bot made by zed0 and Zaer");
	ircNet.joinChannel("#stuff");
	for(int i=0; i<argc; ++i)
	{
		if(string(argv[i]) == "--nick" && argc>i+1)
		{
			ircNet.setNick(argv[i+1]);
		}
		else if(string(argv[i]) == "--channel" && argc>i+1)
		{
			ircNet.joinChannel(argv[i+1]);
		}
	}
	ircNet.connect(argv[1], argv[2]);
	ircNet.sendMsg("#stuff", "Ohayo everybody!");
	string message;
	while(true)
	{
		for(int i=0; i<timedMessages.size(); ++i)
		{
			if(time(NULL) > timedMessages.at(i).time)
			{
				handleCommand(timedMessages.at(i).message, timedMessages.at(i).words);
				//ircNet.sendMsg(timedMessages.at(i).channel, timedMessages.at(i).message);
				timedMessages.erase(timedMessages.begin()+i);
				--i;
			}
		}
		if(ircNet.checkMessages(message))
		{
			cout << message;
			if(message.find("PRIVMSG",0) != string::npos)
			{
				vector<string> words = msgWords(message);
				if(words.at(0).at(0) == COMMAND_CHAR) //if the line begins with our command char then handle commands
				{
					words.at(0).erase(0,1); //strip the command character off the front
					handleCommand(message, words);
				}
				else if(words.at(0) == ircNet.getNick() + ":")
				{
					if(words.at(1) == "you" || words.at(1) == "You")
					{
						string reply = msgNick(message) + ": No, you";
						for(int i=2; i<words.size(); ++i)
						{
							reply += " " + words.at(i);
						}
						ircNet.sendMsg(msgChannel(message), reply);
					}
				}
			}
		}
	}
	return 0;
}

//tokenize the message
vector<string> msgWords(string message)
{
	size_t pos = message.find(" :") + 2;
	return tokenize(message.substr(pos));
}

vector<string> tokenize(string message)
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

//gets the nick that a message was sent from
string msgNick(string message)
{
	string result;
	result = message.substr(1,message.find("!")-1);
	return result;
}

//gets the channel that a message was sent to
string msgChannel(string message)
{
	string result;
	size_t pos = message.find("PRIVMSG ")+8;
	result = message.substr(pos,message.find(" ", pos)-pos);
	if(result == ircNet.getNick())
	{
		result = msgNick(message);
	}
	return result;
}

int setTimedMsg(string message, vector<string> words, int seconds)
{
	static int id = 0;
	++id;
	timedMsg result;
	result.id = id;
	result.time = time(NULL) + seconds;
	result.message = message;
	result.words = words;
	timedMessages.push_back(result);
	return 0;
}

int handleCommand(string message, vector<string> words)
{
	if(words.at(0) == "reply")
	{
		string reply = msgNick(message) + ":";
		if(words.size() > 1)
		{
			for(int i=1; i<words.size(); ++i)
			{
				reply += " " + words.at(i);
			}
			reply += ", Giggle.";
		}
		else
		{
			reply += " Slrp. Slrp! SLRP!!!";
		}
		ircNet.sendMsg(msgChannel(message), reply);
	}
	else if(words.at(0) == "flip")
	{
		srand(time(NULL));
		string reply = msgNick(message);
		if(rand()%2 >= 1)
		{
			reply += ": Heads!";
		}
		else
		{
			reply += ": Tails!";
		}
		ircNet.sendMsg(msgChannel(message), reply);
	}
	else if(words.at(0) == "flop")
	{
		ircNet.sendMe(msgChannel(message), "flops about on the floor.");
	}
	else if(words.at(0) == "count")
	{
		stringstream ss;
		string reply;
		ss << msgNick(message) << ": You gave " << words.size() << " parameters.";
		reply = ss.str();
		ircNet.sendMsg(msgChannel(message), reply);
	}
	else if(words.at(0) == "become")
	{
		if(words.size() < 2)
		{
			ircNet.sendMsg(msgChannel(message), msgNick(message) + ": you need to give a name");
		}
		else
		{
			ircNet.setNick(words.at(1));
		}
	}
	else if(words.at(0) == "time")
	{
		if(words.size() < 2)
		{
			ircNet.sendMsg(msgChannel(message), msgNick(message) + ": Format: !time seconds");
		}
		else
		{
			int time = fromString<int>(words.at(1));
			if(time <= 0)
			{
				ircNet.sendMsg(msgChannel(message), msgNick(message) + ": Invalid time (" + words.at(1) + ")");
			}
			else
			{
				ircNet.sendMsg(msgChannel(message), "Setting timer for " + toString<int>(time) + " seconds.");
				setTimedMsg(message, tokenize("reply Times up"), time);
			}
		}
	}
	else if(words.at(0) == "in")
	{
		if(words.size() < 3)
		{
			ircNet.sendMsg(msgChannel(message), msgNick(message) + ": Format: !in seconds command");
		}
		else
		{
			int time = fromString<int>(words.at(1));
			if(time <= 0)
			{
				ircNet.sendMsg(msgChannel(message), msgNick(message) + ": Invalid time (" + words.at(1) + ")");
			}
			else
			{
				ircNet.sendMsg(msgChannel(message), msgNick(message) + ": Okay, will do in " + toString<int>(time) + " seconds.");
				vector<string> command = words;
				command.erase(command.begin(), command.begin()+2);
				setTimedMsg(message, command, time);
			}
		}
	}
	return 0;
}

template<class T> string toString(const T& t)
{
	ostringstream stream;
	stream << t;
	return stream.str();
}

template<class T> T fromString(const string& s)
{
	istringstream stream(s);
	T t;
	stream >> t;
	return t;
}
