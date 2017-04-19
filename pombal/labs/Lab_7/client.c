#include "storyserver.h"

int main(void)
{
    message m;

    int socket_stream_fd = 0;
    int socket_dgram_fd = 0;
    struct sockaddr_in server_socket_address;
    struct sockaddr_in gateway_socket_address;
    long int ret_val_send = 0;
    long int ret_val_recv = 0;
    int ret_val_connect = 0;
    long int story_len = 0;

    int server_port = 0;

    char* story_to_receive = NULL;

    int ret_val_sscanf = 0;

    //gateway address
    char char_buffer[1024];
    int gateway_port = 0;

        /* socket: create a new communication endpoint */
        socket_stream_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_stream_fd == -1){
            fprintf(stderr, "Error creating socket\n");
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
        memset(&gateway_socket_address, 0, sizeof(gateway_socket_address));   // first we reset the struct
        gateway_socket_address.sin_family = AF_INET;
        gateway_socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
        gateway_socket_address.sin_port = htons(gateway_port);

        exit(EXIT_SUCCESS);

        /* Connect socket to socket address (server) */
        memset((void*)&server_socket_address, 0, sizeof(server_socket_address));  // reset struct
        server_socket_address.sin_family = AF_INET;
        inet_aton("0.0.0.0", &server_socket_address.sin_addr);
        server_socket_address.sin_port = htons(server_port);

        /* connect */
        ret_val_connect = connect(socket_stream_fd, (struct sockaddr *)&server_socket_address, sizeof(server_socket_address));
        if(ret_val_connect != 0){  // check for error
            fprintf(stderr, "Error binding socket.\n");
            exit(EXIT_FAILURE);
        }

        // prompt for message
        printf("message: ");
        fgets(m.buffer, MESSAGE_LEN, stdin);

        /* send message, receive story */
        ret_val_send = send(socket_stream_fd, m.buffer, MESSAGE_LEN, 0);
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
