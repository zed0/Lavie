#ifndef IRC_H
#define IRC_H
#include "network.h"
#include "stringUtils.h"
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::cerr;

class irc
{
	private:
		network *server; //our server
		bool connected;
		vector<string> channels; //list of the channels we are using
		string nick; //our nickname
		string description; //appears in /whois
	public:
		irc();
		//connect to the specified network
		int connect(string hostname, string port);
		//join the specified channel
		int joinChannel(string channel);
		//part the specified channel
		int partChannel(string channel);
		//check messages returns 0 if you don't need to do anything about them
		//puts the message in the buffer
		//NB: this will deal with PINGs etc.
		int checkMessages(string &buffer);
		//send a /msg to the specified recipient where "channel" is a channel or user
		int sendMsg(string channel, string message);
		//set our nickname
		int setNick(string newNick);
		//public method to get our nick
		string getNick();
		//set our description (can only be done before we connect)
		int setDesc(string newNick);
		//public method to get our description
		string getDesc();
		//send a /me to the specified channel
		int sendMe(string channel, string message);
};

#endif
