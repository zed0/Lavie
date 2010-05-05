#ifndef REPLYPLUGIN_H
#define REPLYPLUGIN_H

#include "../plugin.h"

class replyPlugin:public plugin
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
			if(words.at(0) == "reply")
			{
				if(words.size() > 1)
				{
					//note: editing the words vector like this is naughty, may be better to use the commented out code instead.
					words.erase(words.begin());
					reply += stringUtils::joinWords(words);
					/*for(int i=1; i<words.size(); ++i)
					{
						reply += words.at(i);
						if(i < words.size() - 1)
						{
							reply += " ";
						}
					}*/
					reply += ", Giggle.";
				}
				else
				{
					reply += "Slrp. Slrp! SLRP!!!";
				}
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
