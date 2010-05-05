#ifndef COUNTPLUGIN_H
#define COUNTPLUGIN_H

#include "../plugin.h"

using namespace std;

class countPlugin:public plugin
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
			if(words.at(0) == "count")
			{
				string reply = "You gave " + stringUtils::toString(words.size()) + " parameters.";
				ircNet.sendMsg(channel, reply);
				return 1;
			}
			return 0;
		}

		int doTick()
		{
			return 0;
		}
};

#endif
