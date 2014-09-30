
SERVER = nesoxserver
CLIENT = nesoxclient

MAIN = nesoxserver nesoxclient
main: $(MAIN)

s: $(SERVER)
c: $(CLIENT)

all: $(objects) $(MAIN)

CC = clang
CFLAGS = -Weverything

nesoxserver: nesoxserver.c
	$(CC) $(CFLAGS) $? -o $@

nesoxclient: nesoxclient.c
	$(CC) $(CFLAGS) $? -o $@

.PHONY:	clean test

clean:
	rm -rf *.o *.exe $(MAIN)
test:
	$(MAIN)


