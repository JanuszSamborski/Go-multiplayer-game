#include<iostream>
#include<string.h>
#include<unistd.h>
#include<exception>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<cstdint>

#define MAXLINE 4
#define LISTENQ 2

using namespace std;

struct point
{
	uint8_t x,y;
};

struct tlv
{
	uint8_t type;
	uint8_t length;
	point value;
};



class networkingServer
{
	int	listenfd, connfd;
	socklen_t len;
	char buff[MAXLINE], str[INET6_ADDRSTRLEN+1];
	char recvline[MAXLINE];
	time_t ticks;
	struct sockaddr_in6	servaddr, cliaddr;

public:
	networkingServer()
	{
		if((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0)
			throw logic_error("Socket error: " + (string)strerror(errno));

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin6_family = AF_INET6;
		servaddr.sin6_addr   = in6addr_any;
		servaddr.sin6_port   = htons(25565);

		if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
			throw logic_error("Bind error: " + (string)strerror(errno));

		if(listen(listenfd, LISTENQ) < 0)
			throw logic_error("Listen error: " + (string)strerror(errno));

		len = sizeof(cliaddr);
		if((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) < 0)
			throw logic_error("Accept error: " + (string)strerror(errno));
		// TODO the rest

		// for debug prints ip addr
		bzero(str, sizeof(str));
		inet_ntop(AF_INET6, (struct sockaddr  *) &cliaddr.sin6_addr,  str, sizeof(str));
		printf("Connection from %s\n", str);
		// end debug

		// till here
	}

	~networkingServer()
	{
		close(connfd);
	}

	void sendMessage(tlv message)
	{
		buff[0]=message.type;
		buff[1]=message.length;
		buff[2]=message.value.x;
		buff[3]=message.value.y;


		if(write(connfd, buff, 4)< 0)
			throw logic_error("Write error: " + (string)strerror(errno));
	}

	tlv receiveMessage()
	{
		tlv received_value;
		if(int n = read(connfd, recvline, 4)<=0)
			throw logic_error("Read error" + to_string(n));

		received_value.type = (uint8_t)recvline[0];
		received_value.length = (uint8_t)recvline[1];
		switch (received_value.length) {
			case 2:
				received_value.value.y=(uint8_t)recvline[3];
			case 1:
				received_value.value.x=(uint8_t)recvline[2];
			break;
		}
		return received_value;
	}

};

class networkingClient
{

	int	sockfd, n, err;
	struct sockaddr_in6	servaddr;
	char recvline[MAXLINE], buff[MAXLINE];

public:
	networkingClient(char IP[])
	{

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

		// TODO: the rest

		// till here
	}

	~networkingClient()
	{
		// TODO: closing routine
	}

	void sendMessage(tlv message)
	{
		buff[0]=message.type;
		buff[1]=message.length;
		buff[2]=message.value.x;
		buff[3]=message.value.y;


		if(write(sockfd, buff, 4)< 0)
			throw logic_error("Write error: " + (string)strerror(errno));
	}

	tlv receiveMessage()
	{
		tlv received_value;
		if(int n = read(sockfd, recvline, 4)<=0)
			throw logic_error("Read error" + to_string(n));

		received_value.type = (uint8_t)recvline[0];
		received_value.length = (uint8_t)recvline[1];
		switch (received_value.length) {
			case 2:
				received_value.value.y=(uint8_t)recvline[3];
			case 1:
				received_value.value.x=(uint8_t)recvline[2];
			break;
		}
		return received_value;
	}


};
