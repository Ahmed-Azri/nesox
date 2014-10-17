#include "socker.h"

//todo: add UDP/multicast/broadcast support
//todo: investigate raw socket support feasibility

int socker(char *host, short port, struct sockaddr_in *address)
{
	address->sin_family = AF_INET;
	address->sin_addr.s_addr = inet_addr(host);
	address->sin_port = htons(port);

	return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

