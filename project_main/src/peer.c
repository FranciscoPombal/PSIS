#include "../include/peer.h"

bool keepRunning = true;

int main(void)
{
    // generic/program-specific variables
    long int i = 0;
    int k = 1;

    // socket/ipc related variables
    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    int socket_dgram_sync_send_fd = 0;
    int socket_dgram_sync_recv_fd = 0;
    int* conn_sock_fd = NULL;

    struct sockaddr_in gateway_socket_address;

    //thread stuff
    pthread_t thread_master_client_accept_id = 0;
    pthread_t thread_pinger_id = 0;

    pthread_t thread_sync_recv_id = 0;

    pthread_attr_t attr;
    int ret_val_phtread_create = 0;
    ClientHandlerThreadArgs* clientHandlerThreadArgs = NULL;
    SyncRecvThreadArgs* syncRecvThreadArgs = NULL;

    // photo stuff
    SinglyLinkedList* photo_linked_list = NULL;

        // Setup SIGINT handler
        setupInterrupt();

        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        conn_sock_fd = (int*)malloc(HUGE_NUMBER * sizeof(int));

        photo_linked_list = SinglyLinkedList_newNode(NULL);

        // TODO: we may need more sockets for syncing with the gateway and other peers
        /* socket: create a new communication endpoint */
        // stream socket
        socket_stream_fd = clientStreamSocketSetup();
        // dgram socket (for sending and receiving)
        socket_dgram_fd = gatewayConnect(socket_stream_fd, &gateway_socket_address, &socket_dgram_sync_send_fd, &socket_dgram_sync_recv_fd);


        fprintf(stdout, "Peer socket stream address sent to gateway via the dgram socket.\n");

        // start the pinger thread
        ret_val_phtread_create = pthread_create(&thread_pinger_id, &attr, &pingerThread, &socket_dgram_fd);

        // start the thread that receives sync requests
        syncRecvThreadArgs = (SyncRecvThreadArgs*)malloc(sizeof(SyncRecvThreadArgs));
        syncRecvThreadArgs->photo_list_head = photo_linked_list;
        syncRecvThreadArgs->peer_socket_address_sync_recv_dgram = socket_dgram_sync_recv_fd;
        ret_val_phtread_create = pthread_create(&thread_sync_recv_id, &attr, syncRecvThread, syncRecvThreadArgs);

        // TODO: the rest of the peer
        while(true == keepRunning){

            conn_sock_fd[i] = accept(socket_stream_fd, NULL, NULL);
            clientHandlerThreadArgs = malloc(sizeof(ClientHandlerThreadArgs));
            clientHandlerThreadArgs->socket_fd = conn_sock_fd[i];
            clientHandlerThreadArgs->photo_list_head = photo_linked_list;
            clientHandlerThreadArgs->peer_socket_address_sync_send_dgram = socket_dgram_sync_send_fd;

            ret_val_phtread_create = pthread_create(&thread_master_client_accept_id, &attr, &clientHandlerThread, clientHandlerThreadArgs);

            i += 1;
            if(i == HUGE_NUMBER - 1){
                k += 1;
                conn_sock_fd = realloc(conn_sock_fd, HUGE_NUMBER * k);
            }
        }

        fprintf(stdout, "Cleaning up...\n");
        pthread_cancel(thread_pinger_id);
        pthread_cancel(thread_master_client_accept_id);

        close(socket_stream_fd);
        close(socket_dgram_fd);
        free(conn_sock_fd);
        conn_sock_fd = NULL;
        fprintf(stdout, "Caught SIGINT, exiting cleanly\n");

    exit(EXIT_SUCCESS);
}
