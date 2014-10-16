#include "protocol.h"

int helloreq(int fd, char *data, int size)
{
	static char hellorequest[] = "Hello, this is Nesox (REQ)!!!";
	message m = messageinit(HELLO, sizeof(hellorequest));
	putmessage(fd, &m);
	putdata(fd, hellorequest, sizeof(hellorequest));
	logtrace("hello req: %s", hellorequest);

	return 0;
}

int hellorep(int fd, char *data, int size)
{
	static char buffer[maxbuffersize];
	data = buffer;
	//optional handler task 1: getdata
	if (size > 0) {
		getdata(fd, data, size);
		logtrace("hello rep: %s", data);
	}
	//optional handler task 2: putdata
	static char helloreply[] = "Hello, this is Nesox (REP)!!!";
	message m = messageinit(HELLO, sizeof(helloreply));
	putmessage(fd, &m);
	putdata(fd, helloreply, sizeof(helloreply));
	logtrace("hello rep: %s", helloreply);
	return 0;
}

long transfer(int socketdes, char *datastore, long datasize, long requestsize)
{
	long allput = 0;
	long numput = 0;

	int n = requestsize / datasize;
	int r = requestsize % datasize;

	for (int i = 0; i < n; i++){
		numput = putdata(socketdes, datastore, datasize);
		if (numput < 0) return -1;
		else allput += numput;
	}
	logtrace("put size [%d] data [%d] times", datasize, n);
	numput = putdata(socketdes, datastore, r);
	if (numput < 0) return -1;
	else allput += numput;

	return allput;
}

long retrieve(int socketdes, char *datastore, long storesize, long replysize)
{
	long allget = 0;
	long numget = 0;

	int n = replysize / storesize;
	int r = replysize % storesize;

	for (int i = 0; i < n; i++) {
		numget = getdata(socketdes, datastore, storesize);
		if (numget < 0) return -1;
		else allget += numget;
	}
	logtrace("get size [%d] data [%d] times", storesize, n);
	numget = getdata(socketdes, datastore, r);
	if (numget < 0) return -1;
	else allget += numget;

	return allget;
}

handler parsemessage(message *m)
{
	handler fun = NULL;
	switch (m->type) {
		case HELLO:
			fun = hellorep;
			break;
		case ECHO_REQ: break;
		case ECHO_REP: break;
		case RETRIEVE:
			fun = transfer;
			break;
		case TRANSFER: break;
		case SCHEDULE: break;
		default: break;
	}
	return fun;
}

