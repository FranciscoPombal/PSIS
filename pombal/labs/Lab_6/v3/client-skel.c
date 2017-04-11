#include "storyserver.h"

int main(void)
{
    message m;

    int socket_fd = 0;
    struct sockaddr_un srv_socket_addr;
    long int ret_val_sendto = 0;
    long int ret_val_recv = 0;
    int ret_val_connect = 0;

        /* socket: create a new communication endpoint */
        socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if(socket_fd == -1){
            fprintf(stderr, "Error creating socket\n");
            exit(EXIT_FAILURE);
        }

        /* Connect socket to socket address (server) */
        memset(&srv_socket_addr, 0, sizeof(struct sockaddr_un));  // reset struct
        srv_socket_addr.sun_family = AF_UNIX;
        strncpy(srv_socket_addr.sun_path, SOCKET_NAME, sizeof(srv_socket_addr.sun_path) - 1);

        /* connect */
        ret_val_connect = connect(socket_fd, (struct sockaddr *)&srv_socket_addr, sizeof(struct sockaddr_un));
        if(ret_val_connect != 0){  // check for error
            fprintf(stderr, "Error binding socket\n");
            exit(EXIT_FAILURE);
        }

        // prompt for message
        printf("message: ");
        fgets(m.buffer, MESSAGE_LEN, stdin);

        // TODO: send/receive data
        // TODO: close


    exit(EXIT_SUCCESS);
}
