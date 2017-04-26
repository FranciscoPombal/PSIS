# compiler
CC=gcc

# compiler flags for main program (release)
CFLAGS=-Wall\
       -Wextra\
       -pthread\
       -O2

# compiler flags for main program (debug)
CFLAGSDBG=-Wall\
          -Wextra\
          -pthread\
          -g

CFILES=src/linked_list.c

IFILES=include/defs.h\
       include/linked_list.h\
       include/messages.h

IDIR=include/

all: client gateway server client_dbg gateway_dbg server_dbg

debug: client_dbg gateway_dbg server_dbg

release: client gateway server

client: src/client.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/client.c -o bin/release/client

gateway: src/gateway.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/gateway.c -o bin/release/gateway

server: src/server.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/server.c -o bin/release/server

client_dbg: src/client.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG) -I$(IDIR) $(CFILES) src/client.c -o bin/debug/client

gateway_dbg: src/gateway.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG) -I$(IDIR) $(CFILES) src/gateway.c -o bin/debug/gateway

server_dbg: src/server.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG) -I$(IDIR) $(CFILES) src/server.c -o bin/debug/server

.PHONY: clean
clean:
	rm -f \
    bin/release/client \
    bin/release/gateway \
    bin/release/server \
    bin/debug/client \
    bin/debug/gateway \
    bin/debug/server
