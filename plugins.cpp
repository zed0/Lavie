#include "plugins.h"

int plugins::handleCommand(string nick, string channel, vector<string> words)
{
	for(size_t i=0; i<pluginList.size(); ++i)
	{
		if(pluginList.at(i)->handleCommand(nick, channel, words) >= 2)
		{
			return 1;
		}
	}
	return 0;
}

int plugins::handleMessage(string nick, string channel, vector<string> words)
{
	for(size_t i=0; i<pluginList.size(); ++i)
	{
		if(pluginList.at(i)->handleMessage(nick, channel, words) >= 2)
		{
			return 1;
		}
	}
	return 0;
}

int plugins::doTick()
{
	int count = 0;
	for(size_t i=0; i<pluginList.size(); ++i)
	{
		count += pluginList.at(i)->doTick();
	}
	return count;
}

int plugins::startupOptions(vector<string> args)
{
	int count = 0;
	for(size_t i=0; i<pluginList.size(); ++i)
	{
		count += pluginList.at(i)->startupOptions(args);
	}
	return count;
}
