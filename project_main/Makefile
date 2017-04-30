# compiler
CC=gcc

# compiler flags for main program (release)
CFLAGS=-Wall \
       -Wextra \
       -pthread \
       -O2 \

# compiler flags for main program (debug)
CFLAGSDBG_1=-Wall \
            -Wextra \
            -pthread \
            -g \
		    -fno-omit-frame-pointer \
		    -fsanitize=address \
		    -fsanitize=leak \
		    -fsanitize=undefined \

CFLAGSDBG_2=-Wall \
		    -Wextra \
		    -pthread \
		    -g \
            -fno-omit-frame-pointer \
            -fsanitize=thread \

CFILES=src/linked_list.c

IFILES=include/defs.h\
       include/linked_list.h\
       include/messages.h\
	   include/gallery_api.h\
       include/client.h

IDIR=include/

all: client gateway server client_dbg gateway_dbg server_dbg

debug: client_dbg gateway_dbg server_dbg

release: client gateway server

client: src/client.c src/client_functions.c src/linked_list.c include/client.h $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/client.c src/client_functions.c src/gallery_api.c -o bin/release/client 2> build/release/client_compile.log

gateway: src/gateway.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/gateway.c -o bin/release/gateway 2> build/release/gateway_compile.log

server: src/server.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/server.c -o bin/release/server 2> build/release/server_compile.log

client_dbg: client_dbg_1 client_dbg_2

client_dbg_1: src/client.c src/client_functions.c src/linked_list.c include/client.h $(IFILES)
	$(CC) $(CFLAGSDBG_1) -I$(IDIR) $(CFILES) src/client.c src/client_functions.c src/gallery_api.c -o bin/debug/client 2> build/debug/client_compile_asan.log

client_dbg_2: src/client.c src/client_functions.c src/linked_list.c include/client.h $(IFILES)
	$(CC) $(CFLAGSDBG_2) -I$(IDIR) $(CFILES) src/client.c src/client_functions.c src/gallery_api.c -o bin/debug/client 2> build/debug/client_compile_tsan.log

gateway_dbg: gateway_dbg_1 gateway_dbg_2

gateway_dbg_1: src/gateway.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG_1) -I$(IDIR) $(CFILES) src/gateway.c -o bin/debug/gateway 2> build/debug/gateway_compile_asan.log

gateway_dbg_2: src/gateway.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG_2) -I$(IDIR) $(CFILES) src/gateway.c -o bin/debug/gateway 2> build/debug/gateway_compile_tsan.log

server_dbg: server_dbg_1 server_dbg_2

server_dbg_1: src/server.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG_1) -I$(IDIR) $(CFILES) src/server.c -o bin/debug/server 2> build/debug/server_compile_asan.log

server_dbg_2: src/server.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG_2) -I$(IDIR) $(CFILES) src/server.c -o bin/debug/server 2> build/debug/server_compile_tsan.log

.PHONY: clean
clean:
	rm -f \
    bin/release/* \
    bin/debug/* \
    build/release/* \
    build/debug/*
