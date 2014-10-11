#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define maxbuffersize 0xffffffff

int main(int argc, char *argv[])
{
	printf("%s\n", "dataloader: enter");

	if (argc != 2) {
		printf("%s\n", "usage: dataloader filename");
		return -1;
	}

	struct stat filestatus;
	int result = stat(argv[1], &filestatus);
	printf("[%d] file size: %lld\n", result, filestatus.st_size);
	long filesize = filestatus.st_size;

	long buffersize = maxbuffersize;
	if (buffersize < filesize) { printf("%s\n", "buffer is not capable!"); return -1; }

	char *buffer = (char *)malloc(buffersize * sizeof(char));
	if (buffer == NULL) { printf("%s\n", "malloc failed!"); return -1; }
	printf("%s\n", "malloc successed!");
	printf("buffer size: %ld\n", buffersize);


	int fd;
	fd = open(argv[1], O_RDONLY, S_IRUSR);
	if (fd < 0) { printf("%s\n", "open failed"); return -1; }
	printf("%s\n", "open successed!");
	printf("file des: %d\n", fd);

	int numread = read(fd, buffer, filesize);
	if (numread < 0) perror("read failed: ");
	printf("file des: %d\n", fd);
	printf("num read: %d\n", numread);

	// for (int i = 0; i < numread; i++) putchar(buffer[i]);

	free(buffer);
	close(fd);

	printf("%s\n", "dataloader: exit");
	return 0;
}
