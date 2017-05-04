#ifndef _GATEWAY_H_
#define _GATEWAY_H_

#include "messages.h"

static volatile bool keepRunning = true;

typedef struct _recvThreadArgs {
    int socket_fd;
    SinglyLinkedList* list_head;
} RecvThreadArgs;

typedef struct _clientRecvThreadArgs {
    SinglyLinkedList* list_head;
    struct sockaddr_in client_address;
} ClientRecvThreadArgs;

// Functions
int peersDgramSocketSetup(void);
void setupGatewayAddress(struct sockaddr_in *);
int clientDgramSocketSetup(void);
int getGatewayPort(void);
void sigIntHandler(int sig);
void setupInterrupt(void);

// Threads
void* masterPeerRecvThread(void* args);
void* masterClientRecvThread(void* args);
void* slavePeerRecvThread(void* args);
void* slaveClientRecvThread(void* args);

// TODO
void* send_address_to_client(void * args);
void* receive_address_from_peer(void * args);


#endif
