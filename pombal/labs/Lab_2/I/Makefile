# compiler
CC=gcc

# compiler flags
CFLAGS=-Wall\
	   -Wextra\
	   -g\
	   -O0

all: infinite-loop.c
	$(CC) $(CFLAGS) infinite-loop.c -o infinite-loop

.PHONY: clean
clean:
	rm -f infinite-loop
