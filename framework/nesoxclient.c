#include "nesox.h"

int main(int argc, char *argv[])
{
	printf("%s\n", "nesoxclient: enter");

	char *ipaddress = "127.0.0.1";
	short portnum = 8848;
	unsigned amount = 1024;

	if (argc != 4) {
		printf("%s\n", "usage: nesoxclient host port amount [B|K|M|G]");
		printf("%s\n", "               default: 127.0.0.1:8848 1024 B");
	}
	else {
		ipaddress = argv[1];
		portnum = (short)atoi(argv[2]);
		amount = (unsigned)atoi(argv[3]);
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
	snprintf(datasize, sizeof(datasize), "%u", amount);
	numwrite = write(socketfd, datasize, (size_t)strlen(datasize));

	char *datastore = (char *)malloc(amount * sizeof(char));
	if (datastore == NULL) { printf("%s\n", "malloc failed!"); return -1; }
	printf("datastore size: %u\n", amount);

	ssize_t allread = 0;
	ssize_t numread = 0;

	while (allread < amount) {
		while (((numread = recv(socketfd, datastore + allread, amount - allread, 0)) == -1)
			&& (errno == EINTR))
			;
		allread += numread;
	}
	printf("num read: %zd\n", allread);

	for (int i = 0; i < amount; i++) putchar(datastore[i]);
	printf("num read: %zd\n", allread);

	free(datastore);
	close(socketfd);
	printf("\n");
	fflush(NULL);

	printf("%s\n", "nesoxclient: exit");
	return 0;
}

