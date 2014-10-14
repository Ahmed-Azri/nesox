#include "logger.h"

#define FILE_PERMS ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )
#define OPEN_FLAGS ( O_WRONLY | O_APPEND | O_CREAT )

static int logfd = -1;
static int result = 0;

static char *getseverity(int severity)
{
	static char *pseverity = "TRACE";
	if (severity == ERROR) pseverity = "ERROR";
	if (severity == STATS) pseverity = "STATS";
	return pseverity;
}

static char *getlogtime()
{
	static char loggingtime[0x5f];
	time_t t;
	time(&t);
	strftime(loggingtime, 0x5f, "%Y%m%d%H%M%S", localtime(&t));
	return loggingtime;
}

static ssize_t dogwrite(int filedes, void *buffer, size_t size) {
   ssize_t bytes;

   while (((bytes = write(filedes, buffer, size)) == -1) && (errno == EINTR));
   return bytes;
}

int logopen(char *filename)
{
   logfd = open(filename, OPEN_FLAGS, FILE_PERMS);
   return logfd;
}

int logclose()
{
	result = close(logfd);
	return result;
}

int logprintf(char *file, char *function, int line, int severity, char *format, ...)
{
	static char header[0x100];
	if (file != NULL && function != NULL && line != -1)
		snprintf(header, sizeof(header), "%s %s(%10s):%04d ", getlogtime(), file, function, line);
	else
		snprintf(header, sizeof(header), "%s ", getlogtime());

	static char message[0x400];
	va_list arglist;
	va_start(arglist, format);
		vsnprintf(message, sizeof(message), format, arglist);

	static char buffer[0x500];
	ssize_t len = snprintf(buffer, sizeof(buffer), "%s[%s] %s\n", header, getseverity(severity), message);

	return (int) dogwrite(logfd, buffer, len);
}

