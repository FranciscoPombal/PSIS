#include "../include/gateway.h"

// TODO: client linked list nodes must specify which peer is connected to the client, to inform upon (unexpected) peer death
// TODO: peer linked list must include dgram socket address of peer for additional communication

int main(void)
{
    // socket stuff
    int socket_dgram_clients_fd = 0;
    int socket_dgram_peers_fd = 0;

    //linked list stuff
    // Linked list of peers
    SinglyLinkedList* peer_linked_list = NULL;
    SinglyLinkedList* aux_linked_list_node = NULL;
    PeerProperties* peer_properties = NULL;
    PeerProperties* aux_peer_linked_list_item = NULL;

    // Linked list of clients
    SinglyLinkedList* client_linked_list = NULL;

    // thread stuff
    // recv threads
    pthread_t thread_master_peer_recv_id = 0;
    pthread_t thread_master_client_recv_id = 0;
    MasterPeerRecvThreadArgs* masterPeerRecvThreadArgs = NULL;
    MasterClientRecvThreadArgs* masterClientRecvThreadArgs = NULL;

    pthread_t* threadid = malloc(HUGE_NUMBER * sizeof(pthread_t));
    int i = 0;
    int ret_val_phtread_create = 0;
    int ret_val_recv_phtread_create = 0;

    // other
    char* char_buffer = NULL;
    Message_gw message_gw;

    //TODO
    bool new_peer = true;
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

        // prepare arguments for infinite master recv threads
        masterPeerRecvThreadArgs = malloc(sizeof(MasterPeerRecvThreadArgs));
        masterPeerRecvThreadArgs->socket_fd = socket_dgram_peers_fd;
        peer_linked_list = SinglyLinkedList_newNode(NULL);
        masterPeerRecvThreadArgs->peer_list_head = peer_linked_list;

        masterClientRecvThreadArgs = malloc(sizeof(MasterClientRecvThreadArgs));
        masterClientRecvThreadArgs->socket_fd = socket_dgram_clients_fd;
        masterClientRecvThreadArgs->peer_list_head = peer_linked_list;
        client_linked_list = SinglyLinkedList_newNode(NULL);
        masterClientRecvThreadArgs->client_list_head = client_linked_list;

        //Initialize infinite master recv threads
        ret_val_recv_phtread_create = pthread_create(&thread_master_peer_recv_id, NULL, &masterPeerRecvThread, &masterPeerRecvThreadArgs);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create (peer) error!\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv_phtread_create = pthread_create(&thread_master_client_recv_id, NULL, &masterClientRecvThread, masterClientRecvThreadArgs);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create (client) error!\n");
            exit(EXIT_FAILURE);
        }

        // TODO: refactor the rest + threads
        //PREVIOUS CODE (LAB7):

        char_buffer = malloc(CHAR_BUFFER_SIZE * sizeof(char));
        // we assume client and peer family will always be AF_INET
        client_socket_address.sin_family = AF_INET;

        // TODO: while keepRunning
        while(true == keepRunning){
            memset((void*)&client_socket_address, 0, sizeof(client_socket_address));
            ret_val_recv = recv(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS);
            fprintf(stderr, "recdd\n");
            /*
            if(ret_val_recv == -1){
                fprintf(stderr, "Error receiving peer address\n");
                exit(EXIT_FAILURE);
            }*/
            if(message_gw.type == PEER_ADDRESS){
                fprintf(stdout, "Received a message of type PEER\n");

                // TODO: check if peer already exists in linked list; if it does, mark it as available
                for(aux_linked_list_node = peer_linked_list; aux_linked_list_node != NULL; aux_linked_list_node = SinglyLinkedList_getNextNode(aux_linked_list_node)){
                    aux_peer_linked_list_item = (PeerProperties*)SinglyLinkedList_getItem(aux_linked_list_node);

                    if((message_gw.port == ntohs(aux_peer_linked_list_item->peer_socket_address.sin_port)) && (message_gw.address == aux_peer_linked_list_item->peer_socket_address.sin_addr.s_addr)){
                        new_peer = false;
                        if (aux_peer_linked_list_item->status == UNAVAILABLE) {
                            aux_peer_linked_list_item->status = AVAILABLE;
                        }else{
                            fprintf(stderr, "Inconsistency in peer<->gateway communication.\n");
                        }
                    }

                }

                if(true == new_peer){
                    peer_properties = malloc(sizeof(PeerProperties));
                    peer_properties->peer_socket_address.sin_family = AF_INET;
                    peer_properties->peer_socket_address.sin_addr.s_addr = htonl(message_gw.address);
                    peer_properties->peer_socket_address.sin_port = htons(message_gw.port);
                    peer_properties->status = AVAILABLE;

                    if(peer_linked_list == NULL){
                        peer_linked_list = SinglyLinkedList_newNode((void*)peer_properties);
                    }else{
                        SinglyLinkedList_insertAtEnd(peer_linked_list, SinglyLinkedList_newNode((void*)peer_properties));
                    }
                }else{
                    new_peer = true;
                }

                // DEBUG
                //fprintf(stdout, "gateway ip: %s\ngateway port: %d\n-\npeer ip: %s\npeer socket stream port: %d\n", inet_ntoa(gateway_socket_address.sin_addr), ntohs(gateway_socket_address.sin_port), inet_ntoa(peer_socket_address.sin_addr), ntohs(peer_socket_address.sin_port));
            }else if(message_gw.type == CLIENT_ADDRESS){

                client_thread_args.message_gw = message_gw;
                client_thread_args.list_head = peer_linked_list;

                ret_val_phtread_create = pthread_create( &threadid[i], NULL, &clientthread, (void *)&client_thread_args);
                if (ret_val_phtread_create != 0) {
                  fprintf(stderr, "pthread_create error!\n");
                  exit(EXIT_FAILURE);
                }else{
                  i++;
                }

            }else if(message_gw.type == PEER_UNAVAILABLE){
                // TODO: find peer in linked list
                //if it is in list, mark it as unavailable
                //if it isnt, tell it to 'register' first (good practice, security wise)
            }
        }

    // TODO: probably not needed
    pthread_cancel(thread_master_peer_recv_id);
    pthread_cancel(thread_master_client_recv_id);

    // close and free stuff
    close(socket_dgram_peers_fd);
    close(socket_dgram_clients_fd);
    free(char_buffer);
    free(threadid);

    return 0;
}
