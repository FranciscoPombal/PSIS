#include "../include/gateway.h"

pthread_mutex_t client_list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t peer_list_mutex = PTHREAD_MUTEX_INITIALIZER;

/* THREAD: receive peer address and add it to Peers linked list */
void* peerRecvThread(void* args)
{
    bool new_peer = true;
    SinglyLinkedList* peer_list_head = NULL;
    SinglyLinkedList* aux_peer_list_node = NULL;
    SinglyLinkedList* new_peer_list_node = NULL;
    PeerRecvThreadArgs* peerRecvThreadArgs = NULL;
    struct sockaddr_in peer_socket_dgram_address;
    struct sockaddr_in peer_socket_stream_address;
    PeerProperties* aux_peer_list_item = NULL;

        peerRecvThreadArgs = (PeerRecvThreadArgs*)args;
        peer_list_head = peerRecvThreadArgs->peer_list_head;
        peer_socket_dgram_address = peerRecvThreadArgs->peer_socket_dgram_address;
        peer_socket_stream_address = peerRecvThreadArgs->peer_socket_stream_address;

        // CRITICAL SECTION BEGIN
        pthread_mutex_lock(&peer_list_mutex);
        for(aux_peer_list_node = peer_list_head; SinglyLinkedList_getNextNode(aux_peer_list_node) != NULL; aux_peer_list_node = SinglyLinkedList_getNextNode(aux_peer_list_node)){
            if(SinglyLinkedList_getItem(aux_peer_list_node) != NULL){
                aux_peer_list_item = (PeerProperties*)SinglyLinkedList_getItem(aux_peer_list_node);
                if((aux_peer_list_item->peer_socket_stream_address.sin_port ==  peer_socket_stream_address.sin_port) && (aux_peer_list_item->peer_socket_stream_address.sin_addr.s_addr ==  peer_socket_stream_address.sin_addr.s_addr)){
                    new_peer = false;
                    break;
                }
            }
        }
        if(true == new_peer){
            fprintf(stdout, "Peer is new, adding it to the list.\n");
            aux_peer_list_item = (PeerProperties*)malloc(sizeof(PeerProperties));
            aux_peer_list_item->num_connected_clients = 0;
            aux_peer_list_item->peer_socket_dgram_address = peer_socket_dgram_address;
            aux_peer_list_item->peer_socket_stream_address = peer_socket_stream_address;
            if(SinglyLinkedList_getItem(aux_peer_list_node) == NULL){
                SinglyLinkedList_setItem(aux_peer_list_node, aux_peer_list_item);
            }else{
                new_peer_list_node = SinglyLinkedList_newNode(aux_peer_list_item);
                SinglyLinkedList_insertAtEnd(aux_peer_list_node, new_peer_list_node);
            }
        }
        pthread_mutex_unlock(&peer_list_mutex);
        // CRITICAL SECTION END

    pthread_exit(NULL);
}

//Thread always checking for new peer connections (INFINITE LOOP)
void* masterPeerRecvThread(void* args)
{
    long int i = 0;
    int socket_fd = 0;
    int ret_val_recvfrom = 0;
    int ret_val_send_to = 0;
    int ret_val_pthread_create = 0;
    pthread_t* thread_peer_recv_id = NULL;
    struct sockaddr_in peer_socket_dgram_address;
    struct sockaddr_in peer_socket_stream_address;
    socklen_t peer_socket_dgram_address_len = sizeof(peer_socket_dgram_address);
    SinglyLinkedList* peer_list_head = NULL;
    Message_gw message_gw;
    MasterPeerRecvThreadArgs* masterPeerRecvThreadArgs = NULL;
    PeerRecvThreadArgs* peerRecvThreadArgs = NULL;

        // TODO: big enough? when to increase? how to increase?
        thread_peer_recv_id = (pthread_t*)malloc(HUGE_NUMBER * sizeof(pthread_t));
        masterPeerRecvThreadArgs = (MasterPeerRecvThreadArgs*)args;
        socket_fd = masterPeerRecvThreadArgs->socket_fd;
        peer_list_head = masterPeerRecvThreadArgs->peer_list_head;

        while(true){
            memset((void*)&peer_socket_dgram_address, 0, sizeof(peer_socket_dgram_address));
            memset((void*)&peer_socket_stream_address, 0, sizeof(peer_socket_stream_address));
            ret_val_recvfrom = recvfrom(socket_fd, &message_gw, sizeof(message_gw), NO_FLAGS, (struct sockaddr *)&peer_socket_dgram_address, &peer_socket_dgram_address_len);

            if(message_gw.type == PEER_ADDRESS){
                fprintf(stdout, "Received peer\n");
                peer_socket_stream_address.sin_family = AF_INET;
                peer_socket_stream_address.sin_addr.s_addr = htonl(message_gw.address);
                peer_socket_stream_address.sin_port = htons(message_gw.port);

                peerRecvThreadArgs = (PeerRecvThreadArgs*)malloc(sizeof(PeerRecvThreadArgs));
                peerRecvThreadArgs->peer_list_head = peer_list_head;
                peerRecvThreadArgs->peer_socket_dgram_address = peer_socket_dgram_address;
                peerRecvThreadArgs->peer_socket_stream_address = peer_socket_stream_address;

                ret_val_pthread_create = pthread_create(&thread_peer_recv_id[i], NULL, &peerRecvThread, (void *)peerRecvThreadArgs);
                if(ret_val_pthread_create != 0){
                    fprintf(stderr, "recv_pthread_create (peer - slave) error!\n");
                    exit(EXIT_FAILURE);
                }
                i += 1;
            }else{
                fprintf(stdout, "Peer receive thread error: bad message type\n");
                message_gw.type = BAD_PEER;
                ret_val_send_to = sendto(socket_fd, &message_gw, sizeof(message_gw), NO_FLAGS, (struct sockaddr *)&peer_socket_dgram_address, peer_socket_dgram_address_len);
            }
        }

    free(thread_peer_recv_id);
}

/* THREAD: receive client address and add it to Clients linked list */
void* clientRecvThread(void* args)
{
    int socket_fd = 0;
    int ret_val_send_to = 0;
    ClientRecvThreadArgs* client_recv_thread_args = NULL;
    SinglyLinkedList* peer_list_head = NULL;
    SinglyLinkedList* aux_peer_list_node = NULL;
    SinglyLinkedList* client_list_head = NULL;
    SinglyLinkedList* aux_client_list_node = NULL;
    SinglyLinkedList* new_client_list_node = NULL;
    struct sockaddr_in client_socket_address;
    struct sockaddr_in peer_socket_address;
    ClientProperties* clientProperties = NULL;
    Message_gw message_gw;
    bool knownClient = false;
    int low = INT_MAX;
    SinglyLinkedList* low_list_node = NULL;
    bool peerAvailable = false;

        message_gw.type = PEER_UNAVAILABLE;
        client_recv_thread_args = (ClientRecvThreadArgs*)args;
        socket_fd = client_recv_thread_args->socket_fd;
        peer_list_head = client_recv_thread_args->peer_list_head;
        client_list_head = client_recv_thread_args->client_list_head;
        client_socket_address = client_recv_thread_args->client_address;

        // CRITICAL SECTION START
        pthread_mutex_lock(&client_list_mutex);

        for(aux_client_list_node = client_list_head; SinglyLinkedList_getNextNode(aux_client_list_node) != NULL; aux_client_list_node = SinglyLinkedList_getNextNode(aux_client_list_node)){
            if(SinglyLinkedList_getItem(aux_client_list_node) != NULL){
                if((((ClientProperties*)SinglyLinkedList_getItem(aux_client_list_node))->client_socket_address.sin_addr.s_addr == client_socket_address.sin_addr.s_addr) && (((ClientProperties*)SinglyLinkedList_getItem(aux_client_list_node))->client_socket_address.sin_port == client_socket_address.sin_port)){
                    knownClient = true;
                    break;
                    // at this point, aux_client_list_node points to the node with the new client
                }
            }
        }

        if(false == knownClient){
            fprintf(stdout, "Client is new, adding it to the list\n"); // DEBUG
            // create client list item payload
            clientProperties = (ClientProperties*)malloc(sizeof(ClientProperties));
            memset(&(clientProperties->client_socket_address), 0, sizeof(struct sockaddr_in));
            memset(&(clientProperties->connected_peer_socket_address), 0, sizeof(struct sockaddr_in));
            clientProperties -> client_socket_address = client_socket_address;
            // traverse list to last node
            for(aux_client_list_node = client_list_head; SinglyLinkedList_getNextNode(aux_client_list_node) != NULL; aux_client_list_node = SinglyLinkedList_getNextNode(aux_client_list_node)){}
            if(SinglyLinkedList_getItem(aux_client_list_node) == NULL){ // If we are at the head, the node will already exist but item is null
                SinglyLinkedList_setItem(aux_client_list_node, clientProperties);
            }else{ // otherwise, alloc new node, set item and insert it at the end of the list
                new_client_list_node = SinglyLinkedList_newNode(NULL);
                SinglyLinkedList_setItem(new_client_list_node, clientProperties);
                SinglyLinkedList_insertAtEnd(aux_client_list_node, new_client_list_node);
                aux_client_list_node = new_client_list_node;
            }
            // at this point, aux_client_list_node points to the node with the new client
        }
        pthread_mutex_unlock(&client_list_mutex);
        // CRITICAL SECTION END

        // search peer list for available peer to send to client (round robin policy)
        // CRITICAL SECTION START
        pthread_mutex_lock(&peer_list_mutex);
        low_list_node = peer_list_head;
        if(SinglyLinkedList_getItem(peer_list_head) != NULL){
            low = ((PeerProperties*)SinglyLinkedList_getItem(peer_list_head))->num_connected_clients;
            peerAvailable = true;
        }
        for(aux_peer_list_node = peer_list_head; SinglyLinkedList_getNextNode(aux_peer_list_node) != NULL; aux_peer_list_node = SinglyLinkedList_getNextNode(aux_peer_list_node)){
            if(((PeerProperties*)SinglyLinkedList_getItem(aux_peer_list_node)) != NULL){
                if((((PeerProperties*)SinglyLinkedList_getItem(aux_peer_list_node))->num_connected_clients) <= low){
                    low = ((PeerProperties*)SinglyLinkedList_getItem(aux_peer_list_node))->num_connected_clients;
                    low_list_node = aux_peer_list_node;
                    peerAvailable = true;
                }
            }
        }
        if(true == peerAvailable){
            message_gw.type = PEER_ADDRESS;
            peer_socket_address = ((PeerProperties*)SinglyLinkedList_getItem(aux_peer_list_node))->peer_socket_stream_address;
            pthread_mutex_lock(&client_list_mutex);
            ((ClientProperties*)SinglyLinkedList_getItem(aux_client_list_node))->connected_peer_socket_address = peer_socket_address;
            pthread_mutex_lock(&client_list_mutex);
            message_gw.address = ntohl(peer_socket_address.sin_addr.s_addr);
            message_gw.port = ntohs(peer_socket_address.sin_port);
            ((PeerProperties*)SinglyLinkedList_getItem(aux_peer_list_node))->num_connected_clients += 1;
        }

        pthread_mutex_unlock(&peer_list_mutex);
        // CRITTICAL SECTION END

        // send message_gw to client
        ret_val_send_to = sendto(socket_fd, &message_gw, sizeof(message_gw), NO_FLAGS, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address));
        fprintf(stdout, "Message sent to client is of type: %d\n", message_gw.type); // DEBUG

        // We free the args which were allocated in the calling thread; must free list of clients in main
        free(client_recv_thread_args);

    pthread_exit(NULL);
}

//Thread always checking for new client connections (INFINITE LOOP)
void* masterClientRecvThread(void* args)
{
    long int i = 0;
    int socket_fd = 0;
    int ret_val_recvfrom = 0;
    int ret_val_send_to = 0;
    int ret_val_pthread_create = 0;
    pthread_t* thread_client_recv_id = NULL;
    struct sockaddr_in client_socket_address;
    socklen_t client_socket_address_len = sizeof(client_socket_address);
    SinglyLinkedList* client_list_head = NULL;
    SinglyLinkedList* peer_list_head = NULL;
    Message_gw message_gw;
    MasterClientRecvThreadArgs* masterClientRecvThreadArgs = NULL;
    ClientRecvThreadArgs* clientRecvThreadArgs = NULL;

        // TODO: big enough? when to increase? how to increase?
        thread_client_recv_id = (pthread_t*)malloc(HUGE_NUMBER * sizeof(pthread_t));
        masterClientRecvThreadArgs = (MasterClientRecvThreadArgs*)args;
        socket_fd = masterClientRecvThreadArgs->socket_fd;
        client_list_head = masterClientRecvThreadArgs->client_list_head;
        peer_list_head = masterClientRecvThreadArgs->peer_list_head;

        while(true){
            memset((void*)&client_socket_address, 0, sizeof(client_socket_address));
            ret_val_recvfrom = recvfrom(socket_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&client_socket_address, &client_socket_address_len);

            if(message_gw.type == CLIENT_ADDRESS){
                fprintf(stdout, "Received client\n");
                clientRecvThreadArgs = (ClientRecvThreadArgs*)malloc(sizeof(ClientRecvThreadArgs));
                clientRecvThreadArgs->socket_fd = socket_fd;
                clientRecvThreadArgs->peer_list_head = peer_list_head;
                clientRecvThreadArgs->client_list_head = client_list_head;
                clientRecvThreadArgs->client_address = client_socket_address;
                ret_val_pthread_create = pthread_create(&thread_client_recv_id[i], NULL, &clientRecvThread, (void *)clientRecvThreadArgs);
                if(ret_val_pthread_create != 0){
                    fprintf(stderr, "recv_pthread_create (client - slave) error!\n");
                    exit(EXIT_FAILURE);
                }
                i += 1;
            }else{
                fprintf(stdout, "Client receive thread error: bad message type\n");
                message_gw.type = BAD_CLIENT;
                ret_val_send_to = sendto(socket_fd, &message_gw, sizeof(message_gw), NO_FLAGS, (struct sockaddr *)&client_socket_address, client_socket_address_len);
            }
        }

    free(thread_client_recv_id);
}

void* slavePeerPinger(void* args)
{
    SinglyLinkedList* peer_list_node = NULL;
    int ret_val_send_to = 0;
    int ret_val_recvfrom = 0;
    int socket_dgram_fd = 0;
    struct sockaddr_in peer_socket_dgram_address;
    socklen_t peer_socket_dgram_address_len = sizeof(peer_socket_dgram_address);
    struct timeval ping_send_timeout;
    struct timeval ping_recv_timeout;
    Message_ping message_ping;

        peer_list_node = (SinglyLinkedList*)args;

        // timeouts for dgram socket
        ping_send_timeout.tv_sec = PEER_ALIVE_INTERVAL;
        ping_send_timeout.tv_usec = 0;
        ping_recv_timeout.tv_sec = PEER_ALIVE_INTERVAL;
        ping_recv_timeout.tv_usec = 0;

        // TODO: error handling
        socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        setsockopt(socket_dgram_fd, SOL_SOCKET, SO_SNDTIMEO, (void*)&ping_send_timeout, sizeof(ping_send_timeout));
        setsockopt(socket_dgram_fd, SOL_SOCKET, SO_RCVTIMEO, (void*)&ping_recv_timeout, sizeof(ping_recv_timeout));

        ret_val_send_to = sendto(socket_dgram_fd, &message_ping, sizeof(message_ping), NO_FLAGS, (struct sockaddr *)&peer_socket_dgram_address, peer_socket_dgram_address_len);
        if(ret_val_send_to == -1){
            pthread_mutex_lock(&peer_list_mutex);
            SinglyLinkedList_deleteNode(peer_list_node, NULL);
            pthread_mutex_unlock(&peer_list_mutex);
        }

        ret_val_recvfrom = recvfrom(socket_dgram_fd, &message_ping, sizeof(message_ping), NO_FLAGS, (struct sockaddr *)&peer_socket_dgram_address, &peer_socket_dgram_address_len);
        if(ret_val_recvfrom == -1){
            pthread_mutex_lock(&peer_list_mutex);
            SinglyLinkedList_deleteNode(peer_list_node, NULL);
            pthread_mutex_unlock(&peer_list_mutex);
        }

    pthread_exit(NULL);
}

// Thread that is always checking if peers are alive
// TODO
void* masterPeerPinger(void* args)
{
    long int i = 0;
    long int j = 0;
    SinglyLinkedList* peer_list_head = NULL;
    SinglyLinkedList* aux_peer_list_node = NULL;
    pthread_t* thread_peer_pinger_id;

        peer_list_head = (SinglyLinkedList*)args;
        thread_peer_pinger_id = (pthread_t*)malloc(HUGE_NUMBER * sizeof(pthread_t));

        while(true){
            i = 0;
            for(aux_peer_list_node = peer_list_head; SinglyLinkedList_getNextNode(aux_peer_list_node) != NULL; aux_peer_list_node = SinglyLinkedList_getNextNode(aux_peer_list_node)){
                pthread_create(&thread_peer_pinger_id[i], NULL, &slavePeerPinger, aux_peer_list_node);
            }
            for(j = 0; j < i; j++){ // join the threads in this iteration
                pthread_join(thread_peer_pinger_id[j], NULL);
            }

            sleep(PEER_ALIVE_INTERVAL);
        }

        free(thread_peer_pinger_id);

    pthread_exit(NULL);
}
