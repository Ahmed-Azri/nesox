#ifndef LOGGER
#define LOGGER

#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int logopen(char *filename);
int logclose(void);
int logprintf(char *format, ...);
int logsend(void);
int logclear(void);

int LOG(char *format, ...);

#endif
