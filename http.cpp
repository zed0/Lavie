#include "http.h"

http::http(string hostname, string portname)
{
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
	//server->sendMsg("Host: " + host + ":" + port);
	server->sendMsg("Host: " + host);
	server->sendMsg("Connection: close");
	server->sendMsg("");
	stringstream total;
	string buffer;
	string header;
	string data = "";
	string message;
	while(server->recieveMsg(message) > 0)
	{
		total << message;
	}
	while(getline(total, buffer) && buffer.find_first_not_of("\r\n") != string::npos)
	{
		header += buffer + "\n";
	}
	while(getline(total, buffer))
	{
		data += buffer + "\n";
	}
	return data;
}
