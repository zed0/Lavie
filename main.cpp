#define BOT_NICK "Lavie"
#define BOT_DESC "An IRC bot made by zed0 and Zaer"
#define COMMAND_CHAR '!'

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include "irc.h"
#include "stringUtils.h"

using namespace std;

int setTimedMsg(string message, vector<string> words, int seconds);
int handleCommand(string message, vector<string> words);
string loadQuestions(string fileName);
string getQuestion(int questionNum);

struct timedMsg
{
	int id;
	time_t time;
	string message;
	vector<string> words;
};

struct question
{
	int id;
	string question;
	string answer;
};

irc ircNet;
vector<timedMsg> timedMessages;
vector<question> questions;

int main(int argc, char *argv[])
{
	if(argc <3)
	{
		cerr << "usage: " + string(argv[0]) + " hostname port [options]" << endl;
		return 1;
	}
	ircNet.setNick(BOT_NICK);
	ircNet.setDesc(BOT_DESC);
	ircNet.joinChannel("#stuff");
	for(int i=0; i<argc; ++i)
	{
		if(string(argv[i]) == "--nick" && argc>i+1)
		{
			ircNet.setNick(argv[i+1]);
			++i;
		}
		if(string(argv[i]) == "--channel" && argc>i+1)
		{
			ircNet.joinChannel(argv[i+1]);
			++i;
		}
		if(string(argv[i]) == "--questionfile" && argc>i+1)
		{
			cout << loadQuestions(argv[i+1]) << endl;
			++i;
		}
	}
	ircNet.connect(argv[1], argv[2]);
	ircNet.sendMsg("#stuff", "Ohayo everybody!");
	string message;
	while(true)
	{
		for(int i=0; i<timedMessages.size(); ++i)
		{
			if(time(NULL) > timedMessages.at(i).time)
			{
				handleCommand(timedMessages.at(i).message, timedMessages.at(i).words);
				timedMessages.erase(timedMessages.begin()+i);
				--i;
			}
		}
		if(ircNet.checkMessages(message))
		{
			//cout << message;
			if(message.find("PRIVMSG",0) != string::npos)
			{
				vector<string> words = stringUtils::msgWords(message);
				if(words.at(0).at(0) == COMMAND_CHAR) //if the line begins with our command char then handle commands
				{
					words.at(0).erase(0,1); //strip the command character off the front
					handleCommand(message, words);
				}
				else if(words.at(0) == ircNet.getNick() + ":")
				{
					if(words.at(1) == "you" || words.at(1) == "You")
					{
						string reply = stringUtils::msgNick(message) + ": No, you";
						for(int i=2; i<words.size(); ++i)
						{
							reply += " " + words.at(i);
						}
						ircNet.sendMsg(stringUtils::msgChannel(message), reply);
					}
				}
			}
		}
	}
	return 0;
}

int setTimedMsg(string message, vector<string> words, int seconds)
{
	static int id = 0;
	++id;
	timedMsg result;
	result.id = id;
	result.time = time(NULL) + seconds;
	result.message = message;
	result.words = words;
	timedMessages.push_back(result);
	return 0;
}

int handleCommand(string message, vector<string> words)
{
	if(words.at(0) == "reply")
	{
		string reply = stringUtils::msgNick(message) + ":";
		if(words.size() > 1)
		{
			for(int i=1; i<words.size(); ++i)
			{
				reply += " " + words.at(i);
			}
			reply += ", Giggle.";
		}
		else
		{
			reply += " Slrp. Slrp! SLRP!!!";
		}
		ircNet.sendMsg(stringUtils::msgChannel(message), reply);
	}
	else if(words.at(0) == "flip")
	{
		srand(time(NULL));
		string reply = stringUtils::msgNick(message);
		if(rand()%2 >= 1)
		{
			reply += ": Heads!";
		}
		else
		{
			reply += ": Tails!";
		}
		ircNet.sendMsg(stringUtils::msgChannel(message), reply);
	}
	else if(words.at(0) == "flop")
	{
		ircNet.sendMe(stringUtils::msgChannel(message), "flops about on the floor.");
	}
	else if(words.at(0) == "count")
	{
		string reply = stringUtils::msgNick(message) + ": You gave " + stringUtils::toString(words.size()) + " parameters.";
		ircNet.sendMsg(stringUtils::msgChannel(message), reply);
	}
	else if(words.at(0) == "become")
	{
		if(words.size() < 2)
		{
			ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": you need to give a name");
		}
		else
		{
			ircNet.setNick(words.at(1));
		}
	}
	else if(words.at(0) == "time")
	{
		if(words.size() < 2)
		{
			ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": Format: !time seconds");
		}
		else
		{
			int time = stringUtils::parseTime(words.at(1));
			if(time <= 0)
			{
				ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": Invalid time (" + words.at(1) + ")");
			}
			else
			{
				ircNet.sendMsg(stringUtils::msgChannel(message), "Setting timer for " + stringUtils::toString(time) + " seconds.");
				setTimedMsg(message, stringUtils::tokenize("reply Times up"), time);
			}
		}
	}
	else if(words.at(0) == "in")
	{
		if(words.size() < 3)
		{
			ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": Format: !in time command");
		}
		else
		{
			int time = stringUtils::parseTime(words.at(1));
			if(time <= 0)
			{
				ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": Invalid time (" + words.at(1) + "), please use format: 1d2h3m4s");
			}
			else
			{
				ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": Okay, will do in " + stringUtils::toString(time) + " seconds.");
				vector<string> command = words;
				command.erase(command.begin(), command.begin()+2);
				setTimedMsg(message, command, time);
			}
		}
	}
	else if(words.at(0) == "question")
	{
		int number = 1;
		if(words.size() > 1)
		{
			number = stringUtils::fromString<int>(words.at(1));
		}
		ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": " + getQuestion(number));
	}
	else if(words.at(0) == "loadquestions")
	{
		ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": " + loadQuestions("quotes.txt"));
	}
	else if(words.at(0) == "randquestion")
	{
		int number = (rand()%(questions.size()-1));
		ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": " + stringUtils::toString(number) + ": " + getQuestion(number));
	}
	return 0;
}

string getQuestion(int questionNum)
{
	if(questionNum > questions.size() || questionNum <= 0)
	{
		return "Question does not exist.";
	}
	else
	{
		return questions.at(questionNum-1).question;
	}
}

string loadQuestions(string fileName)
{
	ifstream questionFile(fileName.c_str(), ifstream::in);
	if(questionFile.good())
	{
		int id = 0;
		string buffer;
		while(questionFile.good())
		{
			buffer = "";
			question result;
			result.id = ++id;
			result.question = "";
			while(getline(questionFile, buffer) && buffer != ".")
			{
				result.question += buffer;
			}
			if(questionFile.good())
			{
				questions.push_back(result);
				//cout << "Question " << id << " " << result.question << endl;
			}
		}
		questionFile.close();
		return "Finished loading " + stringUtils::toString(id-1) + " questions from " + fileName + " (total now " + stringUtils::toString(questions.size()) + ").";
	}
	else
	{
		return "There was a problem loading from " + fileName + ".";
	}
}
