#ifndef _GATEWAY_H_
#define _GATEWAY_H_

#include "messages.h"

static volatile bool keepRunning = true;

// Functions
int peersDgramSocketSetup(void);
void setupGatewayAddress(struct sockaddr_in *);
int clientDgramSocketSetup(void);
int getGatewayPort(void);
void sigIntHandler(int sig);
void setupInterrupt(void);

// Threads
void* recvThread(void* args);
void* peerRecvThread(void* args);
void* clientRecvThread(void* args);

// TODO
void* send_address_to_client(void * args);
void* receive_address_from_peer(void * args);


#endif
