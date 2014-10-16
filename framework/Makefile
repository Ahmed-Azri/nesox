
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
SIZE = 1024

CC = clang
CC = gcc
CFLAGS = -Weverything
CFLAGS = -Wall

BASE_INCLUDE = ../base
BASE_LIBRARY = ../base

nesoxserver: nesoxserver.c nesox.h
	$(CC) $(CFLAGS) -I $(BASE_INCLUDE) $< -o $@ -L $(BASE_LIBRARY) -lbase

nesoxclient: nesoxclient.c nesox.h
	$(CC) $(CFLAGS) -I $(BASE_INCLUDE) $< -o $@ -L $(BASE_LIBRARY) -lbase

nesox: nesox.c nesox.h
	$(CC) $(CFLAGS) -I $(BASE_INCLUDE) $< -o $@ -L $(BASE_LIBRARY) -lbase


.PHONY:	clean test ts tc

clean:
	rm -rf *.o *.exe *.log $(SERVER) $(CLIENT) nesox

test:
	$(MAIN)
ts:
	nesox -r server -f $(FILE) $(HOST) $(PORT)
tc:
	nesox -r reader -s $(SIZE) $(HOST) $(PORT)
