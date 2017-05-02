#include "../include/peer.h"

int main(void)
{
    // generic/program-specific variables
    int i = 0;
    Message message;
    char* story = NULL;
    long int story_len = 0;

    // socket/ipc related variables
    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    int conn_sock_fd = 0;
    long int ret_val_send = 0;
    long int ret_val_recv = 0;

        // Setup SIGINT handler
        setupInterrupt();

        // TODO: we probably need to change these functions so that the y also give us the addresses
        /* socket: create a new communication endpoint */
        // stream socket
        socket_stream_fd = clientStreamSocketSetup();
        // dgram socket (for sending and receiving)
        socket_dgram_fd = gatewayConnect(socket_stream_fd);

        fprintf(stdout, "Peer address sent to gateway\n");

        // TODO: the rest of the peer
        while(true == keepRunning){

            conn_sock_fd = accept(socket_stream_fd, NULL, NULL);

            // receive message from client
            ret_val_recv = recv(conn_sock_fd, message.buffer, MESSAGE_LEN, NO_FLAGS);

            if(strlen(message.buffer) != 0){
                /* process message */
                fprintf(stdout, "Received message: %s\n", message.buffer);
                story = realloc(story, strlen(story) + strlen(message.buffer) + 1);
                story = strcat(story, message.buffer);
                /* reset buffer receive buffer */
                for(i = 0; i < MESSAGE_LEN; i++){
                    message.buffer[i] = '\0';
                }
            }

            story_len = strlen(story);

            // send story back to client
            if(ret_val_recv != -1){
                ret_val_send = send(conn_sock_fd, &story_len, sizeof(long int), NO_FLAGS);
                if(ret_val_send == -1){  // check for error
                    fprintf(stderr, "Error sending story length.\n");
                    exit(EXIT_FAILURE);
                }

                ret_val_send = send(conn_sock_fd, story, strlen(story), NO_FLAGS);
                if(ret_val_send == -1){  // check for error
                    fprintf(stderr, "Error sending story.\n");
                    exit(EXIT_FAILURE);
                }
            }

            // close connection with current client
            close(conn_sock_fd);
        }


        close(socket_stream_fd);
        close(socket_dgram_fd);
        fprintf(stdout, "Caught SIGINT, exiting cleanly\n");

        /* print the final story */
        if(strlen(story) != 0){
            fprintf(stdout, "*** STORY:\n%s\n", story);
        }

    free(story);

    exit(EXIT_SUCCESS);
}
