#ifndef _GATEWAY_H_
#define _GATEWAY_H_
#endif

#include "linked_list.h"

#ifndef CHAR_BUFFER_SIZE
#define CHAR_BUFFER_SIZE 1024
#endif

typedef struct _message_gw{
    int type;
    char address[20];
    int port;
} message_gw;
