#include "../include/messages.h"
#include "../include/gatewayAPI.h"

static volatile int keepRunning = 1;

void sigIntHandler(int);

void sigIntHandler(int sig)
{
    fprintf(stdout, "Caught signal %d\n", sig);
    keepRunning = 0;

    return;
}

void* clientthread(void * args){
    Client_thread_args* client_thread_args = args;

    SinglyLinkedList* aux_linked_list_node;

    int client_port = client_thread_args->message_gw.port;
    unsigned int client_ip = client_thread_args->message_gw.address;
    SinglyLinkedList* server_linked_list = client_thread_args->list_head;
    ServerProperties* aux_server_linked_list_item = NULL;

    Message_gw message_gw;

    int ret_val_send_to = 0;

    // choose from linked_list

    for(aux_linked_list_node = server_linked_list; aux_linked_list_node != NULL; aux_linked_list_node = SinglyLinkedList_getNextNode(aux_linked_list_node)){
        aux_server_linked_list_item = (ServerProperties*)SinglyLinkedList_getItem(aux_linked_list_node);
        if(aux_server_linked_list_item->status == AVAILABLE){
            // in this initial implementation, the server is automatically marked as unavailable if it is connected with a single client
            aux_server_linked_list_item->status = UNAVAILABLE;
            message_gw.type = PEER_ADDRESS;
            message_gw.address = aux_server_linked_list_item->server_socket_address.sin_addr.s_addr;
            message_gw.port = ntohs(aux_server_linked_list_item->server_socket_address.sin_port);
            break; // breaks out of mearest while, for or do...while
        }
    }

    ret_val_send_to = sendto(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address));
    if(ret_val_send_to == -1){
        fprintf(stderr, "Failed to send server address to client\n");
        exit(EXIT_FAILURE);
    }

}

//Thread always checking for new client or peer connections (INFINITE LOOP)
void* recvthread(void * args){
  int ret_val_recv = 0;
  int ret_val_pthread_create = 0;

  while(true){
    memset((void*)&client_socket_address, 0, sizeof(client_socket_address));
    ret_val_recv = recv(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS);
    fprintf(stderr, "recdd\n");

    if (message_gw.type == CLIENT_ADDRESS) {
      ret_val_pthread_create = pthread_create( &thread_recv_id, NULL, &clientthread, (void *)&client_thread_args);
      if (ret_val_pthread_create != 0) {
        fprintf(stderr, "recv_pthread_create error!\n");
        exit(EXIT_FAILURE);
      }
    }
    else if(message_gw.type == PEER_ADDRESS){
      ret_val_pthread_create = pthread_create( &thread_recv_id, NULL, &clientthread, (void *)&client_thread_args);
      if (ret_val_pthread_create != 0) {
        fprintf(stderr, "recv_pthread_create error!\n");
        exit(EXIT_FAILURE);
      }
    }

  }
}

//Thread that sends peer address to new client
void* send_address_to_client(void * args){

}

//Thread that receives address from new peer
void* receive_address_from_peer(void * args){

}

int main(void)
{
    int socket_dgram_clients_fd = 0;
    int socket_dgram_peers_fd = 0;
    struct sockaddr_in gateway_clients_dgram_socket_address;  // address of the gateway socket
    struct sockaddr_in gateway_peers_dgram_socket_address;  // address of the gateway socket
    // Linked list of servers
    SinglyLinkedList* server_linked_list = NULL;
    SinglyLinkedList* aux_linked_list_node = NULL;
    ServerProperties* server_properties = NULL;
    ServerProperties* aux_server_linked_list_item = NULL;
    bool new_server = true;
    struct sockaddr_in client_socket_address;
    int gateway_port_clients = 0;
    int gateway_port_peers = 0;
    char* char_buffer = NULL;
    int ret_val_bind = 0;
    int ret_val_recv = 0;
    int ret_val_send_to = 0;
    int ret_val_sscanf = 0;
    pthread_t* threadid = malloc(100000*sizeof(pthread_t));
    pthread_t thread_recv_id = 0;
    int i = 0;
    int ret_val_phtread_create = 0;
    int ret_val_recv_phtread_create = 0;

    Message_gw message_gw;
    Client_thread_args client_thread_args;

    // signal realated variables
    struct sigaction sigint_action;



        char_buffer = malloc(CHAR_BUFFER_SIZE * sizeof(char));

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

        //socket call clients
        socket_dgram_clients_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        if(socket_dgram_clients_fd == -1){
            fprintf(stderr, "Error opening dgram socket.\n");
            exit(EXIT_FAILURE);
        }

        //socket call peers
        socket_dgram_peers_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        if(socket_dgram_peers_fd == -1){
            fprintf(stderr, "Error opening dgram socket.\n");
            exit(EXIT_FAILURE);
        }

        /* get gateway address info and set variables accordingly */
        fprintf(stdout, "Insert gateway port for clients:\n");
        gateway_port_clients = getGatewayPort();
        fprintf(stdout, "Insert gateway port for peers:\n");
        gateway_port_peers = getGatewayPort();

        memset((void*)&gateway_clients_dgram_socket_address, 0, sizeof(gateway_clients_dgram_socket_address));   // first we reset the struct
        gateway_clients_dgram_socket_address.sin_family = AF_INET;
        gateway_clients_dgram_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_clients_dgram_socket_address.sin_port = htons(gateway_port_clients);

        memset((void*)&gateway_peers_dgram_socket_address, 0, sizeof(gateway_peers_dgram_socket_address));   // first we reset the struct
        gateway_peers_dgram_socket_address.sin_family = AF_INET;
        gateway_peers_dgram_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_peers_dgram_socket_address.sin_port = htons(gateway_port_peers);

        // we assume client and server family will always be AF_INET
        client_socket_address.sin_family = AF_INET;

        /* bind datagram socket, since we will be receiving CLIENTS*/
        ret_val_bind = bind(socket_dgram_clients_fd, (struct sockaddr *)&gateway_clients_dgram_socket_address, sizeof(gateway_clients_dgram_socket_address));
        if(ret_val_bind == -1){
            fprintf(stderr, "Error binding socket dgram\n");
            exit(EXIT_FAILURE);
        }
        /* bind datagram socket, since we will be receiving PEERS*/
        ret_val_bind = bind(socket_dgram_peers_fd, (struct sockaddr *)&gateway_peers_dgram_socket_address, sizeof(gateway_peers_dgram_socket_address));
        if(ret_val_bind == -1){
            fprintf(stderr, "Error binding socket dgram\n");
            exit(EXIT_FAILURE);
        }

        //Initialize infinite recv thread
        ret_val_recv_phtread_create = pthread_create( &thread_recv_id, NULL, &clientthread, (void *)&client_thread_args);
        if (ret_val_recv_phtread_create != 0) {
          fprintf(stderr, "recv_pthread_create error!\n");
          exit(EXIT_FAILURE);
        }










        // TODO: while keepRunning
        while(true == keepRunning){
            memset((void*)&client_socket_address, 0, sizeof(client_socket_address));
            ret_val_recv = recv(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS);
            fprintf(stderr, "recdd\n");
            /*
            if(ret_val_recv == -1){
                fprintf(stderr, "Error receiving server address\n");
                exit(EXIT_FAILURE);
            }*/
            if(message_gw.type == PEER_ADDRESS){
                fprintf(stdout, "Received a message of type SERVER\n");

                // TODO: check if server already exists in linked list; if it does, mark it as available
                for(aux_linked_list_node = server_linked_list; aux_linked_list_node != NULL; aux_linked_list_node = SinglyLinkedList_getNextNode(aux_linked_list_node)){
                    aux_server_linked_list_item = (ServerProperties*)SinglyLinkedList_getItem(aux_linked_list_node);

                    if((message_gw.port == ntohs(aux_server_linked_list_item->server_socket_address.sin_port)) && (message_gw.address == aux_server_linked_list_item->server_socket_address.sin_addr.s_addr)){
                        new_server = false;
                        if (aux_server_linked_list_item->status == UNAVAILABLE) {
                            aux_server_linked_list_item->status = AVAILABLE;
                        }else{
                            fprintf(stderr, "Inconsistency in server<->gateway communication.\n");
                        }
                    }

                }

                if(true == new_server){
                    server_properties = malloc(sizeof(ServerProperties));
                    server_properties->server_socket_address.sin_family = AF_INET;
                    server_properties->server_socket_address.sin_addr.s_addr = htonl(message_gw.address);
                    server_properties->server_socket_address.sin_port = htons(message_gw.port);
                    server_properties->status = AVAILABLE;

                    if(server_linked_list == NULL){
                        server_linked_list = SinglyLinkedList_newNode((void*)server_properties);
                    }else{
                        SinglyLinkedList_insertAtEnd(server_linked_list, SinglyLinkedList_newNode((void*)server_properties));
                    }
                }else{
                    new_server = true;
                }

                // DEBUG
                //fprintf(stdout, "gateway ip: %s\ngateway port: %d\n-\nserver ip: %s\nserver socket stream port: %d\n", inet_ntoa(gateway_socket_address.sin_addr), ntohs(gateway_socket_address.sin_port), inet_ntoa(server_socket_address.sin_addr), ntohs(server_socket_address.sin_port));
            }else if(message_gw.type == CLIENT_ADDRESS){

                client_thread_args.message_gw = message_gw;
                client_thread_args.list_head = server_linked_list;

                ret_val_phtread_create = pthread_create( &threadid[i], NULL, &clientthread, (void *)&client_thread_args);
                if (ret_val_phtread_create != 0) {
                  fprintf(stderr, "pthread_create error!\n");
                  exit(EXIT_FAILURE);
                }else{
                  i++;
                }

            }else if(message_gw.type == PEER_UNAVAILABLE){
                // TODO: find server in linked list
                //if it is in list, mark it as unavailable
                //if it isnt, tell it to 'register' first (good practice, security wise)
            }
        }

    pthread_cancel(thread_recv_id);

    // close and free stuff
    close(socket_dgram_fd);
    free(char_buffer);
    free(threadid);

    return 0;
}
