#ifndef HTTP_H
#define HTTP_H
#include "network.h"
#include <sstream>

using std::stringstream;

class http
{
	private:
		network *server;
		string host;
		string port;
	public:
		http(string hostname, string port="80");
		~http();
		string get(string path);
};

#endif
