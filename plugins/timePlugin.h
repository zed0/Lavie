#ifndef TIMEPLUGIN_H
#define TIMEPLUGIN_H

#include "../plugin.h"
#include <ctime>

struct timedMsg
{
	int id;
	time_t time;
	string nick;
	string channel;
	vector<string> words;
};

class timePlugin:public plugin
{
	private:
	public:
		static vector<timedMsg> timedMessages;
		int handleCommand(string nick, string channel, vector<string> words)
		{
			string reply = "";
			if(nick != "")
			{
				reply += nick + ": ";
			}
			if(words.at(0) == "time")
			{
				if(words.size() < 2)
				{
					reply += "Format: !time seconds";
					ircNet.sendMsg(channel, reply);
				}
				else
				{
					int time = stringUtils::parseTime(words.at(1));
					if(time <= 0)
					{
						reply += "Invalid time (" + words.at(1) + ")";
						ircNet.sendMsg(channel, reply);
					}
					else
					{
						if(time < 0)
						{
							reply += "Time must be positive.";
						}
						else
						{
							reply += "Setting timer for " + stringUtils::toString(time) + " seconds.";
							setTimedMsg(nick, channel, stringUtils::tokenize("reply Times up"), time);
						}
						ircNet.sendMsg(channel, reply);
					}
				}
				return 1;
			}
			return 0;
		}

		int doTick()
		{
			for(int i=0; i<timedMessages.size(); ++i)
			{
				if(std::time(NULL) > timedMessages.at(i).time)
				{
					handleAllCommands(timedMessages.at(i).nick, timedMessages.at(i).channel, timedMessages.at(i).words);
					timedMessages.erase(timedMessages.begin()+i);
					--i;
				}
			}
			return 1;
		}

		static int setTimedMsg(string nick, string channel, vector<string> words, int seconds)
		{
			static int id = 0;
			++id;
			timedMsg result;
			result.id = id;
			result.time = std::time(NULL) + seconds;
			result.nick = nick;
			result.channel = channel;
			result.words = words;
			timedMessages.push_back(result);
			return 0;
		}
};

#endif
