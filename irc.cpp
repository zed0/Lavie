#include "irc.h"
#include <algorithm>

irc::irc()
{
	connected = false;
	nick = "default";
	description = "default";
}

int irc::connect(string hostname, string port)
{
	server = new network(hostname, port);
	string message;
	while(message.find("NOTICE Auth :*** Looking up your hostname...", 0) == string::npos)
	{
		server->recieveMsg(message);
	}
	server->sendMsg("NICK " + nick);
	bool haveHostname = false;
	while(!haveHostname)
	{
		server->recieveMsg(message);
		if(message.find("NOTICE Auth :*** Found your hostname", 0) == string::npos)
		{
			haveHostname = true;
		}
		else if(message.find("NOTICE Auth :*** Could not resolve your hostname", 0) == string::npos)
		{
			haveHostname = true;
		}
	}
	server->sendMsg("USER " + nick + " 0 * : " + description);
	while(message.find("NOTICE Auth :Welcome", 0) == string::npos)
	{
		server->recieveMsg(message);
	}
	cout << "connected to " << hostname << ":" << port << endl;
	connected = true;
	for(int i=0; i<channels.size(); ++i)
	{
		server->sendMsg("JOIN " + channels.at(i));
		string message;
		while(message.find("JOIN :" + channels.at(i), 0) != string::npos)
		{
			server->recieveMsg(message);
		}
	}
	return 0;
}

int irc::joinChannel(string channel)
{
	channels.push_back(channel);
	if(connected)
	{
		server->sendMsg("JOIN " + channel);
		string message;
		while(message.find("JOIN :" + channel, 0) != string::npos)
		{
			server->recieveMsg(message);
		}
	}
	cout << "joined " << channel << endl;
	return 0;
}

int irc::checkMessages(string &buffer)
{
	if(!connected)
	{
		cerr << "Cannot recieve messages until connected to the server" << endl;
		return 0;
	}
	else
	{
		string message;
		if(server->recieveMsg(message) > 0)
		{
			//Deal with PINGs
			if(message.substr(0,4) == "PING")
			{
				server->sendMsg(message.replace(0,4,"PONG"));
				return 0;
			}
			//Oherwise pass the message on
			buffer = message;
			return message.size();
		}
	}
	return 0;
}

int irc::sendMsg(string channel, string message)
{
	//check we are connected to the server
	if(!connected)
	{
		cerr << "Cannot send messages before connecting to the server" << endl;
		return 1;
	}
	//check if we have actually joined the channel we are trying to send a message to
	else if(find(channels.begin(), channels.end(), channel) == channels.end() && channel.substr(1,1) == "#")
	{
		cerr << "Trying to message channel we are not in" << endl;
		return 1;
	}
	else
	{
		server->sendMsg("PRIVMSG " + channel + " :" + message);
	}
	return 0;
}

int irc::sendMe(string channel, string message)
{
	//check we are connected to the server
	if(!connected)
	{
		cerr << "Cannot send messages before connecting to the server" << endl;
		return 1;
	}
	//check if we have actually joined the channel we are trying to send a message to
	else if(find(channels.begin(), channels.end(), channel) == channels.end())
	{
		cerr << "Trying to message channel we are not in" << endl;
		return 1;
	}
	else
	{
		server->sendMsg("PRIVMSG " + channel + " :\001ACTION " + message + "\001");
	}
	return 0;
}

int irc::setNick(string newNick)
{
	nick = newNick;
	if(connected)
	{
		server->sendMsg("NICK " + newNick);
	}
	return 0;
}

string irc::getNick()
{
	return nick;
}


int irc::setDesc(string newDesc)
{
	if(connected)
	{
		cerr << "Cannot change description after connecting" << endl;
		return 1;
	}
	else
	{
		description = newDesc;
	}
	return 0;
}

string irc::getDesc()
{
	return description;
}
