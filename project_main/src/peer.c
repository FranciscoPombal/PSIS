#include "../include/peer.h"

int main(void)
{
    // generic/program-specific variables
    long int i = 0;

    // socket/ipc related variables
    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    int* conn_sock_fd = NULL;

    //thread stuff
    pthread_t thread_master_client_accept_id = 0;
    pthread_t thread_pinger_id = 0;
    int ret_val_phtread_create = 0;
    ClientHandlerThreadArgs* clientHandlerThreadArgs = NULL;

    // photo stuff
    SinglyLinkedList* photo_linked_list;

        // Setup SIGINT handler
        setupInterrupt();

        conn_sock_fd = (int*)malloc(HUGE_NUMBER * sizeof(int));

        photo_linked_list = SinglyLinkedList_newNode(NULL);
        clientHandlerThreadArgs = malloc(sizeof(ClientHandlerThreadArgs));
        clientHandlerThreadArgs->photo_list_head = photo_linked_list;

        // TODO: we may need more sockets for syncing with the gateway and other peers
        /* socket: create a new communication endpoint */
        // stream socket
        socket_stream_fd = clientStreamSocketSetup();
        // dgram socket (for sending and receiving)
        socket_dgram_fd = gatewayConnect(socket_stream_fd);

        fprintf(stdout, "Peer socket stream address sent to gateway via the dgram socket.\n");

        // start the pinger thread
        ret_val_phtread_create = pthread_create(&thread_pinger_id, NULL, &pingerThread, &socket_dgram_fd);

        // TODO: the rest of the peer
        while(true == keepRunning){

            conn_sock_fd[i] = accept(socket_stream_fd, NULL, NULL);
            clientHandlerThreadArgs->socket_fd = conn_sock_fd[i];

            // TODO: check atributes and arguments
            ret_val_phtread_create = pthread_create(&thread_master_client_accept_id, NULL, &clientHandlerThread, clientHandlerThreadArgs);

            i += 1;
        }


        close(socket_stream_fd);
        close(socket_dgram_fd);
        free(conn_sock_fd);
        fprintf(stdout, "Caught SIGINT, exiting cleanly\n");

    exit(EXIT_SUCCESS);
}
