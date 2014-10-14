#include "daemon.h"
#include "logger.h"

int main(){

	daemonize(1, 0);

	if (logopen("daemon.log") < 0)
		perror("main: logopen failed!");

	sleep(3);

	printf("%s\n", "hello!!!");

	LOG("Hello, LOG!!!");

	logclose();
	return 0;
}

