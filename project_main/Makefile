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

IFILES=include/defs.h \
       include/linked_list.h \
       include/messages.h \
	   include/gallery_api.h \
       include/client.h \
	   include/peer.h \
	   include/gateway.h

IDIR=include/

all: client gateway peer client_dbg gateway_dbg peer_dbg

debug: client_dbg gateway_dbg peer_dbg

release: client gateway peer

client: src/client.c src/client_functions.c src/linked_list.c include/client.h $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/client.c src/client_functions.c src/gallery_api.c -o bin/release/client 2> build/release/client_compile.log

gateway: src/gateway.c src/gateway_functions.c src/gateway_threads.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/gateway.c src/gateway_functions.c src/gateway_threads.c -o bin/release/gateway 2> build/release/gateway_compile.log

peer: src/peer.c src/peer_functions.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGS) -I$(IDIR) $(CFILES) src/peer.c src/peer_functions.c -o bin/release/peer 2> build/release/peer_compile.log

client_dbg: client_dbg_1 client_dbg_2

client_dbg_1: src/client.c src/client_functions.c src/linked_list.c include/client.h $(IFILES)
	$(CC) $(CFLAGSDBG_1) -I$(IDIR) $(CFILES) src/client.c src/client_functions.c src/gallery_api.c -o bin/debug/client 2> build/debug/client_compile_asan.log

client_dbg_2: src/client.c src/client_functions.c src/linked_list.c include/client.h $(IFILES)
	$(CC) $(CFLAGSDBG_2) -I$(IDIR) $(CFILES) src/client.c src/client_functions.c src/gallery_api.c -o bin/debug/client 2> build/debug/client_compile_tsan.log

gateway_dbg: gateway_dbg_1 gateway_dbg_2

gateway_dbg_1: src/gateway.c src/gateway_functions.c src/gateway_threads.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG_1) -I$(IDIR) $(CFILES) src/gateway.c src/gateway_functions.c src/gateway_threads.c -o bin/debug/gateway 2> build/debug/gateway_compile_asan.log

gateway_dbg_2: src/gateway.c src/gateway_functions.c src/gateway_threads.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG_2) -I$(IDIR) $(CFILES) src/gateway.c src/gateway_functions.c src/gateway_threads.c -o bin/debug/gateway 2> build/debug/gateway_compile_tsan.log

peer_dbg: peer_dbg_1 peer_dbg_2

peer_dbg_1: src/peer.c src/peer_functions.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG_1) -I$(IDIR) $(CFILES) src/peer.c src/peer_functions.c -o bin/debug/peer 2> build/debug/peer_compile_asan.log

peer_dbg_2: src/peer.c src/peer_functions.c src/linked_list.c $(IFILES)
	$(CC) $(CFLAGSDBG_2) -I$(IDIR) $(CFILES) src/peer.c src/peer_functions.c -o bin/debug/peer 2> build/debug/peer_compile_tsan.log

.PHONY: clean
clean:
	rm -f \
    bin/release/* \
    bin/debug/* \
    build/release/* \
    build/debug/*
