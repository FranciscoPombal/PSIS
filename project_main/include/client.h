#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "messages.h"
#include "gallery_api.h"

extern bool keepRunning;

// Client helper functions
void sigIntHandler(int);
void setupInterrupt(void);
void getGatewayIPv4(char* gateway_ipv4);
int getGatewayPort(void);
int showMenu(void);

#endif
