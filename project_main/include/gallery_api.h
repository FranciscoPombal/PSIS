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

// 20 megabytes, same as imgur
#ifndef IMAGE_SIZE_LIMIT
#define IMAGE_SIZE_LIMIT 20971520
#endif

#ifndef COMMAND_STRING_LENGTH
#define COMMAND_STRING_LENGTH 7
#endif

#ifndef ADD_PHOTO
#define ADD_PHOTO 1
#endif

#ifndef ADD_KEYWORD
#define ADD_KEYWORD 2
#endif

#ifndef SEARCH_PHOTO
#define SEARCH_PHOTO 3
#endif

#ifndef DELETE_PHOTO
#define DELETE_PHOTO 4
#endif

#ifndef GET_PHOTO_NAME
#define GET_PHOTO_NAME 5
#endif

#ifndef GET_PHOTO
#define GET_PHOTO 6
#endif

#ifndef CLOSE_CONNECTION
#define CLOSE_CONNECTION 7
#endif

int gallery_connect(char* host, in_port_t port);

void setupClientAddress(struct sockaddr_in * csa);
void setupGatewayAddress(struct sockaddr_in * gsa, char* gateway_IP, int gateway_port);
void setupPeerAddress(struct sockaddr_in * psa, unsigned int address, int port);

uint32_t gallery_add_photo(int peer_socket, char* file_name);
int gallery_add_keyword(int peer_socket, uint32_t id_photo, char* keyword);
int gallery_search_photo(int peer_socket, char* keyword, uint32_t** id_photos);
int gallery_delete_photo(int peer_socket, uint32_t id_photo);
int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char** photo_name);
int gallery_get_photo(int peer_socket, uint32_t id_photo, char** file_name);

bool gallery_close_connection(int peer_socket);

#endif
