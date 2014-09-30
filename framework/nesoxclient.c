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
	printf("%s\n", "nesoxclient: start ... ");

	char ipaddress[0x10] = "127.0.0.1";
	short portnum = 8848;

	if (argc != 3) {
		printf("%s\n", "usage: nesoxclient host port");
		printf("%s\n", "	 default: 127.0.0.1:8848");
		printf("%s\n", "It is better to provide meaningful host address and port!");
	}
	else {
		snprintf(ipaddress, 0x10, "%s", argv[1]);
		portnum = (short)atoi(argv[2]);
	}

	int result = 0;
	int socketfd;
	struct sockaddr_in serveraddress;
	bzero(&serveraddress, sizeof(serveraddress));

	socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = inet_addr(ipaddress);
	serveraddress.sin_port = htons(portnum);

	result = connect(socketfd, (struct sockaddr *)(&serveraddress), (socklen_t)(sizeof(serveraddress)));
	if (result < 0) { printf("%s\n", "connect error!"); return -1; }
	printf("%s\n", "connected successed!");

	char buffer[maxbuffersize] = "Hello, Nesox Server! I am the Nesox Client!";
	ssize_t bytes = 0;
	bytes = send(socketfd, buffer, (size_t)strlen(buffer), 0);

	bzero(buffer, maxbuffersize);
	bytes = recv(socketfd, buffer, sizeof(buffer), 0);
	printf("received: %s", buffer);

	return 0;
}
