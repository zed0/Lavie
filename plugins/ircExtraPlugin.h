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
		int startupOptions(vector<string> args)
		{
			for(int i=0; i<args.size(); ++i)
			{
				if(args.at(i) == "--channel" && args.size() > i+1)
				{
					ircNet.joinChannel(args.at(i+1));
					++i;
				}
				else if(args.at(i) == "--nick" && args.size() > i+1)
				{
					ircNet.setNick(args.at(i+1));
					++i;
				}
			}
			return 0;
		}
};

#endif
