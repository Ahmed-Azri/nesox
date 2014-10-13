#include "nesox.h"

void timestamp(){
	time_t t;
	time(&t);
	char timestring[0x5f];
	strftime(timestring, 0x5f, "%Y%m%d - %H%M%S", localtime(&t));
	printf("timestamp: %s\n", timestring);
}

int main(){

	timestamp();

	struct timeval start;
	struct timeval currt;
	gettimeofday(&start, NULL);
	sleep(3);
	gettimeofday(&currt, NULL);

	double elapse = (currt.tv_sec - start.tv_sec) + (currt.tv_usec - start.tv_usec)/1000000.0;

	printf("%.4f seconds elapsed\n", elapse);

	timestamp();

return 0; }

