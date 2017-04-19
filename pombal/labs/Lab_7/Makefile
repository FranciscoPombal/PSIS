# compiler
CC=gcc

# compiler flags for main program (release)
CFLAGS=-Wall\
	   -Wextra\

# compiler flags for program libraries (release)
#CLIBFLAGS=

# compiler flags for main program (debug)
CFLAGSDBG=-Wall\
		  -Wextra\
		  -g

# compiler flags for program libraries (debug)
#CLIBFLAGSDBG=

all: client gateway server

client: client.c
	$(CC) $(CFLAGS) $@.c -o $@

gateway: gateway.c
	$(CC) $(CFLAGS) $@.c -o $@

server: server.c
	$(CC) $(CFLAGS) $@.c -o $@

client_dbg: client.c
	cp $^ $@.c && $(CC) $(CFLAGSDBG) $@.c -o $@ && rm $@.c

gateway_dbg: gateway.c
	cp $^ $@.c && $(CC) $(CFLAGSDBG) $@.c -o $@ && rm $@.c

server_dbg: server.c
	cp $^ $@.c && $(CC) $(CFLAGSDBG) $@.c -o $@ && rm $@.c

.PHONY: clean
clean:
	rm -f client gateway server client_dbg gateway_dbg server_dbg
