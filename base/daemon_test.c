#include "daemon.h"
#include "logger.h"
#include "timing.h"

int main(){

	daemonize(1, 0);

	if (logopen("daemon.log") < 0)
		perror("main: logopen failed!");

	sleep(3);

	logtrace("%s: enter!", timestamp());

	timepoint start;
	timepin(&start);

	sleep(3);

	timepoint currt;
	timepin(&currt);

	timerange interval = timeint(start, currt);

	logtrace("time elapse: %.4f seconds!", timeint(start, currt));
	logtrace("time elapse: %.4f seconds!", interval);

	logtrace("%s: leave!", timestamp());

	logclose();
	return 0;
}

