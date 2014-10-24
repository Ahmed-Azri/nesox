OS = $(shell uname)

SERVER = nesoxserver
CLIENT = nesoxclient

MAIN = nesoxserver
MAIN = nesoxclient
MAIN = nesox

main: $(MAIN)

s: $(SERVER)
c: $(CLIENT)

HOST = 127.0.0.1
PORT = 8848
FILE = ../data/biblewarpease

SIZE = 1048576
SIZE = 6291456
SIZE = 8388608

SIZE = 1073741824
SIZE = 3221225472
SIZE = 4294967296
SIZE = 2147483648

LEVEL = trace
LEVEL = stats

DELAY = 2

ifeq ($(OS), Darwin)
CC = clang
WARN = -Weverything
else ifeq ($(OS), Linux)
CC = gcc -std=gnu99
WARN = -Wall
endif
NOWARN = -w
AR = ar cru
CFLAGS = $(NOWARN)

BASE_INCLUDE = ../base
BASE_LIBRARY = ../base


protocol.o: protocol.c protocol.h
	$(CC) -c $(CFLAGS) -I $(BASE_INCLUDE) $< -o $@

nesoxserver: nesoxserver.c nesox.h
	$(CC) $(CFLAGS) -I $(BASE_INCLUDE) $< -o $@ -L $(BASE_LIBRARY) -lbase

nesoxclient: nesoxclient.c nesox.h
	$(CC) $(CFLAGS) -I $(BASE_INCLUDE) $< -o $@ -L $(BASE_LIBRARY) -lbase

nesox: nesox.c protocol.o
	$(CC) $(CFLAGS) -I $(BASE_INCLUDE) $^ -o $@ -L $(BASE_LIBRARY) -lbase


.PHONY:	clean test ts tr tc info

clean:
	rm -rf *.o *.exe *.log $(SERVER) $(CLIENT) nesox

test:
	$(MAIN)

ts:
	nesox -l $(LEVEL) -r server -f $(FILE) $(HOST) $(PORT)
tr:
	nesox -l $(LEVEL) -r reader -s $(SIZE) $(HOST) $(PORT)
tc:
	nesox -l $(LEVEL) -r client -f $(FILE) $(HOST) $(PORT)
tb:
	nesox -l $(LEVEL) -r beater -s $(SIZE)  $(HOST) $(PORT) $(DELAY)
hb:
	nesox -d $(NESOX_HOME)/heartbeat -g background -l $(LEVEL) -r beater $(HOST) $(PORT) $(DELAY)

info:
	@echo "OS:" $(OS)
	@echo "CC:" $(CC)
	@echo "Warning all:" $(WARN)
