#include "storyserver.h"

int main(void)
{
    message m;

    int socket_fd = 0;
    struct sockaddr_in srv_socket_addr;
    long int ret_val_send = 0;
    long int ret_val_recv = 0;
    int ret_val_connect = 0;
    long int story_len = 0;

    char* story_to_receive = NULL;

        /* socket: create a new communication endpoint */
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_fd == -1){
            fprintf(stderr, "Error creating socket\n");
            exit(EXIT_FAILURE);
        }

        /* Connect socket to socket address (server) */
        memset((void*)&srv_socket_addr, 0, sizeof(srv_socket_addr));  // reset struct
        srv_socket_addr.sin_family = AF_INET;
        inet_aton("0.0.0.0", &srv_socket_addr.sin_addr);
        srv_socket_addr.sin_port = htons(PORT);

        /* connect */
        ret_val_connect = connect(socket_fd, (struct sockaddr *)&srv_socket_addr, sizeof(srv_socket_addr));
        if(ret_val_connect != 0){  // check for error
            fprintf(stderr, "Error binding socket.\n");
            exit(EXIT_FAILURE);
        }

        // prompt for message
        printf("message: ");
        fgets(m.buffer, MESSAGE_LEN, stdin);

        /* send message, receive story */
        ret_val_send = send(socket_fd, m.buffer, MESSAGE_LEN, 0);
        if(ret_val_send == -1){  // check for error
            fprintf(stderr, "Error sending message.\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv = recv(socket_fd, &story_len, sizeof(long int), 0);
        if(ret_val_recv == -1){  // check for error
            fprintf(stderr, "Error receiving data (story len).\n");
            exit(EXIT_FAILURE);
        }

        story_to_receive = (char*)malloc(story_len * sizeof(char));
        if(story_to_receive == NULL){
            fprintf(stderr, "Error allocating memory to receive story\n");
            exit(EXIT_FAILURE);
        }

        ret_val_recv = recv(socket_fd, story_to_receive, story_len, 0);
        if(ret_val_recv == -1){  // check for error
            fprintf(stderr, "Error receiving data (story string).\n");
            exit(EXIT_FAILURE);
        }

        /* print received message, clean up and exit */
        fprintf(stdout, "Story so far:\n%s\n", story_to_receive);

        free(story_to_receive);
        close(socket_fd);

    exit(EXIT_SUCCESS);
}
