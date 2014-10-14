#include "nesox.h"

int main(int argc, char *argv[])
{
	printf("%s\n", "nesoxserver: enter");

	int result = 0;

	char *ipaddress = "127.0.0.1";
	short portnum = 8848;
	char *filename = "../data/bible.txt";

	if (argc != 4) {
		printf("%s\n", "usage: nesoxserver host port filename");
		printf("%s\n", "default: 127.0.0.1:8848 ../data/bible.txt");
	}
	else {
		ipaddress = argv[1];
		portnum = (short)atoi(argv[2]);
		filename = argv[3];
	}

	struct stat filestatus;
	result = stat(filename, &filestatus);
	if (result < 0) { printf("%s\n", "stat failed!"); return -1; }
	long filesize = filestatus.st_size;
	printf("file size: %ld\n", filesize);

	if (datastoresize < filesize) { printf("%s\n", "datastoresize is not capable!"); return -1; }

	char *datastore = (char *)malloc(datastoresize * sizeof(char));
	if (datastore == NULL) { printf("%s\n", "malloc failed!"); return -1; }
	printf("datastore size: %u\n", datastoresize);

	int fd = open(filename, O_RDONLY, S_IRUSR);
	if (fd < 0) { printf("%s\n", "open file failed!"); return -1; }
	printf("%s\n", "open file successed!");
	printf("file des: %d\n", fd);

	int numread = read(fd, datastore, filesize);
	if (numread < 0) { printf("%s\n", "read file failed!"); return -1; }
	printf("file des: %d\n", fd);
	printf("num read: %d\n", numread);
	close(fd);

	int listeningfd, connectedfd;
	listeningfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listeningfd < 0) { printf("%s\n", "socket failed!"); return -1; }
	printf("socket successed!\n");


	struct sockaddr_in serveraddress, clientaddress;
	socklen_t length = sizeof(struct sockaddr_in);
	bzero(&serveraddress, sizeof(serveraddress));
	bzero(&clientaddress, sizeof(clientaddress));

	serveraddress.sin_family = AF_INET;
	serveraddress.sin_addr.s_addr = inet_addr(ipaddress);
	serveraddress.sin_port = htons(portnum);

	result = bind(listeningfd, (struct sockaddr *)(&serveraddress), (socklen_t)(sizeof(serveraddress)));
	if (result < 0) { perror("perror: bind error!"); return -1; }
	printf("bind successed!\n");

	int backlog = 1024;
	result = listen(listeningfd, backlog);
	if (result < 0) { perror("perror: listen error!"); return -1; }
	printf("start listening!\n");

	int counter, status = 1;
	for (counter = 1; status; counter++) {

		connectedfd = accept(listeningfd, (struct sockaddr *)(&clientaddress), &length);
		if (connectedfd < 0) { perror("perror: accept error!"); return -1; }
		printf("\n");
		printf("accept new connection!");
		printf(" counter:%d\n", counter);

		char buffer[maxbuffersize];
		ssize_t numread = 0;
		bzero(buffer, maxbuffersize);

		numread = read(connectedfd, buffer, numbytesofint);
		unsigned datasize = atoi(buffer);
		printf("num read: %zd\n", numread);
		printf("buffer: %s\n", buffer);
		printf("datasize: %u\n", datasize);
		if (datasize > filesize) { printf("%s\n", "NOT enough data to feed!"); close(connectedfd); continue; }

		ssize_t allwrite = 0;
		ssize_t numwrite = 0;

		while (allwrite < datasize) {
			while (((numwrite = send(connectedfd, datastore + allwrite, datasize - allwrite, 0)) == -1)
				&& (errno == EINTR))
				;
			allwrite += numwrite;
		}
		printf("num write: %zd", numwrite);

		fflush(NULL);
		close(connectedfd);
	}

	free(datastore);
	close(listeningfd);

	printf("%s\n", "nesoxserver: exit");
	return 0;
}
