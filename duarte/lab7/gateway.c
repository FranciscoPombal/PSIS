#include "header.h"

 int main(int argc, char const *argv[]) {

  int socket_dgram_fd = 0;
  int gateway_port = 0;
  struct sockaddr_in gateway_socket_address;
  struct sockaddr_in server_socket_address;
  struct sockaddr_in server_socket_address_temp;
  struct sockaddr_in client_socket_address;
  Message_gw message_gw;

  //socket call
  socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, 0);

  //get gateway port
  printf("\ninsert gateway port:");
  scanf("%d", &gateway_port);
  printf("gateway port number is: %d", gateway_port);

  //bind
  gateway_socket_address.sin_family = AF_INET;
  gateway_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
  gateway_socket_address.sin_port = htons(gateway_port);
  bind(socket_dgram_fd, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));

  while(1){
    recv(socket_dgram_fd, &message_gw, sizeof(Message_gw), 0);




  }

  return 0;
}
