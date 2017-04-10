#include "storyserver.h"

int main(void)
{
    /* helper variables for client socket name */
    char* client_socket_name = NULL;
    const char* client_socket_base_name = "/tmp/lab_6_socket_v2_client_";
    char* client_socket_name_helper = NULL;

    message m;
    bool message_big = false;
    char* story_to_receive = NULL;

    int socket_fd = 0;
    struct sockaddr_un srv_socket_addr;
    struct sockaddr_un cli_socket_addr;
    long int ret_val_sendto = 0;
    long int ret_val_recv = 0;
    int ret_val_bind = 0;

        /* determine client socket name */
        client_socket_name_helper = (char*)malloc(MESSAGE_LEN * sizeof(char));
        snprintf(client_socket_name_helper, MESSAGE_LEN, "%d", getpid());
        client_socket_name = (char*)malloc((strlen(client_socket_base_name) + strlen(client_socket_name_helper) + 1) * sizeof(char));
        strncpy(client_socket_name, client_socket_base_name, strlen(client_socket_base_name) + 1);
        strncat(client_socket_name, client_socket_name_helper, strlen(client_socket_name_helper) + 1);

        free(client_socket_name_helper);

        story_to_receive = (char*)malloc(MESSAGE_LEN * sizeof(char));
        int i = 0;
        for(i = 0; i < MESSAGE_LEN; i++){
            story_to_receive[i] = 0;
        }

        /* socket: create a new communication endpoint */
        socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if(socket_fd == -1){
            fprintf(stderr, "Error binding socket\n");
            exit(EXIT_FAILURE);
        }

        /* Connect socket to socket address */
        memset(&srv_socket_addr, 0, sizeof(struct sockaddr_un));  // reset struct
        srv_socket_addr.sun_family = AF_UNIX;
        strncpy(srv_socket_addr.sun_path, SOCKET_NAME, sizeof(srv_socket_addr.sun_path) - 1);

        /* client socket */
        memset(&cli_socket_addr, 0, sizeof(struct sockaddr_un));  // reset struct
        cli_socket_addr.sun_family = AF_UNIX;
        strncpy(cli_socket_addr.sun_path, client_socket_name, sizeof(srv_socket_addr.sun_path) - 1);

        /* bind: attach a local address to a socket */
        ret_val_bind = bind(socket_fd, (struct sockaddr *)&cli_socket_addr, sizeof(struct sockaddr_un));  // call bind
        if(ret_val_bind != 0){  // check for error
            fprintf(stderr, "Error binding socket\n");
            exit(EXIT_FAILURE);
        }

        // prompt for message
        printf("message: ");
        fgets(m.buffer, MESSAGE_LEN, stdin);

        /* send message */
        ret_val_sendto = sendto(socket_fd, m.buffer, MESSAGE_LEN, 0, (struct sockaddr *)&srv_socket_addr, sizeof(srv_socket_addr));
        if(ret_val_sendto == -1){
            fprintf(stderr, "Error sending data.\n");
            exit(EXIT_FAILURE);
        }

        /* receive story, then bool */
        ret_val_recv = recv(socket_fd, story_to_receive, MESSAGE_LEN - 1, 0);
        if(ret_val_recv == -1){
            fprintf(stderr, "Error receiving data.\n");
            exit(EXIT_FAILURE);
        }
        ret_val_recv = recv(socket_fd, &message_big, sizeof(message_big), 0);
        if(ret_val_recv == -1){
            fprintf(stderr, "Error receiving data.\n");
            exit(EXIT_FAILURE);
        }

        if(message_big == true){
            fprintf(stdout, "Story is bigger than message buffer. Partial story is:\n%s\n", story_to_receive);
        }else{
            fprintf(stdout, "Story is:\n%s\n", story_to_receive);
        }

        free(story_to_receive);
        free(client_socket_name);

    exit(0);
}
