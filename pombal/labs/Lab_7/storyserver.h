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

#ifndef NO_FLAGS
#define NO_FLAGS 0
#endif

#define MESSAGE_LEN 100
#define BACKLOG_MAX 10
#define BASE_PORT 3000

typedef struct _message{
    char buffer[MESSAGE_LEN];
} message;
