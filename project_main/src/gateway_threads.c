#include "../include/gateway.h"

pthread_mutex_t client_list_mutex = PTHREAD_MUTEX_INITIALIZER;

//Thread always checking for new peer connections (INFINITE LOOP)
void* masterPeerRecvThread(void* args)
{
    int ret_val_recv = 0;
    int ret_val_pthread_create = 0;
    pthread_t* clients_id = NULL;
    pthread_t* peers_id = NULL;
    unsigned int i=0, j=0;
    Message_gw message_gw;

        clients_id = (pthread_t*)malloc(HUGE_NUMBER * sizeof(pthread_t));
        peers_id = (pthread_t*)malloc(HUGE_NUMBER * sizeof(pthread_t));

        while(true){
            //memset((void*)&client_socket_address, 0, sizeof(client_socket_address));

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

//Thread always checking for new client connections (INFINITE LOOP)
void* masterClientRecvThread(void* args)
{
    int ret_val_recvfrom = 0;
    int ret_val_pthread_create = 0;
    struct sockaddr_in client_socket_address;
    socklen_t client_socket_address_len = sizeof(client_socket_address);
    Message_gw message_gw;
    RecvThreadArgs* recvThreadArgs = (RecvThreadArgs*)args;
    pthread_t* thread_client_recv_id = NULL;
    long int i = 0;
    ClientRecvThreadArgs* clientRecvThreadArgs = NULL; // TODO (see lines 62-64)

        thread_client_recv_id = (pthread_t*)malloc(HUGE_NUMBER * sizeof(pthread_t));

        while(true){
            memset((void*)&client_socket_address, 0, sizeof(client_socket_address));
            ret_val_recvfrom = recvfrom(recvThreadArgs->socket_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&client_socket_address, &client_socket_address_len);

            if(message_gw.type == CLIENT_ADDRESS){
                clientRecvThreadArgs = malloc(sizeof(ClientRecvThreadArgs));
                clientRecvThreadArgs->list_head = recvThreadArgs->list_head;
                clientRecvThreadArgs->client_address = client_socket_address;
                ret_val_pthread_create = pthread_create(&thread_client_recv_id[i], NULL, &slaveClientRecvThread, (void *)clientRecvThreadArgs);
                if (ret_val_pthread_create != 0) {
                    fprintf(stderr, "recv_pthread_create (client - slave) error!\n");
                    exit(EXIT_FAILURE);
                }
                i++;
            }else{
                fprintf(stdout, "Client receive thread error: bad message type\n");
            }
        }

    free(thread_client_recv_id);
}


// TODO
/* THREAD: receive peer address and add it to Peers linked list */
void* peerRecvThread(void* args)
{

}

// TODO
/* THREAD: receive client address and add it to Clients linked list */
void* clientRecvThread(void* args)
{
    ClientRecvThreadArgs* client_recv_thread_args = NULL;
    SinglyLinkedList* client_list_head = NULL;
    SinglyLinkedList* aux_linked_list_node = NULL;
    SinglyLinkedList* new_linked_list_node = NULL;
    struct sockaddr_in client_socket_address;
    ClientProperties* clientProperties = NULL;

        client_recv_thread_args = (ClientRecvThreadArgs*)args;
        client_list_head = client_recv_thread_args->list_head;
        client_socket_address = client_recv_thread_args->client_address;

        // TODO: put client in list
        // create client list item payload
        clientProperties = malloc(sizeof(ClientProperties));
        memset(&(clientProperties->client_socket_address), 0, sizeof(struct sockaddr_in));
        memset(&(clientProperties->connected_peer_socket_address), 0, sizeof(struct sockaddr_in));
        clientProperties -> client_socket_address = client_socket_address;
        // CRITICAL SECTION START
        pthread_mutex_lock(&client_list_mutex);
        for(aux_linked_list_node = client_list_head; SinglyLinkedList_getNextNode(aux_linked_list_node) != NULL; aux_linked_list_node = SinglyLinkedList_getNextNode(aux_linked_list_node)){} // traverse list to last node
        // If we are at the head, the node will already exist but item is null
        if(SinglyLinkedList_getItem(aux_linked_list_node) == NULL){
            SinglyLinkedList_setItem(aux_linked_list_node, clientProperties);
        }else{
        // otherwise, alloc new node, set item and insert it at the end of the list
            new_linked_list_node = SinglyLinkedList_newNode(NULL);
            SinglyLinkedList_setItem(new_linked_list_node, clientProperties);
            SinglyLinkedList_insertAtEnd(aux_linked_list_node, new_linked_list_node);
        }
        pthread_mutex_unlock(&client_list_mutex);
        // CRITICAL SECTION END

        // TODO: search peer list for available peer to send to client
}

//Thread that sends peer address to new client
void* send_address_to_client(void* args)
{

}

//Thread that receives address from new peer
void* receive_address_from_peer(void * args)
{

}
