#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#ifndef LINE_BUFFER
#define LINE_BUFFER 1024
#endif

/* TODO:
    Children get called to often
    Time
*/

int main(int argc, char** argv)
{
    FILE* file = NULL;
    char* buffer;
    char* fgets_ret_val = NULL;
    int sscanf_ret_val = 0;
    char* command = NULL;
    int pid = 0;
    int child_status = 0;
    int killed_child = 0;

        // Check arguments
        if(argc != 2){
            fprintf(stderr, "arg error\n");
            exit(EXIT_FAILURE);
        }

        // Open the file
        file = fopen(argv[1], "r");
        if(file == NULL){
            fprintf(stderr, "fopen error\n");
            exit(EXIT_FAILURE);
        }

        // Allocate memory for the buffers
        buffer = (char*)malloc(LINE_BUFFER * sizeof(char));
        if(buffer == NULL){
            fprintf(stderr, "malloc (buffer) error\n");
            exit(EXIT_FAILURE);
        }

        command = (char*)malloc(LINE_BUFFER * sizeof(char));
        if(command == NULL){
            fprintf(stderr, "malloc (command) error\n");
            exit(EXIT_FAILURE);
        }

        while(feof(file) == false){
            fgets_ret_val = fgets(buffer, LINE_BUFFER, file);
            if(fgets_ret_val == NULL){
                fprintf(stderr, "fgets error\n");
                exit(EXIT_FAILURE);
            }

            // a bit of regex: [^] means "negated set" ([] means set),
            // so we are matching a string that contains any character except \n
            sscanf_ret_val = sscanf(buffer, "%[^\n]s", command);
            if(sscanf_ret_val != 1){
                fprintf(stderr, "sscanf error\n");
                exit(EXIT_FAILURE);
            }

            fprintf(stdout, "NOW EXECUTING: %s\n", command);

            pid = fork();
            if(pid < 0){
                fprintf(stderr, "fork error.\n");
            }else if(pid == 0){
                //child
                system(command);
                exit(EXIT_SUCCESS);
            }else{
                killed_child = wait(&child_status);
                fprintf(stdout, "child exited \n");
                }
            }

        free(buffer);
        free(command);

    return 0;
}
