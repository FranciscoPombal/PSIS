#include "../include/messages.h"
#include "../include/galleryAPI.h"

int gallery_connect(char* host, in_port_t port){
  struct sockaddr_in client_socket_address;
  int socket_stream_fd = 0;
  int socket_dgram_fd = 0;
  struct sockaddr_in peer_socket_address;
  struct sockaddr_in gateway_socket_address;
  int ret_val_bind = 0;
  int ret_val_send_to = 0;
  int ret_val_recvfrom = 0;
  int ret_val_connect = 0;
  Message_gw message_gw;
  socklen_t socklen = sizeof(gateway_socket_address);

  socket_stream_fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
  if(socket_stream_fd == -1){
      fprintf(stderr, "Error creating stream socket\n");
      exit(EXIT_FAILURE);
  }

  // dgram socket
  socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
  if(socket_dgram_fd == -1){
      fprintf(stderr, "Error creating dgram socket\n");
      exit(EXIT_FAILURE);
  }

  setupclientaddress(&client_socket_address);
  setupgatewayaddress(&gateway_socket_address, host, port);

  /* bind datagram socket, since we will be receiving */
  ret_val_bind = bind(socket_dgram_fd, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address));
  if(ret_val_bind == -1){
      fprintf(stderr, "Error binding socket dgram\n");
      exit(EXIT_FAILURE);
  }

  message_gw.type = CLIENT_ADDRESS;
  message_gw.port = client_socket_address.sin_port;
  message_gw.address = client_socket_address.sin_addr.s_addr;

  ret_val_send_to = sendto(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));
  if(ret_val_send_to == -1){
    return -1;
  }

  ret_val_recvfrom = recvfrom(socket_dgram_fd, &message_gw, sizeof(message_gw), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, &socklen);

  if(message_gw.type == PEER_ADDRESS){
    setuppeeraddress(&peer_socket_address, message_gw.address, message_gw.port);
  }
  else if(message_gw.type == PEER_UNAVAILABLE){
    return 0;
  }
  else{
    fprintf(stderr, "Wrong type of message received from gateway\n");
    exit(EXIT_FAILURE);
  }

  /* Connect socket to socket address (server) */
  /* connect */
  ret_val_connect = connect(socket_stream_fd, (struct sockaddr *)&peer_socket_address, sizeof(peer_socket_address));
  if(ret_val_connect != 0){  // check for error
      fprintf(stderr, "Error connecting socket.\n");
      perror("connect");
      exit(EXIT_FAILURE);
  }

  close(socket_dgram_fd);
  return socket_stream_fd;
}

void setupclientaddress(struct sockaddr_in * csa)
{

  int client_port = 0;

  memset((void*)csa, 0, sizeof(*csa));  // reset struct
  csa->sin_family = AF_INET;
  csa->sin_addr.s_addr = htonl(INADDR_ANY);
  client_port = BASE_PORT + getpid();
  if(client_port > USHRT_MAX){
      fprintf(stderr, "Port number too large\n");
      exit(EXIT_FAILURE);
  }
  csa->sin_port = htons(client_port);

  return;
}
void setupgatewayadress(struct sockaddr_in * gsa, char* gateway_IP, int gateway_port)
{

  memset((void*)gsa, 0, sizeof(*gsa));   // first we reset the struct
  gsa->sin_family = AF_INET;
  inet_aton(gateway_IP, &(gsa->sin_addr));
  gsa->sin_port = htons(gateway_port);

  return;
}
void setuppeeraddress(struct sockaddr_in * psa, unsigned int address, int port)
{
  memset((void*)psa, 0, sizeof(*psa));  // reset struct
  psa->sin_family = AF_INET;
  psa->sin_addr.s_addr = htonl(address);
  psa->sin_port = htons(port);

  return;
}


uint32_t gallery_add_photo(int peer_socket, char* file_name){

}

int gallery_add_keyword(int peer_socket, uint32_t id_photo, char* keyword){

}

int gallery_search_photo(int peer_socket, char* keyword, uint32_t** id_photos){

}

int gallery_delete_photos(int peer_socket, uint32_t id_photo){

}

int gallery_get_photo_name(int peer_socket, uint32_t id_photo, char** photo_name){

}

int gallery_get_photo(int peer_socket, uint32_t id_photo, char** file_name){

}
