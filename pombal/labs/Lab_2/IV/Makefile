# compiler
CC=gcc

# compiler flags
CFLAGS=-Wall\
       -Wextra\
	   -g\
	   -O0

all:
	$(CC) $(CFLAGS) char-conv-uninit.c -o char-conv-uninit

.PHONY: clean
clean:
	rm -f char-conv-uninit
