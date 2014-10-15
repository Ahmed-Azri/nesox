#include "nesox.h"

static timepoint enter, leave;

int usage()
{
	fprintf(stderr, "%s\n", "usage: nesox [options] host port");
	fprintf(stderr, "%s\n", "options:");
	fprintf(stderr, "%s\t%s\n", "  -r role", "role: server or reader (default: reader)");
	fprintf(stderr, "%s\t%s\n", "  -f file", "file: pathname of data file (default: (null))");
	fprintf(stderr, "%s\t%s\n", "  -s size", "size: bytes num to transfer (default: 1M)");
	fprintf(stderr, "%s\t%s\n", "comment: ", "(host, port) represents endpoint of communication");
	fprintf(stderr, "%s\t%s\n", "notice:  ", "!!! host should be IP address only !!!");
	fprintf(stderr, "%s\t%s\n", "contact: ", "liying.hku@gmail.com");
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc < 3) { usage(); return -1; }
	timepin(&enter);

	char optstring[] = "r:f:s:";
	char optrole[optionargsize] = "reader";
	char optfile[optionargsize] = "(null)";
	char optsize[optionargsize] = "1048576";

	int optc;
	while ((optc = getopt(argc, argv, optstring)) != -1) {
		fprintf(stderr, "optc: %c\n", optc);
		switch (optc) {
		case 'r':
			snprintf(optrole, sizeof(optrole), "%s", optarg);
			break;
		case 'f':
			snprintf(optfile, sizeof(optfile), "%s", optarg);
			break;
		case 's':
			snprintf(optsize, sizeof(optsize), "%s", optarg);
			break;
		case '?':
		default :
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	fprintf(stderr, "optind: %d\n", optind);

	fprintf(stderr, "role: %s\n", optrole);
	fprintf(stderr, "file: %s\n", optfile);
	fprintf(stderr, "size: %s\n", optsize);

	if (argc > 0) fprintf(stderr, "host: %s\n", argv[0]);
	if (argc > 1) fprintf(stderr, "port: %s\n", argv[1]);


	timepin(&leave);
	fprintf(stderr, "time: %.8f seconds\n", timeint(enter, leave));
	return 0;
}


