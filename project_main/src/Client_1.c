#include <stdio.h>
#include <string.h>

//Simple functions
int getgatewayIP();
int getport();
int menu();

//Socket functions
// int streamsocket()
// int dgramsocket()


int main() {

  char gateway_IP[100];
  int gateway_port;
  int choice = 0;

  //Get gateway information
  strcpy(gateway_IP, getgatewayIP());
  gateway_port = getport();
  fprintf(stderr, "\nGateway IP address: %s\nGateway port: %d\n"
                    , gateway_IP, gateway_port);

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



int getgatewayIP(){
  char IP[100];

  printf("Insert Gateway IP address (default: XXX):\n");
  gets(IP);
  return *IP;
}

int getport(){
  int port;

  printf("\nInsert gateway port (default: XXX):\n");
  scanf("%d", &port);
  return port;
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
