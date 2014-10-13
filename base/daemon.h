#ifndef DAEMON
#define DAEMON

#include <stdio.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>
#include <syslog.h>

void daemonize(const char *cmd);

#endif
