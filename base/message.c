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
	int num = snprintf(buffer, sizeof(buffer), "%03.3d%020.20ld", m->type, m->size);
	fprintf(stderr, "encode: num = [%d]\n", num);
	fprintf(stderr, "encode: buf = [%23s]\n", buffer);
	for (int i = 0; i < numdigitsmssg; i++) fprintf(stderr, "[%02d][%01c][%02d]\n", i, buffer[i], buffer[i]);
	return buffer;
}

