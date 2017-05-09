#ifndef _GALLERY_API_H_
#define _GALLERY_API_H_
#include "../include/messages.h"

// TODO: check include

#ifndef CLIENT_RECV_FROM_GATEWAY_TIMEOUT
#define CLIENT_RECV_FROM_GATEWAY_TIMEOUT 30
#endif

#ifndef CLIENT_SEND_TO_GATEWAY_TIMEOUT
#define CLIENT_SEND_TO_GATEWAY_TIMEOUT 10
#endif

#ifndef ADD_PHOTO
#define ADD_PHOTO 10
#endif

#ifndef SEARCH_PHOTO
#define SEARCH_PHOTO 11
#endif

#ifndef DELETE_PHOTO
#define DELETE_PHOTO 12
#endif

int gallery_connect(char* host, in_port_t port);

void setupClientAddress(struct sockaddr_in * csa);
void setupGatewayAddress(struct sockaddr_in * gsa, char* gateway_IP, int gateway_port);
void setupPeerAddress(struct sockaddr_in * psa, unsigned int address, int port);

uint32_t gallery_add_photo(int peer_socket, char* file_name);
int gallery_add_keyword(int peer_socket, uint32_t id_photo, char* keyword);
int gallery_search_photo(int peer_socket, char* keyword, uint32_t** id_photos);
int gallery_delete_photos(int peer_socket, uint32_t id_photo);
int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char** photo_name);
int gallery_get_photo(int peer_socket, uint32_t id_photo, char** file_name);

struct client_message{
    int id;
    char filename[256];
    char** keywords;
    FILE* fp;
};

#endif
