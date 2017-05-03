#include "../include/gateway.h"

void sigIntHandler(int sig)
{
    fprintf(stdout, "Caught signal %d\n", sig);
    keepRunning = 0;

    return;
}

void setupInterrupt(void)
{
    // signal realated variables
    struct sigaction sigint_action;

        /* setup sigIntHandler as the handler function for SIGINT */
        sigint_action.sa_handler = sigIntHandler;
        sigemptyset(&sigint_action.sa_mask);
        sigint_action.sa_flags = 0;
        sigaction(SIGINT, &sigint_action, NULL);

    return;
}

int getGatewayPort(void)
{
    char* char_buffer;
    int gateway_port = 0;
    int ret_val_sscanf = 0;

        char_buffer = (char*)malloc(CHAR_BUFFER_SIZE * sizeof(char));

        while((gateway_port < 1024) || (gateway_port > USHRT_MAX) || (ret_val_sscanf != 1)){
            fprintf(stdout, "Insert gateway port:\n");
            fgets(char_buffer, sizeof(char_buffer), stdin);
            ret_val_sscanf = sscanf(char_buffer, "%d\n", &gateway_port);

            if ((gateway_port < 1024) || (gateway_port > USHRT_MAX)){
                fprintf(stderr, "Invalid gateway port\n\n");
            }else if(ret_val_sscanf != 1){
                fprintf(stderr, "Error reading port\n\n");
            }
        }

        free(char_buffer);

    return gateway_port;
}
