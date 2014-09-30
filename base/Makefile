
MAIN = tcpsocket_server tcpsocket_client
main: $(MAIN)

objects = tcpsocket.o

all: $(objects) $(MAIN)

CC = g++
CFLAGS = -Weverything

tcpsocket.o: tcpsocket.cc tcpsocket.h
	$(CC) $(CFLAGS) -c $< -o $@

tcpsocket_test: tcpsocket_test.cc tcpsocket.o
	$(CC) $(CFLAGS) $? -o $@

tcpsocket_client: tcpsocket_client.cc tcpsocket.o
	$(CC) $(CFLAGS) $? -o $@

tcpsocket_server: tcpsocket_server.cc tcpsocket.o
	$(CC) $(CFLAGS) $? -o $@

.PHONY:	clean test

clean:
	rm -rf *.o *.exe $(MAIN)
test:
	$(MAIN)
