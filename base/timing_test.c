#include "timing.h"
#include <unistd.h>

int main(){
	printf("%s: enter!\n", timestamp());

	timepoint start;
	timepin(&start);

	sleep(3);

	timepoint currt;
	timepin(&currt);

	timerange interval = timeint(start, currt);

	printf("time elapse: %.4f seconds!\n", timeint(start, currt));
	printf("time elapse: %.4f seconds!\n", interval);

	printf("%s: leave!\n", timestamp());

return 0; }
