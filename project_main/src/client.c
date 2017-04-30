#include "../include/messages.h"

int main(void)
{
    Message message;
    Message_gw message_gw;

    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    int ret_val_bind = 0;
    int client_port = 0;
    struct sockaddr_in server_socket_address;
    struct sockaddr_in client_socket_address;
    struct sockaddr_in gateway_socket_address;
    long int ret_val_send = 0;
    long int ret_val_send_to = 0;
    long int ret_val_recv = 0;
    int ret_val_connect = 0;
    long int story_len = 0;

    char* story_to_receive = NULL;

    int ret_val_sscanf = 0;

    //gateway address
    char char_buffer[1024];
    int gateway_port = 0;

        /* socket: create a new communication endpoint */
        // stream socket
        socket_stream_fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
        if(socket_stream_fd == -1){
            fprintf(stderr, "Error creating stream socket\n");
            exit(EXIT_FAILURE);
        }

        // dgram socket
        socket_dgram_fd = socket(AF_INET, SOCK_DGRAM, DEFAULT_PROTOCOL);
        if(socket_dgram_fd == -1){
            fprintf(stderr, "Error creating dgram socket\n");
            exit(EXIT_FAILURE);
        }

        memset((void*)&client_socket_address, 0, sizeof(client_socket_address));  // reset struct
        client_socket_address.sin_family = AF_INET;
        client_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        client_port = BASE_PORT + getpid();
        if(client_port > USHRT_MAX){
            fprintf(stderr, "Port number too large\n");
            exit(EXIT_FAILURE);
        }
        client_socket_address.sin_port = htons(client_port);

        /* bind datagram socket, since we will be receiving */
        ret_val_bind = bind(socket_dgram_fd, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address));
        if(ret_val_bind == -1){
            fprintf(stderr, "Error binding socket dgram\n");
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

        fprintf(stdout, "Got port %d\n", gateway_port);

        /* set gateway address struct */
        memset((void*)&gateway_socket_address, 0, sizeof(gateway_socket_address));   // first we reset the struct
        gateway_socket_address.sin_family = AF_INET;
        gateway_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_socket_address.sin_port = htons(gateway_port);

        memset((void*)&server_socket_address, 0, sizeof(server_socket_address));  // reset struct
        server_socket_address.sin_family = AF_INET;

        message_gw.type = CLIENT_ADDRESS;
        message_gw.port = client_socket_address.sin_port;
        message_gw.address = client_socket_address.sin_addr.s_addr;

        ret_val_send_to = sendto(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS, (struct sockaddr *)&gateway_socket_address, sizeof(gateway_socket_address));

        ret_val_recv = recv(socket_dgram_fd, &message_gw, sizeof(Message_gw), NO_FLAGS);

        if(message_gw.type == SERVER_ADDRESS){
            server_socket_address.sin_addr.s_addr = htonl(message_gw.address);
            server_socket_address.sin_port = htons(message_gw.port);
            fprintf(stdout, "CLIENT go server address: %s and port: %d\n", inet_ntoa(server_socket_address.sin_addr), server_socket_address.sin_port);
        }else{
            fprintf(stderr, "Error, probably\n");
        }

        /* Connect socket to socket address (server) */
        /* connect */
        ret_val_connect = connect(socket_stream_fd, (struct sockaddr *)&server_socket_address, sizeof(server_socket_address));
        if(ret_val_connect != 0){  // check for error
            fprintf(stderr, "Error connecting socket.\n");
            perror("connect");
            exit(EXIT_FAILURE);
        }

        // prompt for message
        printf("message: ");
        fgets(message.buffer, MESSAGE_LEN, stdin);

        /* send message, receive story */
        ret_val_send = send(socket_stream_fd, message.buffer, MESSAGE_LEN, 0);
        if(ret_val_send == -1){  // check for error
            fprintf(stderr, "Error sending message.\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv = recv(socket_stream_fd, &story_len, sizeof(long int), 0);
        if(ret_val_recv == -1){  // check for error
            fprintf(stderr, "Error receiving data (story len).\n");
            exit(EXIT_FAILURE);
        }

        story_to_receive = (char*)malloc(story_len * sizeof(char));
        if(story_to_receive == NULL){
            fprintf(stderr, "Error allocating memory to receive story\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv = recv(socket_stream_fd, story_to_receive, story_len, 0);
        if(ret_val_recv == -1){  // check for error
            fprintf(stderr, "Error receiving data (story string).\n");
            exit(EXIT_FAILURE);
        }

        /* print received message, clean up and exit */
        fprintf(stdout, "Story so far:\n%s\n", story_to_receive);

        free(story_to_receive);
        close(socket_stream_fd);
        close(socket_dgram_fd);

    exit(EXIT_SUCCESS);
}
