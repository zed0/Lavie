#include "../plugin.h"

using namespace std;

class ircExtraPlugin:public plugin
{
	private:
	public:
		//All lines starting with "!" get sent to this command, you can match and act on these.
		//ircNet is defined so you can use ircNet.sendMsg(channel, reply); to reply
		//If you do not match any commands you should return 0;
		//If you match a command but want other plugins to also be allowed to match commands you should return 1;
		//If you match a command and don't want other plugins to be allowed to match commands you should return 2;
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
				return 1;
			}
			return 0;
		}
		//All lines not starting with a ! get passed on to this command, you do not have to overwrite it
		//If you wish to use it then use the same return values as in the handleCommand() function
		int handleMessage(string nick, string channel, vector<string> words)
		{
			return 0;
		}

		//This function if run every time the program ticks.
		//You can use this to manipulate stored variables etc.
		//Should return 1 if it did anything, oterwise 0.
		int doTick()
		{
			return 0;
		}
};
