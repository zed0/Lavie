#ifndef IRCEXTRAPLUGIN_H
#define IRCEXTRAPLUGIN_H

#include "../plugin.h"

using namespace std;

class ircExtraPlugin:public plugin
{
	private:
	public:
		int handleCommand(string nick, string channel, vector<string> words)
		{
			string reply = "";
			if(nick != "")
			{
				reply += nick + ": ";
			}
			if(words.at(0) == "join" && words.size() > 1)
			{
				ircNet.joinChannel(words.at(1));
				reply += "Hai!  Joined " + words.at(1);
				ircNet.sendMsg(channel, reply);
				ircNet.sendMsg(words.at(1), "Ohayo everybody!");
				return 1;
			}
			if(words.at(0) == "part" && words.size() > 1)
			{
				ircNet.partChannel(words.at(1));
				reply += "Hai!  Parted " + words.at(1);
				ircNet.sendMsg(channel, reply);
				return 1;
			}
			return 0;
		}
		int handleMessage(string nick, string channel, vector<string> words)
		{
			return 0;
		}

		int doTick()
		{
			return 0;
		}
};

#endif
