#ifndef PROTOCOL
#define PROTOCOL

#include "message.h"
#include "handler.h"

enum messagetype {
	HELLO 		= 0,	// symmetric message
	ECHO_REQ 	= 1,	// symmetric message
	ECHO_REP  	= 2,	// symmetric message
	MSGTYPELEN
};

handler parsemessage(message *m);

#endif
