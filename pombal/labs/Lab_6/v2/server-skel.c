#include "storyserver.h"

#include <string.h>
#include <signal.h>

// TODO: sending everything at once in a struct requires serialization. will do it other way.

static volatile int keepRunning = 1;

void sigIntHandler(int);

void sigIntHandler(int sig)
{
    keepRunning = 0;
}

int main(void)
{
    int i = 0;
    message m;
    bool message_big = false;
    char* story = NULL;
    char* story_to_send = NULL;

    int socket_fd = 0;
    int ret_val_bind = 0;
    long int ret_val_sendto = 0;
    long int ret_val_recv_from = 0;
    struct sockaddr_un server_socket_address;   // address of the server socket
    struct sockaddr_un cli_socket_addr;
    unsigned int cli_addr_len = 0;

    struct sigaction sigint_action;

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

        story = (char*)malloc(sizeof(char));
        *story = '\0';
        for(i = 0; i < MESSAGE_LEN; i++){
            m.buffer[i] = '\0';
        }

        story_to_send = (char*)malloc(MESSAGE_LEN * sizeof(char));

        /* socket: create a new communication endpoint */
        socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
        if(socket_fd == -1){
            fprintf(stderr, "Opening socket\n");
            exit(EXIT_FAILURE);
        }

        /* connect socket to socket address */
        unlink(SOCKET_NAME);    // unlink file in case previously the server exited badly
        memset(&server_socket_address, 0, sizeof(server_socket_address));   // first we reset the struct
        server_socket_address.sun_family = AF_UNIX; // specify the sun_family parameter
        strncpy(server_socket_address.sun_path, SOCKET_NAME, sizeof(server_socket_address.sun_path) - 1);   // specify the sun_path

        /* bind: attach a local address to a socket */
        ret_val_bind = bind(socket_fd, (struct sockaddr *)&server_socket_address, sizeof(struct sockaddr_un));  // call bind
        if(ret_val_bind != 0){  // check for error
            fprintf(stderr, "Error binding socket\n");
            exit(EXIT_FAILURE);
        }

        /* listen until we catch SIGINT */
        while(keepRunning == true){
            /* receive the message */
            cli_addr_len = sizeof(cli_socket_addr);
            ret_val_recv_from = recvfrom(socket_fd, m.buffer, MESSAGE_LEN, 0, (struct sockaddr *)&cli_socket_addr, &cli_addr_len);

            if(strlen(m.buffer) != 0){
                /* process message */
                fprintf(stdout, "Received message: %s\n", m.buffer);    // DEBUG
                story = realloc(story, strlen(story) + strlen(m.buffer) + 1);
                story = strcat(story, m.buffer);
                strncpy(story_to_send, story, MESSAGE_LEN);
                /* reset buffer*/
                for(i = 0; i < MESSAGE_LEN; i++){
                    m.buffer[i] = '\0';
                }
            }

            //strncpy(m.buffer, story_to_send, MESSAGE_LEN);
            if(strlen(story) >= (MESSAGE_LEN - 1)){
                message_big = true;
            }else{
                message_big = false;
            }

            /*
                we only send back data if we have previously received successfully
                first we send the story and then the bool
            */
            if(ret_val_recv_from != -1){
                ret_val_sendto = sendto(socket_fd, story_to_send, MESSAGE_LEN - 1, 0, (struct sockaddr *)&cli_socket_addr, cli_addr_len);
                if(ret_val_sendto == -1){
                    fprintf(stderr, "Error sending data (story/partial story).\n");
                    perror("asd");
                    exit(EXIT_FAILURE);
                }

                ret_val_sendto = sendto(socket_fd, &(message_big), sizeof(message_big), 0, (struct sockaddr *)&cli_socket_addr, cli_addr_len);
                if(ret_val_sendto == -1){
                    fprintf(stderr, "Error sending data (bool).\n");
                    exit(EXIT_FAILURE);
                }
            }
        }

        /* close the socket and unlink file */
        close(socket_fd);
        unlink(SOCKET_NAME);
        printf("Caught SIGINT, exiting cleanly.\n");

        /* print final story */
        if(strlen(story) != 0){
            fprintf(stdout, "*** STORY:\n%s\n", story);
        }

        free(story);

    exit(EXIT_SUCCESS);
}
