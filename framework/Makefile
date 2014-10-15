
SERVER = nesoxserver
CLIENT = nesoxclient

MAIN = nesoxserver
MAIN = nesoxclient
MAIN = nesox

main: $(MAIN)

s: $(SERVER)
c: $(CLIENT)

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

.PHONY:	clean test

clean:
	rm -rf *.o *.exe *.log $(SERVER) $(CLIENT) nesox

test:
	$(MAIN)
