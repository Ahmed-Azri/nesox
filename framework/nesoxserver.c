#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>

#define memzero(mem) bzero(&(mem), sizeof(mem))
#define maxsize 1024

int main(int argc, char *argv[])
{

	printf("nesoxserver: starting!");
	if (argc != 3) {
		printf("usage: nesoxserver host port");
		printf("     default host: 127.0.0.1");
		printf("          default port: 8848");
	}

	char ipaddress[0x10] = "127.0.0.1";
	short portnum = 8848;

	int backlog = 1024;
	int result = 0;
	int listeningfd, connectedfd;
	struct sockaddr_in serveraddress, clientaddress;
	socklen_t length = sizeof(clientaddress);

	listeningfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	memzero(serveraddress);
	memzero(clientaddress);

	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = inet_addr(ipaddress);
	serveraddress.sin_port = htons(portnum);

	result = bind(listeningfd, (struct sockaddr_in *)(&serveraddress), (socklen_t)(sizeof(serveraddress)));
	if (result < 0) { printf("%s: bind error!", *argv); return -1; }

	result = listen(listeningfd, backlog);
	if (result < 0) { printf("%s: listen error!", *argv); return -1; }

	connectedfd = accept(listeningfd, (struct sockaddr_in *)(&clientaddress), &length);
	if (connectedfd < 0) { printf("%s: accept error!", *argv); return -1; }

	//echo server logic
	char buffer[maxsize];
	int bytes = 0;
	while((bytes = recv(connectedfd, buffer, sizeof(buffer), 0) > 0)
	{
		send(connectedfd, buffer, bytes, 0);
	}

	return 0;
}

