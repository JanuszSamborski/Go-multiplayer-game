#include<iostream>
#include<string.h>
#include<unistd.h>
#include<exception>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define MAXLINE 1024
#define LISTENQ 2    

using namespace std;

class networkingServer
{
	int	listenfd, connfd;
	socklen_t len;
	char buff[MAXLINE], str[INET6_ADDRSTRLEN+1];
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

		// TODO the rest

		for( ; ; )
		{
			len = sizeof(cliaddr);

			if((connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)) < 0)
				throw logic_error("Accept error: " + (string)strerror(errno));

			bzero(str, sizeof(str));

			inet_ntop(AF_INET6, (struct sockaddr  *) &cliaddr.sin6_addr,  str, sizeof(str));

			printf("Connection from %s\n", str);

			ticks = time(NULL);
			snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

			if(write(connfd, buff, strlen(buff))< 0)
				fprintf(stderr,"write error : %s\n", strerror(errno));

			close(connfd);
		}

		// till here
	}
};

class networkingClient
{

	int	sockfd, n, err;
	struct sockaddr_in6	servaddr;
	char recvline[MAXLINE + 1];

public:
	networkingClient()
	{

		char IP[] = "2001:db8:0:f101::1";

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

		while((n = read(sockfd, recvline, MAXLINE)) > 0)
		{
			recvline[n] = 0;
			if (fputs(recvline, stdout) == EOF)
			{
				fprintf(stderr,"fputs error : %s\n", strerror(errno));
				//return 1;
			}
		}

		if(n < 0)
			fprintf(stderr,"read error : %s\n", strerror(errno));

		fprintf(stderr,"OK\n");
		fflush(stdout);
		fgetc(stdin);

		// till here
	}

};