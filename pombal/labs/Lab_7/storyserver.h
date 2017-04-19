#ifndef _STORYSERVER_H_
#define _STORYSERVER_H_
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "linked_list.h"

#ifndef NO_FLAGS
#define NO_FLAGS 0
#endif

#ifndef MESSAGE_LEN
#define MESSAGE_LEN 100
#endif

#ifndef MESSAGE_GW_LEN
#define MESSAGE_GW_LEN 20
#endif

#ifndef BACKLOG_MAX
#define BACKLOG_MAX 10
#endif

#ifndef BASE_PORT
#define BASE_PORT 3000
#endif

#ifndef CHAR_BUFFER_SIZE
#define CHAR_BUFFER_SIZE 1024
#endif

typedef struct _message{
    char buffer[MESSAGE_LEN];
} message;

typedef struct _message_gw{
    int type;
    char address[MESSAGE_GW_LEN];
    int port;
} message_gw;
