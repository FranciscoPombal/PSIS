#ifndef _PEER_H_
#define _PEER_H_

#include "messages.h"

extern bool keepRunning;

// Peer helper functions
int gatewayConnect(int peerStreamSocket, struct sockaddr_in * gateway_socket_address, int* sock_sync_send, int* sock_sync_recv);
int clientStreamSocketSetup(void);

void sigIntHandler(int);
void setupInterrupt(void);
void getGatewayIPv4(char* gateway_ipv4);
int getGatewayPort(void);
void setupPeerAddress(struct sockaddr_in * psa);
void setupPeerAddressDgram(struct sockaddr_in * psa_dgram);
void setupGatewayAddress(struct sockaddr_in * gsa);

int setupSyncSendSocket(struct sockaddr_in * peer_dgram_sync_send_socket_address);
int setupSyncRecvSocket(struct sockaddr_in * peer_dgram_sync_recv_socket_address);

void writePhotoToDisk(void* photo, long int size, char storage_name[CHAR_BUFFER_SIZE]);
void addPhotoToList(SinglyLinkedList* list_head, PhotoProperties* photo_metadata);

int deletePhotoFromList(uint32_t id, SinglyLinkedList* list_head);

void findPhotoName(SinglyLinkedList* photo_list_head, uint32_t id, int* name_str_len, char** photo_name, char** photo_storage_name);

int retrievePhoto(char* photo_name, long int* file_size, void** file_buffer);

int getPhotoName(SinglyLinkedList* aux_photo_list_node, int* name_str_len, char** photo_names);

SinglyLinkedList* findPhotoById(SinglyLinkedList* photo_list_head, uint32_t id);
void addKeywordtoPhoto(SinglyLinkedList*  photo_list_node, int keyword_str_len, char* keyword);

void findPhotoByKeyword(SinglyLinkedList* photo_list_head, char* keyword, uint32_t** photo_ids, int* num_photos);

// Threads
void* pingerThread(void*);
void* syncRecvThread(void*);
void* clientHandlerThread(void* args);

typedef struct _clientHandlerThreadArgs {
    int socket_fd;
    int peer_socket_address_sync_send_dgram;
    SinglyLinkedList* photo_list_head;
    struct sockaddr_in gateway_socket_dgram_address;
} ClientHandlerThreadArgs;

typedef struct _syncRecvThreadArgs {
    int peer_socket_address_sync_recv_dgram;
    SinglyLinkedList* photo_list_head;
} SyncRecvThreadArgs;

#endif
