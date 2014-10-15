#include "nesox.h"

static timepoint enter, leave;

static int server();
static int reader();

int usage()
{
	fprintf(stderr, "%s\n", "usage: nesox [options] host port");
	fprintf(stderr, "%s\n", "options:");
	fprintf(stderr, "%s\t%s\n", "  -r role", "role: server or reader (default: reader)");
	fprintf(stderr, "%s\t%s\n", "  -f file", "file: pathname of data file (default: (null))");
	fprintf(stderr, "%s\t%s\n", "  -s size", "size: bytes num to transfer (default: 1M)");
	fprintf(stderr, "%s\t%s\n", "  -g ground", "ground: run in background as a daemon or console (default: console)");
	fprintf(stderr, "%s\t%s\n", "comment: ", "(host, port) represents endpoint of communication");
	fprintf(stderr, "%s\t%s\n", "notice:  ", "!!! host should be IP address only !!!");
	fprintf(stderr, "%s\t%s\n", "contact: ", "liying.hku@gmail.com");
	return 0;
}

int main(int argc, char *argv[])
{
	timepin(&enter);
	if (argc < 3) { usage(); return -1; }

	char optstring[] = "r:f:s:g:";
	char optrole[optionargsize] = "reader";
	char optfile[optionargsize] = "(null)";
	char optsize[optionargsize] = "1048576";
	char optgrnd[optionargsize] = "console";
	char arghost[cmdmaxargsize] = "(null)";
	char argport[cmdmaxargsize] = "(null)";

	int optc;
	while ((optc = getopt(argc, argv, optstring)) != -1) {
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
		case 'g':
			snprintf(optgrnd, sizeof(optgrnd), "%s", optarg);
			break;
		case '?':
		default :
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc < 2) { fprintf(stderr, "%s\n", "misssing input: host port"); usage(); return -1; }
	snprintf(arghost, sizeof(arghost), "%s", argv[0]);
	snprintf(argport, sizeof(argport), "%s", argv[1]);

	fprintf(stderr, "role: %s\n", optrole);
	fprintf(stderr, "file: %s\n", optfile);
	fprintf(stderr, "size: %s\n", optsize);
	fprintf(stderr, "grnd: %s\n", optgrnd);

	fprintf(stderr, "host: %s\n", arghost);
	fprintf(stderr, "port: %s\n", argport);

	int background = strcmp(optgrnd, "console");
	if (background) logopen("nesox.log"); else logdup(2);

	if (!strcmp(optrole, "server")) server(background);
	if (!strcmp(optrole, "reader")) reader(background);

	timepin(&leave);
	fprintf(stderr, "time: %.8f second(s)\n", timeint(enter, leave));
	return 0;
}

int server(int background)
{
	fprintf(stderr, "%s\n", "!!!server!!!");
	if (background) fprintf(stderr, "%s\n", "run into background!");
	logtrace("server: run in %s!", background ? "background" : "frontend");
	return 0;
}

int reader(int background)
{
	fprintf(stderr, "%s\n", "!!!reader!!!");
	if (background) fprintf(stderr, "%s\n", "run into background!");
	logtrace("server: run in %s!", background ? "background" : "frontend");
	return 0;
}

