#include "storyserver.h"

#include <string.h>
#include <signal.h>

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
    char* story = NULL;
    long int story_len = 0;

    int socket_fd = 0;
    int conn_sock_fd = 0;
    int ret_val_bind = 0;
    int ret_val_listen = 0;
    long int ret_val_send = 0;
    long int ret_val_recv = 0;
    struct sockaddr_in server_socket_address;   // address of the server socket

    struct sigaction sigint_action;

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
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_fd == -1){
            fprintf(stderr, "Error opening socket.\n");
            exit(EXIT_FAILURE);
        }

        /* connect socket to socket address */
        unlink(SOCKET_NAME);    // unlink file in case previously the server exited badly
        memset(&server_socket_address, 0, sizeof(server_socket_address));   // first we reset the struct
        server_socket_address.sin_family = AF_INET;
        server_socket_address.sin_addr.s_addr = INADDR_ANY;
        server_socket_address.sin_port = htons(PORT);


        /* bind: attach a local address to a socket */
        ret_val_bind = bind(socket_fd, (struct sockaddr *)&server_socket_address, sizeof(server_socket_address));  // call bind
        if(ret_val_bind == -1){  // check for error
            fprintf(stderr, "Error binding socket\n");
            exit(EXIT_FAILURE);
        }

        /* listen on the socket */
        ret_val_listen = listen(socket_fd, BACKLOG_MAX);
        if(ret_val_listen == -1){  // check for error
            fprintf(stderr, "Error listening for connections.\n");
            exit(EXIT_FAILURE);
        }

        while(true == keepRunning){

            conn_sock_fd = accept(socket_fd, NULL, NULL);


            // receive message from client
            ret_val_recv = recv(conn_sock_fd, m.buffer, MESSAGE_LEN, 0);

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
                ret_val_send = send(conn_sock_fd, &story_len, sizeof(long int), 0);
                if(ret_val_send == -1){  // check for error
                    fprintf(stderr, "Error sending story length.\n");
                    exit(EXIT_FAILURE);
                }

                ret_val_send = send(conn_sock_fd, story, strlen(story), 0);
                if(ret_val_send == -1){  // check for error
                    fprintf(stderr, "Error sending story.\n");
                    exit(EXIT_FAILURE);
                }
            }

            // close connection with current client
            close(conn_sock_fd);
        }

        // TODO: close all
        close(socket_fd);
        unlink(SOCKET_NAME);
        fprintf(stdout, "Caught SIGINT, exiting cleanly\n");

        /* print the final story */
        if(strlen(story) != 0){
            fprintf(stdout, "*** STORY:\n%s\n", story);
        }

        free(story);

    exit(EXIT_SUCCESS);
}
