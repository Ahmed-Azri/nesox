#include "nesox.h"

int main(int argc, char *argv[])
{

	timepoint s;
	timepin(&s);

	fprintf(stderr, "%s\n", "usage: nesox [options] host port");
	fprintf(stderr, "%s\n", "options:");
	fprintf(stderr, "%s\t%s\n", "  -r role", "role: daemon or reader");
	fprintf(stderr, "%s\t%s\n", "  -f file", "file: pathname of data file");
	fprintf(stderr, "%s\t%s\n", "  -s size", "size: bytes num to transfer");

	timepoint e;
	timepin(&e);

	fprintf(stderr, "%.4f\n", timeint(s,e));
	return 0;
}

