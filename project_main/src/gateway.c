#include "../include/gateway.h"

int main(void)
{
    // socket stuff
    int socket_dgram_clients_fd = 0;
    int socket_dgram_peers_fd = 0;

    //linked list stuff
    // Linked list of peers
    SinglyLinkedList* peer_linked_list = NULL;
    // Linked list of clients
    SinglyLinkedList* client_linked_list = NULL;

    // thread stuff
    // recv threads
    pthread_t thread_master_peer_recv_id = 0;
    pthread_t thread_master_client_recv_id = 0;
    pthread_t thread_master_peer_pinger_id = 0;
    MasterPeerRecvThreadArgs* masterPeerRecvThreadArgs = NULL;
    MasterClientRecvThreadArgs* masterClientRecvThreadArgs = NULL;

    pthread_t* threadid = NULL;
    int ret_val_recv_phtread_create = 0;

        threadid = (pthread_t*)malloc(HUGE_NUMBER * sizeof(pthread_t));

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
        ret_val_recv_phtread_create = pthread_create(&thread_master_peer_recv_id, NULL, &masterPeerRecvThread, masterPeerRecvThreadArgs);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create (peer) error!\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv_phtread_create = pthread_create(&thread_master_client_recv_id, NULL, &masterClientRecvThread, masterClientRecvThreadArgs);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create (client) error!\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv_phtread_create = pthread_create(&thread_master_peer_pinger_id, NULL, &masterPeerPinger, peer_linked_list);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create (peer pinger) error!\n");
            exit(EXIT_FAILURE);
        }

        while(true == keepRunning){
            // TODO: rest of gateway
        }

    // TODO: probably not needed
    pthread_cancel(thread_master_peer_recv_id);
    pthread_cancel(thread_master_client_recv_id);

    // close and free stuff
    close(socket_dgram_peers_fd);
    close(socket_dgram_clients_fd);
    free(threadid);

    return 0;
}
