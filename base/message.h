#ifndef MESSAGE
#define MESSAGE

#include <stdio.h>
#include <string.h>

#define numdigitschar 3
#define numdigitslong 20
#define numdigitsmssg 24

enum messagetype { HELLO = 0, ECHO = 1 };

typedef struct message {
	char  type;
	long  size;
} message;

message messageinit(char type, long size, message *m);

char *encode(message *m);
char *decode(message *m);

int putmessage(int des, message *m);
int getmessage(int des, message *m);

#endif


