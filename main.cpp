#define BOT_NICK "Lavie"
#define BOT_DESC "An IRC bot made by zed0 and Zaer"
#define COMMAND_CHAR '!'
#define QUIZ_TIME 10 //time between question, answer and next question in the quiz

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include "irc.h"
#include "http.h"
#include "stringUtils.h"

using namespace std;

int setTimedMsg(string nick, string channel, vector<string> words, int seconds);
int handleCommand(string nick, string channel, vector<string> words);
string loadQuestions(string fileName);
string getQuestion(int questionNum);
string getAnswer(int questionNum);

struct timedMsg
{
	int id;
	time_t time;
	string nick;
	string channel;
	vector<string> words;
};

struct question
{
	int id;
	string question;
	//string answer;
	vector<string> answer;
	string category;
	string file;
};

irc ircNet;
vector<timedMsg> timedMessages;
vector<question> questions;
int currentQuestion;
int quizTiming = QUIZ_TIME;
bool continuousQuestions = false;

int main(int argc, char *argv[])
{
	srand(time(NULL));
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
				handleCommand(timedMessages.at(i).nick, timedMessages.at(i).channel, timedMessages.at(i).words);
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
					handleCommand(stringUtils::msgNick(message), stringUtils::msgChannel(message), words);
				}
				else if(currentQuestion != 0) //Handle valid answers to the current question
				{
					for(int i=0; i<questions.at(currentQuestion-1).answer.size(); ++i)
					{
						if(stringUtils::joinWords(words) == questions.at(currentQuestion-1).answer.at(i))
						{
							ircNet.sendMsg(stringUtils::msgChannel(message), stringUtils::msgNick(message) + ": Correct answer!  Congratulations!");
							currentQuestion = 0;
							if(continuousQuestions == true)
							{
								setTimedMsg("", stringUtils::msgChannel(message), stringUtils::tokenize("randquestion"), quizTiming);
							}
							break;
						}
					}
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

int setTimedMsg(string nick, string channel, vector<string> words, int seconds)
{
	static int id = 0;
	++id;
	timedMsg result;
	result.id = id;
	result.time = time(NULL) + seconds;
	result.nick = nick;
	result.channel = channel;
	result.words = words;
	timedMessages.push_back(result);
	return 0;
}

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
	}
	else if(words.at(0) == "flip")
	{
		if(rand()%2 >= 1)
		{
			reply += "Heads!";
		}
		else
		{
			reply += "Tails!";
		}
		ircNet.sendMsg(channel, reply);
	}
	else if(words.at(0) == "flop")
	{
		ircNet.sendMe(channel, "flops about on the floor.");
	}
	else if(words.at(0) == "count")
	{
		string reply = "You gave " + stringUtils::toString(words.size()) + " parameters.";
		ircNet.sendMsg(channel, reply);
	}
	else if(words.at(0) == "become")
	{
		if(words.size() < 2)
		{
			reply += "You need to give a name";
			ircNet.sendMsg(channel, reply);
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
				reply += "Setting timer for " + stringUtils::toString(time) + " seconds.";
				ircNet.sendMsg(channel, reply);
				setTimedMsg(nick, channel, stringUtils::tokenize("reply Times up"), time);
			}
		}
	}
	else if(words.at(0) == "in")
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
				setTimedMsg(nick, channel, command, time);
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
		currentQuestion = number;
		ircNet.sendMsg(channel, reply + getQuestion(number));
		setTimedMsg(nick, channel, stringUtils::tokenize("answer " + stringUtils::toString(number)), quizTiming);
	}
	else if(words.at(0) == "loadquestions")
	{
		if(words.size() < 2)
		{
			ircNet.sendMsg(channel, reply + "Format: !loadquestions file");
		}
		else
		{
			ircNet.sendMsg(channel, reply + loadQuestions("questions/" + words.at(1)));
		}
	}
	else if(words.at(0) == "randquestion")
	{
		if(questions.size() > 0)
		{
			int number = (rand()%(questions.size()))+1;
			currentQuestion = number;
			ircNet.sendMsg(channel, reply + getQuestion(number));
			setTimedMsg(nick, channel, stringUtils::tokenize("answer " + stringUtils::toString(number)), quizTiming);
		}
		else
		{
			ircNet.sendMsg(channel, reply + "No questions");
		}
	}
	else if(words.at(0) == "answer")
	{
		if(words.size() < 2)
		{
			ircNet.sendMsg(channel, reply + "Format: !answer id");
		}
		else
		{
			int number = stringUtils::fromString<int>(words.at(1));
			if(number == currentQuestion)
			{
				ircNet.sendMsg(channel, reply + "No one got the answer which was: \"" + getAnswer(number) + "\"");
				currentQuestion = 0;
				if(continuousQuestions == true)
				{
					setTimedMsg("", channel, stringUtils::tokenize("randquestion"), quizTiming);
				}
			}
		}
	}
	else if(words.at(0) == "startquiz")
	{
		continuousQuestions = true;
		setTimedMsg("", channel , stringUtils::tokenize("randquestion"), quizTiming);
		ircNet.sendMsg(channel, reply + "Starting quiz!");
	}
	else if(words.at(0) == "stopquiz")
	{
		continuousQuestions = false;
		ircNet.sendMsg(channel, reply + "Will stop quiz after this question.");
	}
	else if(words.at(0) == "questioninfo")
	{
		ircNet.sendMsg(channel, reply + "Question ID: " + stringUtils::toString(currentQuestion) + "; Category: " + questions.at(currentQuestion-1).category + "; File: " + questions.at(currentQuestion-1).file);
	}
	else if(words.at(0) == "quiztime")
	{
		if(words.size() < 2)
		{
			ircNet.sendMsg(channel, reply + "The quiz timing is currently set to " + stringUtils::toString(quizTiming) + ".");
		}
		else
		{
			int newTiming = stringUtils::fromString<int>(words.at(1));
			ircNet.sendMsg(channel, reply + "Setting quiz timing to " + stringUtils::toString(newTiming) + ".");
			quizTiming = newTiming;
		}
	}
	else if(words.at(0) == "clearquestions")
	{
		questions.clear();
		ircNet.sendMsg(channel, reply + "Cleared questions, please use !loadquestions to add some more.");
	}
	else if(words.at(0) == "httpget")
	{
		string hostname = "zed0.uwcs.co.uk";
		string path = "/";
		string port = "80";
		if(words.size() > 1)
		{
			hostname = stringUtils::urlHostname(words.at(1));
			path = stringUtils::urlPath(words.at(1));
			port = stringUtils::urlPort(words.at(1));
		}
		cout << "Getting " << hostname << " " << path << " " << port << endl;
		http httpNet(hostname, port);
		string result = httpNet.get(path);
		//cout << result << endl;
	}
	else if(words.at(0) == "test")
	{
		if(words.size() > 1)
		{
			ircNet.sendMsg(channel, reply + "Hostname: " + stringUtils::urlHostname(words.at(1)) + " Path: " + stringUtils::urlPath(words.at(1)) + " Port: " + stringUtils::urlPort(words.at(1)));
		}
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

string getAnswer(int questionNum)
{
	if(questionNum > questions.size() || questionNum <= 0)
	{
		return "Question does not exist.";
	}
	else
	{
		return questions.at(questionNum-1).answer.at(0);
	}
}

string loadQuestions(string fileName)
{
	ifstream questionFile(fileName.c_str(), ifstream::in);
	if(questionFile.good())
	{
		int id = 0;
		string buffer;
		stringstream currentQuestion;
		while(getline(questionFile, buffer))
		{
			if(buffer.size()<5)
			{
				continue;
			}
			question result;
			result.id = ++id;
			result.file = fileName;
			char seperator = buffer.at(0);
			buffer.erase(0,1);
			currentQuestion << buffer;
			string part;
			getline(currentQuestion, part, seperator);
			result.category = part;
			while(getline(currentQuestion, part, seperator))
			{
				if(part == "")
				{
					break;
				}
				else
				{
					result.answer.push_back(part);
				}
			}
			getline(currentQuestion, part);
			result.question = part;
			questions.push_back(result);
			currentQuestion.clear();
/*
			cout << "Question " << result.id << "; file: " << result.file << "; category:" << result.category << "; answers: ";
			for(int i=0; i<result.answer.size(); ++i)
			{
				cout << result.answer.at(i) << "/";
			}
			cout << "/" << result.question << endl;
*/
		}
		questionFile.close();
		return "Finished loading " + stringUtils::toString(id-1) + " questions from " + fileName + " (total now " + stringUtils::toString(questions.size()) + ").";
	}
	else
	{
		return "There was a problem loading from " + fileName + ".";
	}
}
