#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <strings.h>

#define maxbuffersize 0x100

int main(int argc, char *argv[])
{
	printf("%s\n", "nesoxclient: enter");

	char ipaddress[0x10] = "127.0.0.1";
	short portnum = 8848;

	if (argc != 3) {
		printf("%s\n", "usage: nesoxclient host port amount [M|G]");
		printf("%s\n", "             default: 127.0.0.1:8848 1024");
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
	if (result < 0) {
		perror("perror: connect error!");
		return -1;
	}
	printf("%s\n", "connect successed!");

	ssize_t numwrite = 0;

	char datasize[] = "4294967295";
	numwrite = write(socketfd, datasize, (size_t)strlen(datasize));



	close(socketfd);
	printf("%s\n", "nesoxclient: exit");
	return 0;
}
