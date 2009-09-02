#ifndef NETWORK_H
#define NETWORK_H
#include "network.h"
#endif
#include <sstream>

using namespace std;

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
