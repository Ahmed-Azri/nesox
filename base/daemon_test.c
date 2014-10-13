#include "daemon.h"
#include "logger.h"

int main(){

	logopen("daemon.log");
	daemonize("daemonize:");

	logprintf("Hello, this is a daemon!");
	logsend();
	logclose();

	sleep(10);
	return 0;
}/*main*/

