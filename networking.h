#include<iostream>
#include<string.h>
#include<unistd.h>
#include<exception>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<cstdint>

#pragma once

#define MAXLINE 4
#define LISTENQ 2

using namespace std;

struct point
{
	uint8_t x, y;
};

struct tlv
{
	uint8_t type;
	uint8_t length;
	point value;
};

class networkingBase
{

	protected:

	char recvline[MAXLINE], buff[MAXLINE];
	sockaddr_in6 servaddr;
	int	sockfd;

	public:

	virtual bool isServer() = 0;

	void sendMessage(tlv message)
	{
		buff[0] = message.type;
		buff[1] = message.length;
		buff[2] = message.value.x;
		buff[3] = message.value.y;

		if(write(sockfd, buff, MAXLINE) < 0)
			throw logic_error("Write error: " + (string)strerror(errno));
	}

	tlv receiveMessage()
	{
		tlv received_value;
		int n;

		if(n = read(sockfd, recvline, MAXLINE) <= 0)
			throw logic_error("Read error" + to_string(n));

		received_value.type 	= (uint8_t)recvline[0];
		received_value.length	= (uint8_t)recvline[1];

		switch (received_value.length)
		{
			case 2:
				received_value.value.y = (uint8_t)recvline[3];
			case 1:
				received_value.value.x = (uint8_t)recvline[2];
				break;
		}

		return received_value;
	}
};

class networkingServer: public networkingBase
{
	int	listenfd;
	sockaddr_in6 cliaddr;
	socklen_t len;

	public:

	networkingServer()
	{
		int so_reuseaddr = 1;

		if((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
			throw logic_error("Socket error: " + (string)strerror(errno));

		if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr, sizeof so_reuseaddr) < 0)
    		throw logic_error("Setsockopt error: " + (string)strerror(errno));

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin6_family = AF_INET6;
		servaddr.sin6_addr   = in6addr_any;
		servaddr.sin6_port   = htons(25565);

		if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
			throw logic_error("Bind error: " + (string)strerror(errno));

		if(listen(listenfd, LISTENQ) < 0)
			throw logic_error("Listen error: " + (string)strerror(errno));

		len = sizeof(cliaddr);
		if((sockfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) < 0)
			throw logic_error("Accept error: " + (string)strerror(errno));

		char str[INET6_ADDRSTRLEN + 1];
		bzero(str, sizeof(str));
		inet_ntop(AF_INET6, (struct sockaddr  *) &cliaddr.sin6_addr,  str, sizeof(str));
		cout<<"Connection from: " + (string)str<<endl;
	}

	~networkingServer()
	{
		close(listenfd);
		close(sockfd);
	}

	bool isServer()
	{
		return true;
	}
};

class networkingClient : public networkingBase
{

	public:

	networkingClient(char IP[])
	{
		int err;

		if((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
			throw logic_error("Socket error: " + (string)strerror(errno));

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin6_family = AF_INET6;
		servaddr.sin6_port   = htons(25565);

		if((err = inet_pton(AF_INET6, IP, &servaddr.sin6_addr)) == -1)
			throw logic_error("inet_pton error for given IP" + (string)strerror(errno));
		else if(err == 0)
			throw logic_error("Invalid address family");

		if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
			throw logic_error("Connect error: " + (string)strerror(errno));
	}

	~networkingClient()
	{
		close(sockfd);
	}

	bool isServer()
	{
		return false;
	}
};
