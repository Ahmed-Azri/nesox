
SERVER = nesoxserver
CLIENT = nesoxclient
DATALD = dataloader

MAIN = nesoxserver nesoxclient
MAIN = dataloader

main: $(MAIN)

s: $(SERVER)
c: $(CLIENT)
d: $(DATALD)

all: $(objects) $(MAIN)

CC = clang
CC = gcc
CFLAGS = -Weverything
CFLAGS = -Wall

nesoxserver: nesoxserver.c
	$(CC) $(CFLAGS) $? -o $@

nesoxclient: nesoxclient.c
	$(CC) $(CFLAGS) $? -o $@

.PHONY:	clean test

clean:
	rm -rf *.o *.exe $(SERVER) $(CLIENT) $(DATALD)
test:
	$(MAIN)
