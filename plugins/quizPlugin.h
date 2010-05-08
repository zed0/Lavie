#ifndef QUIZPLUGIN_H
#define QUIZPLUGIN_H

#include "../plugin.h"
#include "timePlugin.h"
#include "filePlugin.h"
#include "../stringUtils.h"
#include <cstdlib>

struct question
{
	int id;
	string question;
	vector<string> answer;
	string category;
	string file;
};

class quizPlugin:public plugin
{
	private:
		static vector<question> questions;
		static int currentQuestion;
		static int quizTiming;
		static bool continuousQuestions;
	public:
		string loadQuestions(string fileName)
		{
			stringstream questionFile;
			if(fileName.find("http://")!=string::npos)
			{
				filePlugin::loadHttp(fileName, questionFile);
			}
			else
			{
				filePlugin::loadFile("questions/"+fileName, questionFile);
			}
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
			return "Finished loading " + stringUtils::toString(id) + " questions from " + fileName + " (total now " + stringUtils::toString(questions.size()) + ").";
		}

		string getQuestion(int questionNum)
		{
			if(questionNum > questions.size() || questionNum <= 0)
			{
				return "Question does not exist.";
			}
			else
			{
				return "[" + questions.at(questionNum-1).category + "] " + questions.at(questionNum-1).question;
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

		int handleCommand(string nick, string channel, vector<string> words)
		{
			string reply = "";
			if(nick != "")
			{
				reply += nick + ": ";
			}

			if(words.at(0) == "question")
			{
				int number = 1;
				if(words.size() > 1)
				{
					number = stringUtils::fromString<int>(words.at(1));
				}
				currentQuestion = number;
				ircNet.sendMsg(channel, reply + getQuestion(number));
				timePlugin::setTimedMsg(nick, channel, stringUtils::tokenize("answer " + stringUtils::toString(number)), quizTiming);
				return 1;
			}
			else if(words.at(0) == "loadquestions")
			{
				if(words.size() < 2)
				{
					ircNet.sendMsg(channel, reply + "Format: loadquestions file");
				}
				else
				{
					ircNet.sendMsg(channel, reply + loadQuestions(words.at(1)));
				}
				return 1;
			}
			else if(words.at(0) == "randquestion")
			{
				if(questions.size() > 0)
				{
					int number = (rand()%(questions.size()))+1;
					currentQuestion = number;
					ircNet.sendMsg(channel, reply + getQuestion(number));
					timePlugin::setTimedMsg(nick, channel, stringUtils::tokenize("answer " + stringUtils::toString(number)), quizTiming);
				}
				else
				{
					ircNet.sendMsg(channel, reply + "No questions");
				}
				return 1;
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
							timePlugin::setTimedMsg("", channel, stringUtils::tokenize("randquestion"), quizTiming);
						}
					}
				}
				return 1;
			}
			else if(words.at(0) == "startquiz")
			{
				continuousQuestions = true;
				timePlugin::setTimedMsg("", channel , stringUtils::tokenize("randquestion"), quizTiming);
				ircNet.sendMsg(channel, reply + "Starting quiz!");
				return 1;
			}
			else if(words.at(0) == "stopquiz")
			{
				continuousQuestions = false;
				ircNet.sendMsg(channel, reply + "Will stop quiz after this question.");
				return 1;
			}
			else if(words.at(0) == "questioninfo")
			{
				ircNet.sendMsg(channel, reply + "Question ID: " + stringUtils::toString(currentQuestion) + "; Category: " + questions.at(currentQuestion-1).category + "; File: " + questions.at(currentQuestion-1).file);
				return 1;
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
					if(newTiming < 0)
					{
						reply += "Time must be positive.";
						ircNet.sendMsg(channel, reply);
					}
					else
					{
						ircNet.sendMsg(channel, reply + "Setting quiz timing to " + stringUtils::toString(newTiming) + ".");
						quizTiming = newTiming;
					}
				}
				return 1;
			}
			else if(words.at(0) == "clearquestions")
			{
				questions.clear();
				ircNet.sendMsg(channel, reply + "Cleared questions, please use !loadquestions to add some more.");
				return 1;
			}
			return 0;
		}

		int handleMessage(string nick, string channel, vector<string> words)
		{
			if(currentQuestion != 0 && !questions.empty()) //Handle valid answers to the current question
			{
				for(int i=0; i<questions.at(currentQuestion-1).answer.size(); ++i)
				{
					if(stringUtils::toLower(stringUtils::joinWords(words)) == stringUtils::toLower(questions.at(currentQuestion-1).answer.at(i)))
					{
						ircNet.sendMsg(channel, nick + ": Correct answer!  Congratulations!");
						currentQuestion = 0;
						if(continuousQuestions == true)
						{
							timePlugin::setTimedMsg("", channel, stringUtils::tokenize("randquestion"), quizTiming);
						}
						break;
					}
				}
			}
			return 0;
		}

		int doTick()
		{
			return 0;
		}
};

#endif
