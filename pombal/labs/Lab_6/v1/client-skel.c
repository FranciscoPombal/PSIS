#include "storyserver.h"

int main(void)
{
    message m;
    int socket_fd = 0;
    struct sockaddr_un srv_socket_addr;
    long int ret_val_sendto = 0;

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

        // With datagram sockets, if we are only sending, no need to bind. The OS finds a port each time the socket sends a packet.
        // If we are receiving, we need to bind.

        // prompt for message
        printf("message: ");
        fgets(m.buffer, MESSAGE_LEN, stdin);

        /* send message */
        ret_val_sendto = sendto(socket_fd, m.buffer, MESSAGE_LEN, 0, (struct sockaddr *)&srv_socket_addr, sizeof(srv_socket_addr));
        if(ret_val_sendto == -1){
            fprintf(stderr, "Error sending data.\n");
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "Sent message: %s\n", m.buffer);

    exit(0);
}
