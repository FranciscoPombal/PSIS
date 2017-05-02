#ifndef _PEER_H_
#define _PEER_H_

#include "messages.h"

static volatile bool keepRunning = true;

// Peer helper functions
int gatewayConnect(char* ipv4, int port);
int clientStreamSocketSetup();

void sigIntHandler(int);
void setupInterrupt(void);
void getGatewayIPv4(char* gateway_ipv4);
int getGatewayPort(void);
void setupPeerAddress(struct sockaddr_in * psa);
void setupGatewayAddress(struct sockaddr_in * gsa);

#endif
