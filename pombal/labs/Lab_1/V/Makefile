# compiler
CC=gcc

# compiler flags for main program (release)
CFLAGS=-Wall\
	   -Wextra\

# compiler flags for program libraries (release)
CLIBFLAGS=-Wall\
		  -Wextra\
		  -shared\
		  -fPIC\

# compiler flags for main program (debug)
CFLAGSDBG=-Wall\
		  -Wextra\
		  -g

# compiler flags for program libraries (debug)
CLIBFLAGSDBG=-Wall\
			 -Wextra\
			 -shared\
			 -fPIC\
			 -g

# makes all: release version and debug version of main program
all: prog2 prog2dbg

# makes main program; requires libs
prog2: libs prog2.c include/test.h
	$(CC) $(CFLAGS) prog2.c -ldl -o bin/release/prog2

# makes all libs; requires each lib
libs: libtest1 libtest2

# makes libtest1
libtest1: lib/libtest1.c include/test.h
	$(CC) $(CLIBFLAGS) lib/test1.c -ldl -o lib/libtest1.so

# makes libtest2
libtest2: lib/libtest2.c include/test.h
	$(CC) $(CLIBFLAGS) lib/test2.c -ldl -o lib/libtest2.so

# -----------------------------------------------------------------------------
# everything below is the same as above but compiled with debug symbols

prog2dbg: libsdbg prog2.c include/test.h
	$(CC) $(CFLAGSDBG) prog2.c -ldl -o bin/debug/prog2

libsdbg: libtest1dbg libtest2dbg

libtest1dbg: lib/libtest1.c include/test.h
	$(CC) $(CLIBFLAGSDBG) lib/test1.c -ldl -o lib/libtest1dbg.so

libtest2dbg: lib/libtest2.c include/test.h
	$(CC) $(CLIBFLAGSDBG) lib/test2.c -ldl -o lib/libtest2dbg.so

# -----------------------------------------------------------------------------
# clean all object/executable files
.PHONY: clean
clean:
	rm -f bin/release/* bin/debug/* lib/*.so
