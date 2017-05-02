#include "../include/peer.h"

int main(void)
{
    // generic/program-specific variables
    int i = 0;
    Message message;
    Message_gw message_gw;
    char* story = NULL;
    long int story_len = 0;

    // socket/ipc related variables
    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    int conn_sock_fd = 0;
    int ret_val_bind = 0;
    int ret_val_listen = 0;
    long int ret_val_send = 0;
    long int ret_val_recv = 0;
    long int ret_val_send_to = 0;
    struct sockaddr_in peer_socket_address;   // address of the server socket
    struct sockaddr_in gateway_socket_address;  // address of the gateway socket

        // Setup SIGINT handler
        setupInterrupt();

        /* socket: create a new communication endpoint */
        // stream socket
        socket_stream_fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
        if(socket_stream_fd == -1){
            fprintf(stderr, "Error opening stream socket.\n");
            exit(EXIT_FAILURE);
        }

        //dgram socket
        socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        if(socket_dgram_fd == -1){
            fprintf(stderr, "Error opening dgram socket.\n");
            exit(EXIT_FAILURE);
        }

        /* connect socket to socket address */
        setupPeerAddress(&peer_socket_address);

        /* bind: attach a local address to a socket */
        // TODO: we are probably need to bind the datagram socket, depending on whether we will be receiving stuff from the gateway

        // stream socket
        ret_val_bind = bind(socket_stream_fd, (struct sockaddr *)&peer_socket_address, sizeof(peer_socket_address));  // call bind
        if(ret_val_bind == -1){  // check for error
            fprintf(stderr, "Error binding socket stream\n");
            exit(EXIT_FAILURE);
        }

        /* listen on the socket */
        ret_val_listen = listen(socket_stream_fd, BACKLOG_MAX);
        if(ret_val_listen == -1){  // check for error
            fprintf(stderr, "Error listening for connections.\n");
            exit(EXIT_FAILURE);
        }

        /* get gateway address info */
        setupGatewayAddress(&gateway_socket_address);

        message_gw.address = peer_socket_address.sin_addr.s_addr;
        message_gw.type = PEER_ADDRESS;
        message_gw.port = ntohs(peer_socket_address.sin_port);

        // send server address to gateway
        ret_val_send_to = sendto(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));


        fprintf(stdout, "before while\n");

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
            //TODO  send availability status to gateway
            // send server address to gateway (will singnal availability)
            ret_val_send_to = sendto(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));
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
