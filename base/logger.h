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

#define LOG(...) logprintf( __VA_ARGS__ ), logflush()

#define TRACE 0
#define ERROR 1
#define SINFO 2

int logopen(char *filename);
int logclose(void);

int logprintf(char *format, ...);
int logflush(void);

#endif
