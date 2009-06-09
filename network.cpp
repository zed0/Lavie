#include "network.h"

using namespace std;

network::network(string hostname, string port)
{
	struct addrinfo *p;
	struct addrinfo hints = {0};
	sockfd = 0;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &res)) != 0)
	{
		cerr << "getaddrinfo: " << gai_strerror(status) << endl;
	}

	cout << "Connecting to: " << hostname;

	for(p = res; p != NULL; p = p->ai_next)
	{
		void *addr;
		string ipver;

		//get the pointer to the address itself,
		//different fields in IPv4 and IPv6:
		if(p->ai_family == AF_INET)
		{
			//IPv4
			struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in *>(p->ai_addr);
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		}
		else
		{
			//IPv6
			struct sockaddr_in6 *ipv6 = reinterpret_cast<struct sockaddr_in6 *>(p->ai_addr);
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		//convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
		cout << " (" << ipver << ": " << ipstr << ")" << endl;
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	connect(sockfd, res->ai_addr, res->ai_addrlen);
}

int network::recieveMsg(string &result)
{
	int numbytes = 0;
	const int MAX_SIZE = 10000;
	char temp[MAX_SIZE];

	numbytes = recv(sockfd, temp, MAX_SIZE-1, 0);
	if(numbytes == -1)
	{
		if(errno != EWOULDBLOCK)
		{
			perror("recv");
			close(sockfd);
			freeaddrinfo(res);
			return -1;
		}
		return 0;
	}
	else if(numbytes == 0)
	{
		cout << "Disconnected" << endl;
		close(sockfd);
		freeaddrinfo(res);
		return -1;
	}
	else
	{
		temp[numbytes] = '\0';
		result = string(temp);
		cout << result;
	}
	return numbytes;
}

int network::sendMsg(string message)
{
	int sent;
	int len;
	message = message + "\n";
	cout << "Sending: " << message;
	len = message.size();
	sent = send(sockfd, message.c_str(), len, 0);
	//cout << "(sent " << sent << "/" << len << " bytes)" << endl;
	return len;
}
