#include "message.h"

// TODO: use snprintf to encapsulate message
// TODO: use sscanf to parse message

message messageinit(char type, long size, message *m)
{
	m->type = type;
	m->size = size;
	return *m;
}

char *encode(message *m)
{
	static char buffer[numdigitsmssg];
	int num = snprintf(buffer, sizeof(buffer), "%03d%020ld", m->type, m->size);
	if (num != numdigitschar + numdigitslong ) return NULL;
	return buffer;
}

message decode(char *s)
{
	message m;
	int num = sscanf(s, "%03d%020ld", &m.type, &m.size);
	fprintf(stderr, "decode: %03d\n", m.type);
	fprintf(stderr, "decode: %020ld\n", m.size);
	return m;
}

