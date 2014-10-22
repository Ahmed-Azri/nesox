OS = $(shell uname)

MAIN = compute

BASE_INCLUDE = ../base
NESOX_INCLUDE = ../framework

ifeq ($(OS), Darwin)
CC = clang
WARN = -Weverything
else ifeq ($(OS), Linux)
CC = gcc -std=gnu99
WARN = -Wall
endif
NOWARN = -w
CFLAGS = $(NOWARN)

compute: compute.c
	$(CC) $(CFLAGS) -I $(NESOX_INCLUDE) -I $(BASE_INCLUDE) $^ -o $@


.PHONY:	clean test

clean:
	rm -rf $(MAIN)

test:
	$(MAIN)
