#ifndef FLIPPLUGIN_H
#define FLIPPLUGIN_H

#include "../plugin.h"
#include <cstdlib>

using namespace std;

class flipPlugin:public plugin
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
			if(words.at(0) == "flip")
			{
				if(words.size() >= 2)
				{
					if(stringUtils::findWord(words, "or")>=0)
					{
//						words.erase(words.begin());
						vector<string> options = stringUtils::split(vector<string>(words.begin()+1, words.end()), "or");
						int option = rand()%options.size();
						reply += "I choose: " + options.at(option);
					}
					else
					{
						if(rand()%2 >= 1)
						{
							reply += "Yes!";
						}
						else
						{
							reply += "No!";
						}
					}
				}
				else
				{
					if(rand()%2 >= 1)
					{
						reply += "Heads!";
					}
					else
					{
						reply += "Tails!";
					}
				}
				ircNet.sendMsg(channel, reply);
				return 1;
			}
			else if(words.at(0) == "flop")
			{
				ircNet.sendMe(channel, "flops about on the floor.");
				return 1;
			}

			return 0;
		}

};

#endif
