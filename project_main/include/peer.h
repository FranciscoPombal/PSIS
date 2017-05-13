#ifndef _PEER_H_
#define _PEER_H_

#include "messages.h"

static volatile bool keepRunning = true;

// Peer helper functions
int gatewayConnect(int peerStreamSocket);
int clientStreamSocketSetup(void);

void sigIntHandler(int);
void setupInterrupt(void);
void getGatewayIPv4(char* gateway_ipv4);
int getGatewayPort(void);
void setupPeerAddress(struct sockaddr_in * psa);
void setupPeerAddressDgram(struct sockaddr_in * psa_dgram);
void setupGatewayAddress(struct sockaddr_in * gsa);

// Threads
void* pingerThread(void*);
void* clientHandlerThread(void* args);

#endif
