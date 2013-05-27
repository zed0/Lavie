#ifndef INPLUGIN_H
#define INPLUGIN_H

#include "../plugin.h"
#include "timePlugin.h"

using namespace std;

class inPlugin:public plugin
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
			if(words.at(0) == "in")
			{
				if(words.size() < 3)
				{
					reply += "Format: !in time command";
					ircNet.sendMsg(channel, reply);
				}
				else
				{
					int time = stringUtils::parseTime(words.at(1));
					if(time <= 0)
					{
						reply += "Invalid time (" + words.at(1) + "), please use format: 1d2h3m4s";
						ircNet.sendMsg(channel, reply);
					}
					else
					{
						reply += "Okay, will do in " + stringUtils::toString(time) + " seconds.";
						ircNet.sendMsg(channel, reply);
						vector<string> command = words;
						command.erase(command.begin(), command.begin()+2);
						timePlugin::setTimedMsg(nick, channel, command, time);
					}
				}
				return 1;
			}
			return 0;
		}
};

#endif
