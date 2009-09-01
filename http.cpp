#include "http.h"

using namespace std;

http::http(string hostname, string portname)
{
	cout << "debug4" << endl;
	host = hostname;
	port = portname;
	server = new network(host, port);
}

http::~http()
{
	delete server;
}

string http::get(string path)
{
	server->sendMsg("GET " + path + " HTTP/1.1");
	server->sendMsg("Host: " + host + ":" + port);
	server->sendMsg("Connection: close");
	server->sendMsg("");
	string result;
	string message;
	while(server->recieveMsg(message) > 0)
	{
		result += message;
	}
	return result;
}
