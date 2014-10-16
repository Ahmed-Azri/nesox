#ifndef MESSAGE
#define MESSAGE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define numdigitschar 3
#define numdigitslong 20
#define numdigitsmssg 24

#define messageformat "%03d%020ld"

enum messagetype { HELLO = 0, ECHO = 1 };

typedef struct message {
	char  type;
	long  size;
} message;

message messageinit(char type, long size);

char *encode(message *m);
message decode(char *ms);

int putmessage(int des, message *m);
int getmessage(int des, message *m);

int messagecmp(message s, message t);

#endif

