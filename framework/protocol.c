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

handler parsemessage(message *m)
{
	handler fun = NULL;
	switch (m->type) {
		case HELLO:
			fun = hellorep;
			break;
		case ECHO_REQ: break;
		case ECHO_REP: break;
		default: break;
	}
	return fun;
}

