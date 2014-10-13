
SERVER = nesoxserver
CLIENT = nesoxclient

MAIN = nesoxserver 
MAIN = nesoxclient

main: $(MAIN)

s: $(SERVER)
c: $(CLIENT)

all: $(objects) $(MAIN)

CC = clang
CC = gcc
CFLAGS = -Weverything
CFLAGS = -Wall

nesoxserver: nesoxserver.c nesox.h
	$(CC) $(CFLAGS) $< -o $@

nesoxclient: nesoxclient.c nesox.h
	$(CC) $(CFLAGS) $< -o $@


.PHONY:	clean test

clean:
	rm -rf *.o *.exe $(SERVER) $(CLIENT) 

test:
	$(MAIN)

