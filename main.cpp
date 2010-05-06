#define BOT_NICK "Lavie"
#define BOT_DESC "An IRC bot made by zed0 and Zaer"
#define COMMAND_CHAR '-'

#include <cstdlib>
#include <sstream>
#include <fstream>
#include <ctime>
#include "irc.h"
#include "http.h"
#include "stringUtils.h"
#include "plugins.h"

using std::ifstream;
using std::ios_base;

int handleAllCommands(string nick, string channel, vector<string> words);
int handleAllMessages(string nick, string channel, vector<string> words);
//Should return 1 if it did anything, oterwise 0.

irc ircNet;
plugins pluginList;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	if(argc <3)
	{
		cerr << "usage: " + string(argv[0]) + " hostname port [options]" << endl;
		return 1;
	}
	ircNet.setNick(BOT_NICK);
	ircNet.setDesc(BOT_DESC);
	ircNet.joinChannel("#stuff");
	for(int i=0; i<argc; ++i)
	{
		if(string(argv[i]) == "--nick" && argc>i+1)
		{
			ircNet.setNick(argv[i+1]);
			++i;
		}
		if(string(argv[i]) == "--channel" && argc>i+1)
		{
			ircNet.joinChannel(argv[i+1]);
			++i;
		}
		if(string(argv[i]) == "--questionfile" && argc>i+1)
		{
			//cout << quizPlugin::loadQuestions(argv[i+1]) << endl;
			++i;
		}
	}
	ircNet.connect(argv[1], argv[2]);
	ircNet.sendMsg("#stuff", "Ohayo everybody!");
	string message;
	while(true)
	{
		pluginList.doTick();
		if(ircNet.checkMessages(message))
		{
			if(message.find("PRIVMSG",0) != string::npos)
			{
				vector<string> words = stringUtils::msgWords(message);
				if(words.at(0).at(0) == COMMAND_CHAR) //if the line begins with our command char then handle commands
				{
					words.at(0).erase(0,1); //strip the command character off the front
					handleAllCommands(stringUtils::msgNick(message), stringUtils::msgChannel(message), words);
				}
				else if(words.at(0) == ircNet.getNick() + ":")
				{
					if(words.at(1) == "you" || words.at(1) == "You")
					{
						string reply = stringUtils::msgNick(message) + ": No, you";
						for(int i=2; i<words.size(); ++i)
						{
							reply += " " + words.at(i);
						}
						ircNet.sendMsg(stringUtils::msgChannel(message), reply);
					}
					if(words.at(1) == "you're" || words.at(1) == "You're")
					{
						string reply = stringUtils::msgNick(message) + ": No, you're";
						for(int i=2; i<words.size(); ++i)
						{
							reply += " " + words.at(i);
						}
						ircNet.sendMsg(stringUtils::msgChannel(message), reply);
					}
				}
				handleAllMessages(stringUtils::msgNick(message), stringUtils::msgChannel(message), words);
			}
		}
	}
	return 0;
}

int handleAllCommands(string nick, string channel, vector<string> words)
{
	pluginList.handleCommand(nick, channel, words);
	return 0;
	if(words.at(0) == "httpget")
	{
		string hostname = "zed0.uwcs.co.uk";
		string path = "/";
		string port = "80";
		if(words.size() > 1)
		{
			hostname = stringUtils::urlHostname(words.at(1));
			path = stringUtils::urlPath(words.at(1));
			port = stringUtils::urlPort(words.at(1));
		}
		cout << "Getting " << hostname << " " << path << " " << port << endl;
		http httpNet(hostname, port);
		string result = httpNet.get(path);
		//ircNet.sendMsg(channel, reply + result);
		cout << result << endl;
	}
	return 0;
}

int handleAllMessages(string nick, string channel, vector<string> words)
{
	pluginList.handleMessage(nick, channel, words);
	return 0;
}
