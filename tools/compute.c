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

int usage(int print)
{
	if (print < 0) return 0;
	puts("usage: compute [output mode] [header]");
	puts("output mode: ");
	puts("  0: one in all (default)");
	puts("  1: statistical data in one line");
	puts("  2: average completion time");
	puts("  3: transfer span");
	puts("header: ");
	puts("  0: no header (default)");
	puts("  1: print header");
	puts("note: ");
	puts("  seperator is \':\'");
	return 0;
}

int main(int argc, char *argv[])
{
	if (!(argc - 2) && !strcmp("help", argv[1])) usage(1);

	int outputmode = 0;
	int headermode = 0;

	if (!(argc - 2)) outputmode = atoi(argv[1]);
	if (!(argc - 3)) headermode = atoi(argv[1]);

	if (!outputmode) fprintf(stderr, "%s: Best Wishes!\n", argv[0]);

	char *line = NULL;
	size_t capacity = 0;
	ssize_t linelen;

	flow flist[maxflowlistsize];
	int flistsize = 0;

	for (int i = 0; (linelen = getline(&line, &capacity, stdin)) > 0; i++) {
		if (debug) fprintf(stderr, "%ld/%ld: %s", linelen, capacity, line);

		char *temp = advance(line, "[STATS]"); if (!temp) continue;
		char *this = advance(temp, "):"); if (!this) continue;

		int flowindex = i + 1;

		flow f = flowinit(flowindex);
		parseflow(this, &f);
		flist[i] = f;

		flistsize = flowindex;
	}

	for (int i = 0; i < flistsize; i++) {
		if ( flist[i].sindex == flist[i].dindex ) continue;
		if (!outputmode) fprintf(stderr, "%02d/%02d-", i, flistsize);
		if (!outputmode) printflow(flist[i]);
	}

	double firststart = DBL_MAX;
	double lastfinish = DBL_MIN;
	double transferspan = 0.0;

	double shortstcompletiontime = DBL_MAX;
	double longestcompletiontime = DBL_MIN;

	double averagecompletiontime = 0.0;
	double totalcompletiontime = 0.0;

	long totaldatasize = 0;
	long mindatasize = LONG_MAX;
	long maxdatasize = LONG_MIN;

	double averagedatasize = 0.0;

	int effectiveflownum = 0;

	for (int i = 0; i < flistsize; i++) {
		if ( flist[i].sindex == flist[i].dindex ) continue;

		firststart = min(firststart, flist[i].stime);
		lastfinish = max(lastfinish, flist[i].etime);

		totalcompletiontime += flist[i].interval;
		shortstcompletiontime = min(shortstcompletiontime, flist[i].interval);
		longestcompletiontime = max(longestcompletiontime, flist[i].interval);

		mindatasize = min(mindatasize, flist[i].datasize);
		maxdatasize = max(maxdatasize, flist[i].datasize);
		totaldatasize += flist[i].datasize;
		effectiveflownum++;
	}
	averagecompletiontime = totalcompletiontime / effectiveflownum;
	averagedatasize = totaldatasize / effectiveflownum;
	transferspan = lastfinish - firststart;


	if (!outputmode) fprintf(stderr, "\n[%0.8f:%0.8f:%0.8f][%ld:%ld:%ld:%0.8f][%0.8f:%0.8f:%0.8f:%0.8f]\n",
		firststart, lastfinish, transferspan,
		mindatasize, maxdatasize, totaldatasize, averagedatasize,
		shortstcompletiontime, longestcompletiontime, totalcompletiontime, averagecompletiontime);

	if (!(outputmode - 1)) fprintf(stderr, "%0.8f:%0.8f:%0.8f%ld:%ld:%ld:%0.8f%0.8f:%0.8f:%0.8f:%0.8f\n",
		firststart, lastfinish, transferspan,
		mindatasize, maxdatasize, totaldatasize, averagedatasize,
		shortstcompletiontime, longestcompletiontime, totalcompletiontime, averagecompletiontime);

	if (!(outputmode - 2)) fprintf(stderr, "%0.8f", averagecompletiontime);
	if (!(outputmode - 3)) fprintf(stderr, "%0.8f", transferspan);


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
