#ifndef COUNTPLUGIN_H
#define COUNTPLUGIN_H

#include "../plugin.h"

using namespace std;

class countPlugin:public plugin
{
	private:
	public:
		int handleCommand(const string& nick, const string& channel, const vector<string>& words)
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

};

#endif
