#ifndef _MESSAGES_H_
#define _MESSAGES_H_

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
#include <pthread.h>

#include "./linked_list.h"

#ifndef NO_FLAGS
#define NO_FLAGS 0
#endif

#ifndef DEFAULT_PROTOCOL
#define DEFAULT_PROTOCOL 0
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

#ifndef HUGE_NUMBER
#define HUGE_NUMBER 100000
#endif

#ifndef PEER_ALIVE_INTERVAL
#define PEER_ALIVE_INTERVAL 10
#endif

#ifndef IPV4_STRING_SIZE
#define IPV4_STRING_SIZE 16
#endif

#ifndef PEER_UNAVAILABLE
#define PEER_UNAVAILABLE -1
#endif

#ifndef PEER_ADDRESS
#define PEER_ADDRESS 0
#endif

#ifndef PEER_AVAILABLE
#define PEER_AVAILABLE 1
#endif

#ifndef BAD_PEER
#define BAD_PEER -101
#endif

#ifndef CLIENT_ADDRESS
#define CLIENT_ADDRESS 1
#endif

#ifndef BAD_CLIENT
#define BAD_CLIENT -201
#endif

#ifndef MESSAGE_TYPE_PEER_PING
#define MESSAGE_TYPE_PEER_PING 101
#endif

#ifndef GALLERY_API_ADD_PHOTO
#define GALLERY_API_ADD_PHOTO 501
#endif

#ifndef GALLERY_API_SEARCH_PHOTO
#define GALLERY_API_SEARCH_PHOTO 502
#endif

#ifndef GALLERY_API_DELETE_PHOTO
#define GALLERY_API_DELETE_PHOTO 503
#endif

#ifndef GALLERY_API_GET_PHOTO_NAME
#define GALLERY_API_GET_PHOTO_NAME 504
#endif

#ifndef GALLERY_API_GET_PHOTO
#define GALLERY_API_GET_PHOTO 505
#endif

#ifndef GALLERY_API_ADD_KEYWORD
#define GALLERY_API_ADD_KEYWORD 506
#endif

#ifndef GALLERY_API_CLOSE_CONNECTION
#define GALLERY_API_CLOSE_CONNECTION 507
#endif

typedef struct _message{
    char buffer[MESSAGE_LEN];
} Message;

typedef struct _message_gw{
    int type;
    unsigned int address;
    int port;
} Message_gw;

typedef struct _message_ping {
    int type;
} Message_ping;

typedef struct _message_api_op_type {
    int type;
} Message_api_op_type;

typedef struct _peer_properties{
    struct sockaddr_in peer_socket_dgram_address;
    struct sockaddr_in peer_socket_stream_address;
    int num_connected_clients;
} PeerProperties;


typedef struct _client_properties{
    struct sockaddr_in client_socket_address;
    struct sockaddr_in connected_peer_socket_address;
} ClientProperties;

#endif
