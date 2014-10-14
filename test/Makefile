MAIN = base_test


CC = gcc
CFLAGS = -Weverything
INCLUDE = ../base
LIBRARY = ../base

base_test: base_test.c
	$(CC) -I $(INCLUDE) -o $@ $< -L $(LIBRARY) -lbase

clean:
	rm -rf *.o *_test *.log

test:
	$(MAIN)

