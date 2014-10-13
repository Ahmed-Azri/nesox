
SERVER = nesoxserver
CLIENT = nesoxclient
DATALD = dataloader
TIMING = nesoxtime

MAIN = nesoxserver nesoxclient
MAIN = dataloader
MAIN = nesoxtime

main: $(MAIN)

s: $(SERVER)
c: $(CLIENT)
d: $(DATALD)

all: $(objects) $(MAIN)

CC = clang
CC = gcc
CFLAGS = -Weverything
CFLAGS = -Wall

nesoxserver: nesoxserver.c nesox.h
	$(CC) $(CFLAGS) $< -o $@

nesoxclient: nesoxclient.c nesox.h
	$(CC) $(CFLAGS) $< -o $@

nesoxtime: nesoxtime.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY:	clean test

clean:
	rm -rf *.o *.exe $(SERVER) $(CLIENT) $(DATALD) $(TIMING)
test:
	$(MAIN)

