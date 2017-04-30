#include "../include/messages.h"

int gallery_connect(char* host, in_port_t port);

void setupclientaddress(struct sockaddr_in * csa);
void setupgatewayaddress(struct sockaddr_in * gsa, char* gateway_IP, int gateway_port);
void setuppeeraddress(struct sockaddr_in * psa, unsigned int address, int port);

uint32_t gallery_add_photo(int peer_socket, char* file_name);
int gallery_add_keyword(int peer_socket, uint32_t id_photo, char* keyword);
int gallery_search_photo(int peer_socket, char* keyword, uint32_t** id_photos);
int gallery_delete_photos(int peer_socket, uint32_t id_photo);
int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char** photo_name);
int gallery_get_photo(int peer_socket, uint32_t id_photo, char** file_name);
