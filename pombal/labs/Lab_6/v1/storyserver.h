#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define SOCKET_NAME "lab_6_socket"
#define MESSAGE_LEN 100

typedef struct message{
    char buffer[MESSAGE_LEN];
} message;
