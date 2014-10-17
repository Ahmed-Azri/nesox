
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

CC = clang
CC = gcc
CFLAGS = -Weverything
CFLAGS = -Wall
CFLAGS = -w

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


.PHONY:	clean test ts tc

clean:
	rm -rf *.o *.exe *.log $(SERVER) $(CLIENT) nesox

test:
	$(MAIN)

ts:
	nesox -r server -f $(FILE) $(HOST) $(PORT)
tr:
	nesox -r reader -s $(SIZE) $(HOST) $(PORT)
tc:
	nesox -r client -f $(FILE) $(HOST) $(PORT)

build: nesox
	cp nesox ../bin/nesox