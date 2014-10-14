#include "daemon.h"
#include "logger.h"

int main(){

	daemonize(1, 0);

	int result = 0;
	result = logopen("daemon.log");
	perror("main: logopen failed!");

	logprintf("Hello, this is a daemon!");
	logsend();

	sleep(3);

	printf("%s\n", "hello!!!");

	LOG("Hello, LOG!!!");
	logclose();

	return 0;
}

