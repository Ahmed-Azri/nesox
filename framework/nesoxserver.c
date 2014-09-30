#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>

#define maxbuffersize 1024

int main(int argc, char *argv[])
{
	printf("%s\n", "nesoxserver: start ... ");

	char ipaddress[0x10] = "127.0.0.1";
	short portnum = 8848;

	if (argc != 3) {
		printf("%s\n", "usage: nesoxserver host port");
		printf("%s\n", "	 default: 127.0.0.1:8848");
	}
	else {
		snprintf(ipaddress, 0x10, "%s", argv[1]);
		portnum = (short)atoi(argv[2]);
	}

	int backlog = 1024;
	int result = 0;
	int listeningfd, connectedfd;
	struct sockaddr_in serveraddress, clientaddress;
	socklen_t length = sizeof(clientaddress);

	listeningfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	bzero(&serveraddress, sizeof(serveraddress));
	bzero(&clientaddress, sizeof(clientaddress));

	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = inet_addr(ipaddress);
	serveraddress.sin_port = htons(portnum);

	result = bind(listeningfd, (struct sockaddr *)(&serveraddress), (socklen_t)(sizeof(serveraddress)));
	if (result < 0) { printf("%s: bind error!\n", *argv); return -1; }
	printf("bind successed!\n");

	result = listen(listeningfd, backlog);
	if (result < 0) { printf("%s: listen error!\n", *argv); return -1; }
	printf("start listening!\n");

	connectedfd = accept(listeningfd, (struct sockaddr *)(&clientaddress), &length);
	if (connectedfd < 0) { printf("%s: accept error!\n", *argv); return -1; }
	printf("accept new connection!\n");

	//echo server logic
	char buffer[maxbuffersize];
	ssize_t bytes = 0;
	bzero(buffer, maxbuffersize);
	bytes = recv(connectedfd, buffer, sizeof(buffer), 0);
	printf("received: %s", buffer);
	send(connectedfd, buffer, (size_t)bytes, 0);

	return 0;
}
