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

void writePhotoToDisk(void* photo, long int size, char storage_name[CHAR_BUFFER_SIZE]);
void addPhotoToList(SinglyLinkedList* list_head, PhotoProperties* photo_metadata);

int deletePhotoFromList(uint32_t id, SinglyLinkedList* list_head);

void findPhotoName(SinglyLinkedList* photo_list_head, uint32_t id, int* name_str_len, char* photo_name);

// Threads
void* pingerThread(void*);
void* clientHandlerThread(void* args);

typedef struct _clientHandlerThreadArgs {
    int socket_fd;
    SinglyLinkedList* photo_list_head;
} ClientHandlerThreadArgs;

#endif
