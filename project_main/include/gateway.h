#ifndef _GATEWAY_H_
#define _GATEWAY_H_

#include "messages.h"

static volatile bool keepRunning = true;

typedef struct _masterPeerRecvThreadArgs {
    int socket_fd;
    SinglyLinkedList* peer_list_head;
} MasterPeerRecvThreadArgs;

typedef struct _peerRecvThreadArgs {
    SinglyLinkedList* peer_list_head;
    struct sockaddr_in peer_address;
} PeerRecvThreadArgs;

typedef struct _masterClientRecvThreadArgs {
    int socket_fd;
    SinglyLinkedList* client_list_head;
    SinglyLinkedList* peer_list_head;
} MasterClientRecvThreadArgs;

typedef struct _clientRecvThreadArgs {
    int socket_fd;
    struct sockaddr_in client_address;
    SinglyLinkedList* client_list_head;
    SinglyLinkedList* peer_list_head;
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
