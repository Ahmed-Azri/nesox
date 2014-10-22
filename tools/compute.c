#include "nesox.h"

static char *advance(char *line, char *pattern);

typedef struct flowstat {
	int findex;
	char sindex, dindex;
	long datasize;
	double stime, etime, interval;
} flow;

static int printflow(flow f);
static flow parseflow(char *s, flow *f);
static flow flowinit(int index);

static int debug = 0;

int main(int argc, char *argv[])
{
	fprintf(stderr, "%s: Best Wish!\n", argv[0]);

	char *line = NULL;
	size_t capacity = 0;
	ssize_t linelen;

	for (int i = 0; (linelen = getline(&line, &capacity, stdin)) > 0; i++) {
		if (debug) fprintf(stderr, "%ld/%ld: %s", linelen, capacity, line);

		char *temp = advance(line, "[STATS]");
		char *this = advance(temp, "):");

		flow f = flowinit(i+1);
		parseflow(this, &f);
		printflow(f);

	}

	return 0;
}

char *advance(char *line, char *pattern)
{
	char *p = strstr(line, pattern);
	if (p) p += strlen(pattern);
	return p;
}

int printflow(flow f)
{
	return fprintf(stderr, "%04d: %c:%c:%ld:%0.8f:%0.8f:%0.8f\n",
	f.findex, f.sindex, f.dindex, f.datasize, f.stime, f.etime, f.interval);
}

flow parseflow(char *s, flow *p)
{
	flow f;
	char *string = strdup(s);
	char *token = NULL;
	for (int t = 0; (token = strsep(&string, ":\n")) != NULL && t < 6; t++)
	{
		if (!token) fprintf(stderr, "%d: %s\n", t, token);
		switch(t)
		{
			case 0: f.sindex = token[0]; break;
			case 1: f.dindex = token[0]; break;
			case 2: f.datasize = atol(token); break;
			case 3: f.stime = atof(token); break;
			case 4: f.etime = atof(token); break;
			case 5: f.interval = atof(token); break;
			default: break;
		}
	}
	f.findex = p->findex;
	*p = f;
	return f;
}

flow flowinit(int index)
{
	flow f; f.findex = index; return f;
}
