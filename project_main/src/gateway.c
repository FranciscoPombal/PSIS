#include "../include/gateway.h"

bool keepRunning = true;

int main(void)
{
    int i = 0;
    // socket stuff
    int socket_dgram_clients_fd = 0;
    int socket_dgram_peers_fd = 0;
    int socket_sync_fd = 0;

    Message_gw message_gw;
    struct sockaddr_in peer_socket_dgram_sync_recv_address;
    socklen_t peer_socket_dgram_sync_recv_address_len = sizeof(struct sockaddr_in);

    //linked list stuff
    // Linked list of peers
    SinglyLinkedList* peer_linked_list = NULL;
    // Linked list of clients
    SinglyLinkedList* client_linked_list = NULL;

    //signal handling
    struct sigaction sig_action;
    sigset_t sigset;
    sigset_t oldset;

    // thread stuff
    int ret_val_recv_phtread_create = 0;
    int ret_val_sync_pthread_create = 0;
    int ret_val_sync = 0;
    int last_sync = -1;
    int add_id = 0;
    int del_id = 0;
    int thread_queue = 0;

    // recv threads
    pthread_t thread_master_peer_recv_id = 0;
    pthread_t thread_master_client_recv_id = 0;
    MasterPeerRecvThreadArgs* masterPeerRecvThreadArgs = NULL;
    MasterClientRecvThreadArgs* masterClientRecvThreadArgs = NULL;

    // pinger thread
    pthread_t thread_master_peer_pinger_id = 0;

    // other threads
    pthread_t* thread_ids = NULL;
    pthread_t thread_sync_add[HUGE_NUMBER];
    pthread_t thread_sync_delete[HUGE_NUMBER];

    pthread_attr_t attr;

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        thread_ids = (pthread_t*)malloc(HUGE_NUMBER * sizeof(pthread_t));

        // Block the SIGINT signal. The threads will inherit the signal mask.
        // This will avoid them catching SIGINT instead of this thread.
        sigemptyset(&sigset);
        sigaddset(&sigset, SIGINT);
        pthread_sigmask(SIG_BLOCK, &sigset, &oldset);

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
        ret_val_recv_phtread_create = pthread_create(&thread_master_peer_recv_id, &attr, &masterPeerRecvThread, masterPeerRecvThreadArgs);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create (peer) error!\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv_phtread_create = pthread_create(&thread_master_client_recv_id, &attr, &masterClientRecvThread, masterClientRecvThreadArgs);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create (client) error!\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv_phtread_create = pthread_create(&thread_master_peer_pinger_id, &attr, &masterPeerPinger, peer_linked_list);
        if(ret_val_recv_phtread_create != 0){
            fprintf(stderr, "recv_pthread_create (peer pinger) error!\n");
            exit(EXIT_FAILURE);
        }

        // Install the signal handler for SIGINT.
        sig_action.sa_handler = sigIntHandler;
        sigemptyset(&sig_action.sa_mask);
        sig_action.sa_flags = 0;
        sigaction(SIGINT, &sig_action, NULL);

        // Restore the old signal mask only for this thread.
        pthread_sigmask(SIG_SETMASK, &oldset, NULL);

        while(true == keepRunning){
            // TODO: sync
            socket_sync_fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);

            memset((void*)&peer_socket_dgram_sync_recv_address,0 , sizeof(struct sockaddr_in));

            ret_val_sync = recvfrom(socket_sync_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&peer_socket_dgram_sync_recv_address, &peer_socket_dgram_sync_recv_address_len);

            switch(message_gw.type){
                case SYNC_ADD:
                if(last_sync != SYNC_ADD)
                {
                    for(i=thread_queue; i>0; i++)
                    {
                        pthread_join(thread_sync_delete[del_id-i+1], NULL);
                    }
                    thread_queue = 0;
                }
                fprintf(stdout, "Initializing SYNC ADD\n");
                ret_val_sync_pthread_create = pthread_create(&thread_sync_add[add_id], &attr, &peerSyncAdd, peer_linked_list);
                if(ret_val_sync_pthread_create != 0){
                    fprintf(stderr, "sync_pthread_create (initialize peer sync) error!\n");
                    exit(EXIT_FAILURE);
                }
                last_sync = SYNC_ADD;
                thread_queue++;
                break;

                case SYNC_DELETE:
                if(last_sync != SYNC_DELETE)
                {
                    for(i=thread_queue; i>0; i++)
                    {
                        pthread_join(thread_sync_add[add_id-i+1], NULL);
                    }
                    thread_queue = 0;
                }
                fprintf(stdout, "Initializing SYNC DELETE\n");
                ret_val_sync_pthread_create = pthread_create(&thread_sync_delete[del_id], &attr, &peerSyncDelete, peer_linked_list);
                if(ret_val_sync_pthread_create != 0){
                    fprintf(stderr, "sync_pthread_create (initialize peer sync) error!\n");
                    exit(EXIT_FAILURE);
                }
                last_sync = SYNC_DELETE;
                thread_queue++;
                break;
            }

            sleep(1);
        }

    fprintf(stdout, "Cleaning up...\n");
    pthread_cancel(thread_master_peer_recv_id);
    pthread_cancel(thread_master_client_recv_id);
    pthread_cancel(thread_master_peer_pinger_id);

    // close and free stuff
    close(socket_dgram_peers_fd);
    close(socket_dgram_clients_fd);
    close(socket_sync_fd);
    free(thread_ids);
    free(masterClientRecvThreadArgs);
    free(masterPeerRecvThreadArgs);
    free(thread_sync_add);
    free(thread_sync_delete);
    thread_ids = NULL;
    masterClientRecvThreadArgs = NULL;
    masterPeerRecvThreadArgs = NULL;

    SinglyLinkedList_freeList(peer_linked_list, NULL);
    SinglyLinkedList_freeList(client_linked_list, NULL);

    return 0;
}
