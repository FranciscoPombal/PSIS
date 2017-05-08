#include "../include/peer.h"

int main(void)
{
    // generic/program-specific variables
    int i = 0;
    Message message;

    // socket/ipc related variables
    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    int conn_sock_fd = 0;
    long int ret_val_send = 0;
    long int ret_val_recv = 0;

    //thread stuff
    pthread_t thread_master_client_accept_id = 0;
    pthread_t thread_pinger_id = 0;
    int ret_val_phtread_create = 0;

        // Setup SIGINT handler
        setupInterrupt();

        // TODO: we probably need to change these functions so that the y also give us the addresses
        /* socket: create a new communication endpoint */
        // stream socket
        socket_stream_fd = clientStreamSocketSetup();
        // dgram socket (for sending and receiving)
        socket_dgram_fd = gatewayConnect(socket_stream_fd);

        fprintf(stdout, "Peer socket stream address sent to gateway via the dgram socket.\n");

        // TODO: setup the Pinger thread
        ret_val_phtread_create = pthread_create(&thread_pinger_id, NULL, &threadPinger, &socket_dgram_fd)

        // TODO: setup the accept thread

        // TODO: the rest of the peer
        while(true == keepRunning){

            conn_sock_fd = accept(socket_stream_fd, NULL, NULL);

            // TODO: call thread to handle each client. is thread detached

            // close connection with current client
            close(conn_sock_fd);
        }


        close(socket_stream_fd);
        close(socket_dgram_fd);
        fprintf(stdout, "Caught SIGINT, exiting cleanly\n");

    exit(EXIT_SUCCESS);
}
