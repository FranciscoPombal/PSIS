# compiler
CC=gcc

# compiler flags
CFLAGS=-Wall\
	   -Wextra\
	   -g\
	   -O0

all: char-conv-prob.c
	$(CC) $(CFLAGS) char-conv-prob.c -o char-conv-prob

.PHONY: clean
clean:
	rm -f char-conv-prob
