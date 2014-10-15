#include "nesox.h"

static timepoint enter, leave;

static int server(int, char*, short, char*);
static int reader(int, char*, short, int);

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
	if (background) {
		char logfilename[maxbuffersize] = "";
		snprintf(logfilename, sizeof(logfilename), "nesox-%d.log", (int)getpid());
		daemoninit(NOCHDIR, CLOSEFD);
		logopen(logfilename);
	}
	else
		logdup(2);

	if (!strcmp(optrole, "server")) { server(background, arghost, (short)atoi(argport), optfile); }
	if (!strcmp(optrole, "reader")) { reader(background, arghost, (short)atoi(argport), atoi(optsize)); }

	timepin(&leave);
	fprintf(stderr, "time: %.8f second(s)\n", timeint(enter, leave));
	return 0;
}

int server(int background, char *host, short port, char *filename)
{
	logtrace("server run in %s!", background ? "background" : "frontend");
	logtrace("host: %s", host);
	logtrace("port: %d", port);
	logtrace("file: %s", filename);

	int result = 0;

	struct stat filestatus;
	result = stat(filename, &filestatus);
	if (result < 0) { logerror("stat failed: %s", strerror(errno)); return -1; }
	long filesize = filestatus.st_size;
	logtrace("file size: %ld", filesize);

	long datasize = filesize - filesize % megabytes;
	logtrace("data size: %ld bytes = %d megabytes", datasize, datasize / megabytes);

	if (datasize > datastoresize) { datasize = datastoresize; logtrace("datasize shrink to storesize!"); }

	char *datastore = (char *)malloc(datastoresize * sizeof(char));
	if (datastore == NULL) { logerror("malloc failed: %s", strerror(errno)); return -1; }
	logtrace("datastore size: %u", datastoresize);

	int fd = open(filename, O_RDONLY, S_IRUSR);
	if (fd < 0) { logerror("open failed: %s", strerror(errno)); return -1; }
	logtrace("open file successed!");
	logtrace("file des: %d", fd);

	int numread = read(fd, datastore, datasize);
	if (numread < 0) { logerror("read failed: %s", strerror(errno)); return -1; }
	logtrace("num read %d", numread);

	result = close(fd);
	if (result < 0) { logerror("close failed: %s", strerror(errno)); return -1; }
	logtrace("file closed!");

	struct sockaddr_in serveraddress, clientaddress;
	socklen_t sockerlength = sizeof(struct sockaddr_in);
	int listeningfd = socker(host, port, &serveraddress);
	if (listeningfd < 0) { logerror("socket/socker failed: %s", strerror(errno)); return -1; }
	logtrace("socket/socker successed!");

	result = bind(listeningfd, (struct sockaddr *)(&serveraddress), (socklen_t)(sizeof(serveraddress)));
	if (result < 0) { logerror("bind failed: %s", strerror(errno)); return -1; }
	logtrace("bind successed!");

	int backlog = 1024;
	result = listen(listeningfd, backlog);
	if (result < 0) { logerror("listen failed: %s", strerror(errno)); return -1; }
	logtrace("start listening!");


	int counter, status = 1;
	for (counter = 1; status; counter++) {
		logtrace("listening ... ");
		int connectedfd = accept(listeningfd, (struct sockaddr *)(&clientaddress), &sockerlength);
		if (connectedfd < 0) { logerror("accept failed: %s", strerror(errno)); return -1; }
		logtrace("accept connection: %d", counter);

		// handle(connectedfd);

		close(connectedfd);
		logtrace("closed connection: %d", counter);
	} //foreach acceptable connection!


	close(listeningfd);
	free(datastore);
	logtrace("server return!");
	return 0;
}

int reader(int background, char *host, short port, int amount)
{
	logtrace("reader run in %s!", background ? "background" : "frontend");
	logtrace("host: %s", host);
	logtrace("port: %d", port);
	logtrace("size: %d", amount);

	int result = 0;

	struct sockaddr_in serveraddress;
	int socketfd = socker(host, port, &serveraddress);
	if (socketfd < 0) { logerror("socket/socker failed: %s", strerror(errno)); return -1; }
	logtrace("socket/socker successed!");

	result = connect(socketfd, (struct sockaddr *)(&serveraddress), (socklen_t)(sizeof(serveraddress)));
	if (result < 0) { logerror("connect failed: %s", strerror(errno)); return -1; }
	logtrace("connected!");



	close(socketfd);
	logtrace("reader return");
	return 0;
}

