#ifndef PROTOCOL
#define PROTOCOL

#include "nesox.h"

enum messagetype {
	HELLO 		= 0,	// symmetric message
	ECHO_REQ 	= 1,	// symmetric message
	ECHO_REP  	= 2,	// symmetric message
	RETRIEVE	= 3,	// reader -> server
	TRANSFER	= 4,	// server -> reader
	SCHEDULE	= 5, 	// controller -> server
	MSGTYPELEN
};

handler parsemessage(message *m);

int helloreq(int, char*, int);
int hellorep(int, char*, int);

int echoreq(int, char*, int);
int echorep(int, char*, int);

long transfer(int socketdes, char *datastore, long datasize, long requestsize);
long retrieve(int socketdes, char *datastore, long storesize, long replysize);

#endif
