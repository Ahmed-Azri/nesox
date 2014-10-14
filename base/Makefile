
MAIN = timing_test
MAIN = daemon_test
MAIN = logger_test

main: $(MAIN)

CC = gcc
CFLAGS = -Weverything

logger.o: logger.c logger.h
	$(CC) -c $(CFLAGS) $< -o $@

logger_test: logger_test.c logger.o
	$(CC) $(CFLAGS) $^ -o $@

daemon.o: daemon.c daemon.h
	$(CC) -c $(CFLAGS) $< -o $@

daemon_test: daemon_test.c daemon.o logger.o
	$(CC) $(CFLAGS) $^ -o $@

timing.o: timing.c timing.h
	$(CC) -c $(CFLAGS) $< -o $@

timing_test: timing_test.c timing.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o *_test *.log

test:
	$(MAIN)

