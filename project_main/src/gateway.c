#include "../include/gateway.h"

bool keepRunning = true;

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

    //signal handling
    struct sigaction sig_action;
    sigset_t sigset;
    sigset_t oldset;

    // thread stuff
    int ret_val_recv_phtread_create = 0;

    // recv threads
    pthread_t thread_master_peer_recv_id = 0;
    pthread_t thread_master_client_recv_id = 0;
    MasterPeerRecvThreadArgs* masterPeerRecvThreadArgs = NULL;
    MasterClientRecvThreadArgs* masterClientRecvThreadArgs = NULL;

    // pinger thread
    pthread_t thread_master_peer_pinger_id = 0;

    // other threads
    pthread_t* thread_ids = NULL;

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
            sleep(1);
        }

    fprintf(stdout, "Cleaning up...\n");
    pthread_cancel(thread_master_peer_recv_id);
    pthread_cancel(thread_master_client_recv_id);
    pthread_cancel(thread_master_peer_pinger_id);

    // close and free stuff
    close(socket_dgram_peers_fd);
    close(socket_dgram_clients_fd);
    free(thread_ids);
    free(masterClientRecvThreadArgs);
    free(masterPeerRecvThreadArgs);
    thread_ids = NULL;
    masterClientRecvThreadArgs = NULL;
    masterPeerRecvThreadArgs = NULL;

    SinglyLinkedList_freeList(peer_linked_list, NULL);
    SinglyLinkedList_freeList(client_linked_list, NULL);

    return 0;
}
