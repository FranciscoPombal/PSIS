#include "../include/messages.h"

//Simple functions
char* getgatewayIP();
int getgatewayport();
int menu();

//Socket functions
// int streamsocket()
// int dgramsocket()


int main() {

  char gateway_IP[50];
  int gateway_port;
  int choice = 0;
  int socket_stream_fd = 0;
  int socket_dgram_fd = 0;
  struct sockaddr_in peer_socket_address;
  struct sockaddr_in client_socket_address;
  struct sockaddr_in gateway_socket_address;
  int ret_val_bind = 0;

  /* socket: create a new communication endpoint */
  // stream socket


  //Get gateway information
  strcpy(gateway_IP, getgatewayIP());
  gateway_port = getgatewayport();
  fprintf(stderr, "\nGateway IP address: %s\nGateway port: %d\n"
                    , gateway_IP, gateway_port);

  socket_stream_fd = gallery_connect(gateway_IP, gateway_port);
  if(socket_stream_fd == 0){
    fprintf(stderr, "Unable to connect to gallery\n");
    exit(EXIT_FAILURE);
  }



  exit(0);

  //Connect to a peer
  //gallery_connect();

  //Print client options
  choice = menu();

  //Execute option
  switch (choice) {
    case 1:
    break;
    case 2:
    break;
    case 3:
    break;
    case 4:
    break;
    case 5:
    break;
  }

  return 0;
}



char* getgatewayIP(){
  int IP[4];
  char* buffer;
  int ret_value = 0;
  char* IP_v4;
  int i = 0;
  bool valid_ip = false;
  int x=0;

  buffer = malloc(CHAR_BUFFER_SIZE*sizeof(char));
  IP_v4 = malloc(16*sizeof(char));

 while(ret_value != 4 || false == valid_ip ){
   printf("Insert Gateway IP address (default: XXX):\n");
   fgets(buffer, 100, stdin);
   ret_value = sscanf(buffer, "%d.%d.%d.%d", &IP[0], &IP[1], &IP[2], &IP[3]);
   if(ret_value != 4){
     fprintf(stderr, "Error getting gateway IP\n\n");
   }

   for (i = 0; i < 4; i++) {
     if(IP[i] < 0 || IP[i] > 255){
       x++;
     }
   }
   if(x == 0){
     valid_ip = true;
   }
   else{
     valid_ip = false;
   }
   if (false == valid_ip) {
     fprintf(stderr, "Invalid gateway IP\n\n");
   }
   x = 0;

 }

  sprintf(IP_v4, "%d.%d.%d.%d", IP[0], IP[1], IP[2], IP[3]);
  free(buffer);
  //Must not forget to free IP_v4 in main()
  return IP_v4;
}

int getgatewayport(){
  char* char_buffer;
  int gateway_port = 0;
  int ret_val_sscanf = 0;

  char_buffer = malloc(CHAR_BUFFER_SIZE*sizeof(char));


  while(gateway_port < 1024 || gateway_port > USHRT_MAX || ret_val_sscanf != 1){
    fprintf(stdout, "Insert gateway port:\n");
    fgets(char_buffer, sizeof(char_buffer), stdin);
    ret_val_sscanf = sscanf(char_buffer, "%d\n", &gateway_port);

    if (gateway_port < 1024 || gateway_port > USHRT_MAX) {
      fprintf(stderr, "Invalid gateway port\n\n");
    }
    else if(ret_val_sscanf != 1){
      fprintf(stderr, "Error reading port\n\n");
    }
  }

  free(char_buffer);
  return gateway_port;
}

void setuppeeraddres(struct sockaddr_in * psa){

  memset((void*)&psa, 0, sizeof(psa));  // reset struct
  psa->sin_family = AF_INET;

  return;
}

int menu(){
  int option = 0;

  printf("\nChoose an option (number):\n");
  printf("1 - \n");
  printf("2 - \n");
  printf("3 - \n");
  printf("4 - \n");
  printf("5 - \n");

  scanf("%d\n", &option);
  return option;
}
