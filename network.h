#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

class network
{
	private:
		struct addrinfo *res;
		int sockfd;
	public:
		//initializer takeing the hostname and port (as a string)
		network(string hostname, string port);
		//recieve a message from the network (put in the buffer, returns message length)
		int recieveMsg(string &buffer);
		//send a message to the network not including newline (returns the number of bytes send)
		int sendMsg(string message);
};
