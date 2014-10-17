#include "nesox.h"

static timepoint enter, leave;

static int server(int, char*, short, char*);
static int reader(int, char*, short, long);
static int client(int, char*, short, char*);

static int loaddata(char *filename, char **store);

int usage()
{
	fprintf(stderr, "%s\n", "usage: nesox [options] host port [delay]");
	fprintf(stderr, "%s\n", "options:");
	fprintf(stderr, "%s\t%s\n", "  -r role", "role: server or reader (default: reader)");
	fprintf(stderr, "%s\t%s\n", "  -f file", "file: pathname of data file (default: (null))");
	fprintf(stderr, "%s\t%s\n", "  -s size", "size: bytes num to transfer (default: 1M)");
	fprintf(stderr, "%s\t%s\n", "  -g ground", "ground: run in background as a daemon or console (default: console)");
	fprintf(stderr, "%s\t%s\n", "  -d workdir", "workdir: working directory for daemon (default: \"./\")");
	fprintf(stderr, "%s\n", "comment: ");
	fprintf(stderr, "%s\t%s\n", "  host   ", "IP address for communication");
	fprintf(stderr, "%s\t%s\n", "  port   ", "port number for communication");
	fprintf(stderr, "%s\t%s\n", "  delay  ", "num of microseconds before run server/reader ... ");
	fprintf(stderr, "%s\n", "");
	fprintf(stderr, "%s\t%s\n", "notice: ", "!!! host should be IP address only !!!");
	fprintf(stderr, "%s\t%s\n", "contact: ", "liying.hku@gmail.com");
	return 0;
}

int main(int argc, char *argv[])
{
	timepin(&enter);
	if (argc < 3) { usage(); return -1; }

	char optstring[] = "r:f:s:g:d:";
	char optrole[optionargsize] = "reader";
	char optfile[optionargsize] = "(null)";
	char optsize[optionargsize] = "1048576";
	char optgrnd[optionargsize] = "console";
	char optwdir[optionargsize] = "./";
	char arghost[cmdmaxargsize] = "(null)";
	char argport[cmdmaxargsize] = "(null)";
	char argdely[cmdmaxargsize] = "0";

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
		case 'd':
			snprintf(optwdir, sizeof(optwdir), "%s", optarg);
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

	if (argc == 3)
	snprintf(argdely, sizeof(argdely), "%s", argv[2]);

	fprintf(stderr, "role: %s\n", optrole);
	fprintf(stderr, "file: %s\n", optfile);
	fprintf(stderr, "size: %s\n", optsize);
	fprintf(stderr, "grnd: %s\n", optgrnd);
	fprintf(stderr, "wdir: %s\n", optwdir);

	fprintf(stderr, "host: %s\n", arghost);
	fprintf(stderr, "port: %s\n", argport);
	fprintf(stderr, "dely: %s\n", argdely);

	int background = strcmp(optgrnd, "console");
	if (background) {
		char logfilename[maxbuffersize] = "";
		snprintf(logfilename, sizeof(logfilename), "nesox-%d.log", (int)getpid());
		daemoninit(optwdir, CLOSEFD);
		logopen(logfilename);
	}
	else
		logdup(2);

	unsigned delay = (unsigned)atoi(argdely);
	microsleep(delay);

	if (!strcmp(optrole, "server")) { server(background, arghost, (short)atoi(argport), optfile); }
	if (!strcmp(optrole, "reader")) { reader(background, arghost, (short)atoi(argport), atol(optsize)); }
	if (!strcmp(optrole, "client")) { client(background, arghost, (short)atoi(argport), optfile); }

	timepin(&leave);
	fprintf(stderr, "time: %.8f second(s)\n", timeint(enter, leave));
	return 0;
}

int loaddata(char *filename, char **store)
{
	int result = 0;

	struct stat filestatus;
	result = stat(filename, &filestatus);
	if (result < 0) { logerror("stat failed: %s", strerror(errno)); return -1; }
	long filesize = filestatus.st_size;
	logtrace("file size: %ld", filesize);

	long datasize = filesize - filesize % megabytes;
	logtrace("data size: %ld bytes = %d megabytes", datasize, datasize / megabytes);

	if (datasize > datastoresize) { datasize = datastoresize; logtrace("datasize shrink to storesize!"); }

	char *datastore = (char *)malloc(datasize * sizeof(char));
	if (datastore == NULL) { logerror("malloc failed: %s", strerror(errno)); return -1; }
	logtrace("datastore size: %u", datasize);

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

	*store = datastore;
	return datasize;
}

int server(int background, char *host, short port, char *filename)
{
	logtrace("server run in %s!", background ? "background" : "frontend");
	logtrace("host: %s", host);
	logtrace("port: %d", port);
	logtrace("file: %s", filename);

	int result = 0;

	char *datastore = NULL;
	long datasize = loaddata(filename, &datastore);
	if (datasize < 0) { logerror("loaddata failed: %s", strerror(errno)); return -1; }
	logtrace("load data successed!");

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
		timepoint s; timepin(&s);


		// server task 1: get message
		message m;
		result = getmessage(connectedfd, &m);
		if (result < 0) { logerror("getmessage failed!"); return -1; }
		logtrace("got a message: %s", encode(&m));

		// server task 2: parse message
		handler handlefunc = parsemessage(&m);
		if (!handlefunc) { logerror("parsemessage failed!"); return -1; }
		logtrace("message parsed!");

		// server task 3: handle message
		/**
		 * For getting a general message handling mechanism, 'datastore' is passsed to each handler!
		 *  1. Each handler should allocate its own local buffer to 'getdata'!
		 *  2. 'm.size' is the 'size' info from message, its meaning depends on its context
		 */
		if (!(m.type - LOADDATA)) {
			free(datastore);
			filename = (char *) malloc(maxbuffersize * sizeof(char));
			result = getdata(connectedfd, filename, m.size);
			if(result < 0) { logerror("get file name failed!"); return -1; }
			logtrace("data file name: %s", filename);
			datasize = loaddata(filename, &datastore);
			if (datasize < 0) { logerror("loaddata failed: %s", strerror(errno)); return -1; }
			logtrace("load data successed!");
			free(filename);
			message dataloadmessage = messageinit(LOADDATA, datasize);
			result = putmessage(connectedfd, &dataloadmessage);
			if (result < 0) { logerror("putmessage failed!"); return -1; }
			logtrace("sent back loaded file size: %d", datasize);
		}
		else if (!(m.type - RETRIEVE)) {
			timepoint stransfer; timepin(&stransfer);

			long numtransfer = transfer(connectedfd, datastore, datasize, m.size);
			if (numtransfer < 0) { logerror("handle retrieve (transfer) failed!"); return -1; }
			logtrace("transfer data size: %ld", numtransfer);

			timepoint etransfer; timepin(&etransfer);
			logstats("data transfer time cost: %0.8f seconds", timeint(stransfer, etransfer));
		}
		else
		{
			result = (*handlefunc)(connectedfd, datastore, m.size);
			if (result < 0) { logerror("message handler failed!"); return -1; }
			logtrace("message handled!");
		}

		timepoint e; timepin(&e);
		logstats("connection processing time: %.8f", timeint(s,e));
		close(connectedfd);
		logtrace("closed connection: %d", counter);
	} //foreach acceptable connection!


	close(listeningfd);
	free(datastore);
	logtrace("server return!");
	return 0;
}

int reader(int background, char *host, short port, long requestsize)
{
	logtrace("reader run in %s!", background ? "background" : "frontend");
	logtrace("host: %s", host);
	logtrace("port: %d", port);
	logtrace("size: %ld", requestsize);

	int result = 0;

	long storesize = ((unsigned)requestsize > gigabytes) ? gigabytes : requestsize;
	char *datastore = (char *)malloc(storesize * sizeof(char));
	if (datastore == NULL) { logerror("malloc failed: %s", strerror(errno)); return -1; }
	logtrace("malloc datastore size: %ld", storesize);

	struct sockaddr_in serveraddress;
	int socketfd = socker(host, port, &serveraddress);
	if (socketfd < 0) { logerror("socket/socker failed: %s", strerror(errno)); return -1; }
	logtrace("socket/socker successed!");

	result = connect(socketfd, (struct sockaddr *)(&serveraddress), (socklen_t)(sizeof(serveraddress)));
	if (result < 0) { logerror("connect failed: %s", strerror(errno)); return -1; }
	logtrace("connected!");



	message m = messageinit(RETRIEVE, requestsize);
	putmessage(socketfd, &m);
	logtrace("request sent: %s", encode(&m));



	timepoint sgetdata; timepin(&sgetdata);

	long numretrieve = retrieve(socketfd, datastore, storesize, requestsize);
	if (numretrieve < 0) { logerror("retrieve failed!"); return -1; }
	logtrace("retrieve data size: %ld", numretrieve);

	timepoint egetdata; timepin(&egetdata);
	logstats("data retrieve time cost: %0.8f seconds", timeint(sgetdata,egetdata));



	close(socketfd);
	free(datastore);
	logtrace("reader return");
	return 0;
}

int client(int background, char *host, short port, char *filename)
{
	logtrace("client run in %s!", background ? "background" : "frontend");
	logtrace("file: %s", filename);

	long result = 0;

	struct sockaddr_in serveraddress;
	int socketfd = socker(host, port, &serveraddress);
	if (socketfd < 0) { logerror("socket/socker failed: %s", strerror(errno)); return -1; }
	logtrace("socket/socker successed!");

	result = connect(socketfd, (struct sockaddr *)(&serveraddress), (socklen_t)(sizeof(serveraddress)));
	if (result < 0) { logerror("connect failed: %s", strerror(errno)); return -1; }
	logtrace("connected!");

	long filenamesize = strlen(filename);
	message dataloadmessage = messageinit(LOADDATA, filenamesize);
	result = putmessage(socketfd, &dataloadmessage);
	if (result < 0) { logerror("putmessage failed: %s", strerror(errno)); return -1; }

	result = putdata(socketfd, filename, filenamesize);
	if (result < 0) { logerror("putdata failed: %s", strerror(errno)); return -1; }

	message dataloadfeedback;
	result = getmessage(socketfd, &dataloadfeedback);
	if (result < 0) { logerror("getmessage failed: %s", strerror(errno)); return -1; }
	if (!(dataloadfeedback.type - LOADDATA)) logtrace("loaded data size: %ld", dataloadfeedback.size);

	close(socketfd);
	logtrace("client return");
	return 0;
}

