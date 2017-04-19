#include "storyserver.h"

static volatile int keepRunning = 1;

void sigIntHandler(int);

void sigIntHandler(int sig)
{
    fprintf(stdout, "Caught signal %d\n", sig);
    keepRunning = 0;

    return;
}

int main(void)
{
    // generic/program-specific variables
    int i = 0;
    message m;
    char* story = NULL;
    long int story_len = 0;
    int ret_val_sscanf = 0;

    //gateway address
    char* char_buffer;
    int gateway_port = 0;

    // socket/ipc related variables
    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    int server_port = 0;
    int conn_sock_fd = 0;
    int ret_val_bind = 0;
    int ret_val_listen = 0;
    long int ret_val_send = 0;
    long int ret_val_recv = 0;
    long int ret_val_send_to = 0;
    struct sockaddr_in server_socket_address;   // address of the server socket
    struct sockaddr_in gateway_socket_address;  // address of the gateway socket

    // signal realated variables
    struct sigaction sigint_action;

        char_buffer = malloc(CHAR_BUFFER_SIZE * sizeof(char));

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

        // initialize char arrays
        story = (char*)malloc(sizeof(char));
        *story = '\0';
        for(i = 0; i < MESSAGE_LEN; i++){
            m.buffer[i] = '\0';
        }

        /* socket: create a new communication endpoint */
        // stream socket
        socket_stream_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_stream_fd == -1){
            fprintf(stderr, "Error opening stream socket.\n");
            exit(EXIT_FAILURE);
        }

        //dgram socket
        socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, 0);
        if(socket_dgram_fd == -1){
            fprintf(stderr, "Error opening dgram socket.\n");
            exit(EXIT_FAILURE);
        }

        /* connect socket to socket address */
        memset(&server_socket_address, 0, sizeof(server_socket_address));   // first we reset the struct
        server_socket_address.sin_family = AF_INET;
        server_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        server_port = BASE_PORT + getpid();
        if(server_port > USHRT_MAX){
            fprintf(stderr, "Port number too large\n");
            exit(EXIT_FAILURE);
        }
        server_socket_address.sin_port = htons(server_port);
        fprintf(stdout, "Server datagram socket is on port: %d\n", server_port);

        /* bind: attach a local address to a socket */
        // no need to bind dgram socket, since we will only send

        // stream socket
        ret_val_bind = bind(socket_stream_fd, (struct sockaddr *)&server_socket_address, sizeof(server_socket_address));  // call bind
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
        fprintf(stdout, "Insert gateway port:\n");
        fgets(char_buffer, sizeof(char_buffer), stdin);
        ret_val_sscanf = sscanf(char_buffer, "%d", &gateway_port);
        if(ret_val_sscanf != 1){
            fprintf(stderr, "sscanf: error reading port number!\n");
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "Got port %d\n",gateway_port);

        /* set gateway address struct */
        memset(&gateway_socket_address, 0, sizeof(gateway_socket_address));   // first we reset the struct
        gateway_socket_address.sin_family = AF_INET;
        gateway_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_socket_address.sin_port = htons(gateway_port);

        // send server address to gateway
        ret_val_send_to = sendto(socket_dgram_fd, &server_socket_address, sizeof(server_socket_address), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));

        while(true == keepRunning){

            conn_sock_fd = accept(socket_stream_fd, NULL, NULL);

            // receive message from client
            ret_val_recv = recv(conn_sock_fd, m.buffer, MESSAGE_LEN, NO_FLAGS);

            if(strlen(m.buffer) != 0){
                /* process message */
                fprintf(stdout, "Received message: %s\n", m.buffer);
                story = realloc(story, strlen(story) + strlen(m.buffer) + 1);
                story = strcat(story, m.buffer);
                /* reset buffer receive buffer */
                for(i = 0; i < MESSAGE_LEN; i++){
                    m.buffer[i] = '\0';
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

        // TODO: close all
        close(socket_stream_fd);
        close(socket_dgram_fd);
        fprintf(stdout, "Caught SIGINT, exiting cleanly\n");

        /* print the final story */
        if(strlen(story) != 0){
            fprintf(stdout, "*** STORY:\n%s\n", story);
        }

    free(story);
    free(char_buffer);

    exit(EXIT_SUCCESS);
}
