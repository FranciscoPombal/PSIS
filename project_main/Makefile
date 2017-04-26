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

CFILES=linked_list.c
CFILESDBG=linked_list_dbg.c

all: client gateway server client_dbg gateway_dbg server_dbg

dbg: client_dbg gateway_dbg server_dbg

client: client.c
    $(CC) $(CFLAGS) $(CFILES) $@.c -o $@

gateway: gateway.c
    $(CC) $(CFLAGS) $(CFILES) $@.c -o $@

server: server.c
    $(CC) $(CFLAGS) $(CFILES) $@.c -o $@

client_dbg: client.c
    cp $(CFILES) $(CFILESDBG) && cp $^ $@.c && $(CC) $(CFLAGSDBG) $(CFILESDBG) $@.c -o $@

gateway_dbg: gateway.c
    cp $(CFILES) $(CFILESDBG) && cp $^ $@.c && $(CC) $(CFLAGSDBG) $(CFILESDBG) $@.c -o $@

server_dbg: server.c
    cp $(CFILES) $(CFILESDBG) && cp $^ $@.c && $(CC) $(CFLAGSDBG) $(CFILESDBG) $@.c -o $@

.PHONY: clean
clean:
    rm -f client gateway server client_dbg gateway_dbg server_dbg client_dbg.c gateway_dbg.c server_dbg.c linked_list_dbg.c
