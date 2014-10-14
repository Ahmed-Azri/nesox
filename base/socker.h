#ifndef SOCKER
#define SOCKER

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int socker(char *host, short port, struct sockaddr_in *address);

#endif
