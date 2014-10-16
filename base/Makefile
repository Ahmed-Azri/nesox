
MAIN = timing_test
MAIN = daemon_test
MAIN = socker_test
MAIN = base_test
MAIN = logger_test
MAIN = message_test
MAIN = handler_test

LIBS = libbase.a

main: $(MAIN)
libs: $(LIBS)

AR = ar cru
CC = gcc
CFLAGS = -Weverything
INCLUDE = ./
LIBRARY = ./

logger.o: logger.c logger.h
	$(CC) -c $(CFLAGS) $< -o $@

logger_test: logger_test.c logger.o
	$(CC) $(CFLAGS) $^ -o $@

timing.o: timing.c timing.h
	$(CC) -c $(CFLAGS) $< -o $@

timing_test: timing_test.c timing.o
	$(CC) $(CFLAGS) $^ -o $@

daemon.o: daemon.c daemon.h
	$(CC) -c $(CFLAGS) $< -o $@

daemon_test: daemon_test.c daemon.o logger.o timing.o
	$(CC) $(CFLAGS) $^ -o $@

socker.o: socker.c socker.h
	$(CC) -c $(CFLAGS) $< -o $@

socker_test: socker_test.c socker.o
	$(CC) $(CFLAGS) $^ -o $@

message.o: message.c message.h
	$(CC) -c $(CFLAGS) $< -o $@

message_test: message_test.c message.o
	$(CC) $(CFLAGS) $^ -o $@

handler.o: handler.c handler.h
	$(CC) -c $(CFLAGS) $< -o $@

handler_test: handler_test.c handler.o
	$(CC) $(CFLAGS) $^ -o $@

libbase.a: logger.o daemon.o socker.o timing.o message.o handler.o
	$(AR) $@ $^

base_test: base_test.c
	$(CC) -I $(INCLUDE) -o $@ $< -L $(LIBRARY) -lbase

clean:
	rm -rf *.o *.a *_test *.log *.cache

test:
	$(MAIN)

