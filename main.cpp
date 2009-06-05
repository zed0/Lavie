#include <cstdlib>
#include <ctime>
#include <sstream>
#include "irc.h"

using namespace std;

vector<string> msgWords(string message);
string msgNick(string message);
string msgChannel(string message);


irc ircNet;

int main(int argc, char *argv[])
{
	if(argc !=3)
	{
		cerr << "usage: " + string(argv[0]) + " hostname port" << endl;
		return 1;
	}
	ircNet.setNick("Lavie");
	ircNet.setDesc("An IRC bot made by zed0 and Zaer");
	ircNet.connect(argv[1], argv[2]);
	ircNet.joinChannel("#stuff");
	ircNet.joinChannel("#brownies");
	ircNet.sendMsg("#stuff", "Ohayo everybody!");
	ircNet.sendMsg("#brownies", "Ohayo everybody!");
	string message;
	while(true)
	{
		if(ircNet.checkMessages(message))
		{
			cout << message;
			if(message.find("PRIVMSG",0) != string::npos)
			{
				vector<string> words = msgWords(message);
				if(words.at(0) == "!reply")
				{
					string reply = msgNick(message) + ":";
					if(words.size() > 1)
					{
						for(int i=1; i<words.size(); ++i)
						{
							reply += " " + words.at(i);
						}
						reply += ". Giggle.";
					}
					else
					{
						reply += " Slrp. Slrp! SLRP!!!";
					}
					ircNet.sendMsg(msgChannel(message), reply);
				}
				if(words.at(0) == "!flip")
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
				if(words.at(0) == "!flop")
				{
					ircNet.sendMe(msgChannel(message), "flops about on the floor.");
				}
				if(words.at(0) == "!count")
				{
					stringstream ss;
					string reply;
					ss << msgNick(message) << ": You gave " << words.size() << " parameters.";
					reply = ss.str();
					ircNet.sendMsg(msgChannel(message), reply);
				}
				if(words.at(0) == "!become")
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
				if(words.at(0) == ircNet.getNick() + ":" && words.at(1) == "you")
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
	return 0;
}

//tokenize the message
vector<string> msgWords(string message)
{
	vector<string> result;
	size_t pos = message.find(" :") + 2;
	istringstream iss(message.substr(pos));
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



