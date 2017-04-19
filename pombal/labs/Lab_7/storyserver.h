#ifndef _STORYSERVER_H_
#define _STORYSERVER_H_
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define SOCKET_STREAM_NAME "/tmp/lab_7_socket_stream"
#define SOCKET_DGRAM_NAME "/tmp/lab_7_socket_dgram"
#define MESSAGE_LEN 100
#define BACKLOG_MAX 10
#define BASE_PORT 3000

typedef struct message{
    char buffer[MESSAGE_LEN];
} message;
