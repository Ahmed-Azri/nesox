#ifndef TIMING
#define TIMING

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

char *timestamp(void);

typedef struct timeval timepoint;
typedef double timerange;

timepoint timepin(timepoint *instant);
timerange timeint(timepoint s, timepoint e);

#endif
