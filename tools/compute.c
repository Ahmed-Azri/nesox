#include "nesox.h"

static char *getstime(char *line);
static char *getetime(char *line);
static char *getint(char *line);
static char *getsindex(char *line);
static char *getdindex(char *line);
static char *getdatasize(char *line);

static char *advance(char *line, char *pattern);

int main(int argc, char *argv[])
{
	fprintf(stderr, "%s: Best Wish!\n", argv[0]);

	char *line = NULL;
	size_t capacity = 0;
	ssize_t linelen;

	for (int i = 0; (linelen = getline(&line, &capacity, stdin)) > 0; i++) {
		fprintf(stderr, "%ld/%ld: %s", linelen, capacity, line);
		char *p = strstr(line, "[STATS]");
		if (p) fprintf(stderr, "%s", p);
		char *q = advance(line, "[STATS]");
		if (q) fprintf(stderr, "%s", q);
	}

	return 0;
}

char *advance(char *line, char *pattern)
{
	char *p = strstr(line, pattern);
	if (p) p += strlen(pattern);
	return p;
}
