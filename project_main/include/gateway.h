#ifndef _GATEWAY_H_
#define _GATEWAY_H_

#include "messages.h"

static volatile bool keepRunning = true;

int peersDgramSocketSetup(void);
void setupGatewayAddress(struct sockaddr_in *);
int clientDgramSocketSetup(void);

int getGatewayPort(void);
void sigIntHandler(int sig);
void setupInterrupt(void);


#endif
