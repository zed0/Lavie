#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace stringUtils
{
	//gets the nick that a message was sent from
	string msgNick(string message);
	//gets the channel that a message was sent to
	string msgChannel(string message);
	//gets the individual words from a message
	vector<string> msgWords(string message);
	//tokenize the message
	vector<string> tokenize(string message);
	//checks whether the character is numeric
	bool charIsNum(char character);
	//parses the string into a int given a time formated as 1d2h3m4s etc
	int parseTime(string time);
	//joins a vector of strings, turning it in to a single space seperated string
	string joinWords(vector<string> words);
	//remove the protocol from the start of a url
	string stripProtocol(string url);
	//some functions to get various parts of a url:
	string urlHostname(string url);
	string urlPath(string url);
	string urlPort(string url);

	//Due to C++ being rubish you have to declare and implement templates in the same file,
	//thus these are implemented here.

	//converts from a string to an arbitray numerical format
	template<class T> T fromString(const string& s)
	{
		istringstream stream(s);
		T t;
		stream >> t;
		return t;
	}
	//converts to a string from an arbitrary numerical format
	template<class T> string toString(const T& t)
	{
		ostringstream stream;
		stream << t;
		return stream.str();
	}
}
