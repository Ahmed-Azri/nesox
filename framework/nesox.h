#ifndef NESOX
#define NESOX

#include "timing.h"
#include "logger.h"
#include "socker.h"
#include "daemon.h"

#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define maxbuffersize 0x100
#define datastoresize 0xffffffff
#define numbytesofint 0xa
#define optionargsize 0x100
#define cmdmaxargsize 0x100

#endif
