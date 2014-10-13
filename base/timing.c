#include "timing.h"


char *timestamp()
{
	static char timestampstring[0x5f];
	time_t t;
	time(&t);
	strftime(timestampstring, 0x5f, "%Y%m%d%H%M%S", localtime(&t));
	return timestampstring;
}

timepoint timepin(timepoint *instant)
{
	gettimeofday(instant, NULL);
	return *instant;
}

timerange timeint(timepoint s, timepoint e)
{
	return ( e.tv_sec - s.tv_sec ) + ( e.tv_usec - s.tv_usec)/1000000.0;
}

