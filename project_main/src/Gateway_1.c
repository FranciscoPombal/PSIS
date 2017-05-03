#include "../include/gateway.h"

// TODO: client linked list nodes must specify which peer is connected to the client, to inform upon (unexpected) peer death
// TODO: peer linked list must include dgram socket address of server for additional communication

int main(void)
{
    // socket stuff
    int socket_dgram_clients_fd = 0;
    int socket_dgram_peers_fd = 0;

    //linked list stuff
    // Linked list of peers
    SinglyLinkedList* server_linked_list = NULL;
    SinglyLinkedList* aux_linked_list_node = NULL;
    ServerProperties* server_properties = NULL;
    ServerProperties* aux_server_linked_list_item = NULL;

    // Linked list of clients
    SinglyLinkedList* client_linked_list = NULL;

    // thread stuff
    pthread_t* threadid = malloc(100000*sizeof(pthread_t));
    pthread_t thread_recv_id = 0;
    int i = 0;
    int ret_val_phtread_create = 0;
    int ret_val_recv_phtread_create = 0;
    Client_thread_args client_thread_args;

    // other
    char* char_buffer = NULL;
    Message_gw message_gw;

    //TODO
    bool new_server = true;
    struct sockaddr_in client_socket_address;
    int ret_val_bind = 0;
    int ret_val_recv = 0;
    int ret_val_send_to = 0;
    int ret_val_sscanf = 0;

        //setup SIGINT
        setupInterrupt();

        // Create dgram sockets to handle clients and peers
        socket_dgram_peers_fd = peersDgramSocketSetup();
        socket_dgram_clients_fd = clientDgramSocketSetup();

        // TODO: refactor the rest + threads

        //Initialize infinite main recv thread
        ret_val_recv_phtread_create = pthread_create(&thread_recv_id, NULL, &recvThread, (void *)&client_thread_args);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create error!\n");
            exit(EXIT_FAILURE);
        }


        //PREVIOUS CODE (LAB7):

        char_buffer = malloc(CHAR_BUFFER_SIZE * sizeof(char));
        // we assume client and server family will always be AF_INET
        client_socket_address.sin_family = AF_INET;

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
