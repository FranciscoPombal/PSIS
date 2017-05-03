#include "../include/gateway.h"

/* THREAD: receive peer address and add it to Peers linked list */
void* peerRecvThread(void* args)
{

}

/* THREAD: receive client address and add it to Clients linked list */
void* clientRecvThread(void* args)
{
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
void* recvThread(void* args)
{
    int ret_val_recv = 0;
    int ret_val_pthread_create = 0;
    pthread_t* clients_id = NULL;
    pthread_t* peers_id = NULL;
    unsigned int i=0, j=0;
    Message_gw message_gw;

    clients_id = (pthread_t*)malloc(100000 * sizeof(pthread_t));
    peers_id = (pthread_t*)malloc(100000 * sizeof(pthread_t));

        while(true){
            memset((void*)&client_socket_address, 0, sizeof(client_socket_address));
            ret_val_recv = recv(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS);
            fprintf(stderr, "recdd\n");

            if(message_gw.type == CLIENT_ADDRESS){
                ret_val_pthread_create = pthread_create( &clients_id[i], NULL, &clientthread, (void *)&client_thread_args);
                if (ret_val_pthread_create != 0) {
                    fprintf(stderr, "recv_pthread_create error!\n");
                    exit(EXIT_FAILURE);
                }
                i++;
            }else if(message_gw.type == PEER_ADDRESS){
                ret_val_pthread_create = pthread_create( &peers_id[j], NULL, &clientthread, (void *)&client_thread_args);
                if(ret_val_pthread_create != 0){
                    fprintf(stderr, "recv_pthread_create error!\n");
                    exit(EXIT_FAILURE);
                }
                j++;
            }
        }

    free(clients_id);
    free(peers_id);
}

//Thread that sends peer address to new client
void* send_address_to_client(void * args)
{

}

//Thread that receives address from new peer
void* receive_address_from_peer(void * args)
{

}
