# compiler
CC=gcc

# compiler flags
CFLAGS=-Wall\
	   -Wextra\
	   -g\
	   -O0

all: cache-example1 cache-example2

cache-example1: cache-example1.c
	$(CC) $(CFLAGS) cache-example1.c -o cache-example1

cache-example2: cache-example2.c
	$(CC) $(CFLAGS) cache-example2.c -o cache-example2

.PHONY: clean
clean:
	rm -f cache-example1 cache-example2
