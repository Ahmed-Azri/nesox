
MAIN = tcp_socket_test

objects = logging.o tcp_socket.o

all: $(objects) $(MAIN)

CC = g++
CFLAGS = -Weverything

logging.o: logging.cc logging.h
	$(CC) $(CFLAGS) -c $< -o $@

logging_test: logging_test.cc logging.o
	$(CC) $(CFLAGS) $? -o $@

tcp_socket.o: tcp_socket.cc tcp_socket.h
	$(CC) $(CFLAGS) -c $< -o $@

tcp_socket_test: tcp_socket_test.cc tcp_socket.o logging.o
	$(CC) $(CFLAGS) $? -o $@

.PHONY:	clean test

clean:
	rm -rf *.o ../logs/*.log $(MAIN)

test:
	$(MAIN)

