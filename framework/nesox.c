#include "nesox.h"

int main(int argc, char *argv[])
{
	fprintf(stderr, "%s\n", "usage: nesox [options] host port");
	fprintf(stderr, "%s\n", "options:");
	fprintf(stderr, "%s\t%s\n", "  -r role", "role: daemon or reader");
	fprintf(stderr, "%s\t%s\n", "  -f file", "file: pathname of data file");
	fprintf(stderr, "%s\t%s\n", "  -s size", "size: bytes num to transfer");

	return 0;
}

