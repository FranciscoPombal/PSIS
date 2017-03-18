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

int main(int argc, char** argv)
{
    FILE* file = NULL;
    char* buffer;
    char* fgets_ret_val = NULL;
    int sscanf_ret_val = 0;
    char* command = NULL;
    double command_time = 0.0;
    double total_time = 0.0;
    clock_t begin = 0;
    clock_t end = 0;


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


        while((fgets_ret_val = fgets(buffer, LINE_BUFFER, file)) != NULL){
            // a bit of regex: [^] means "negated set" ([] means set),
            // so we are matching a string that contains any character except \n
            sscanf_ret_val = sscanf(buffer, "%[^\n]s", command);
            if(sscanf_ret_val != 1){
                fprintf(stderr, "sscanf error\n");
                exit(EXIT_FAILURE);
            }

            fprintf(stdout, "\n\n>>>>>NOW EXECUTING COMMAND: %s\n", command);
            begin = clock();    // Start timer
            system(command);    // Execute command
            end = clock();      // End timer
            command_time = ((double)end - (double)begin) / (double)CLOCKS_PER_SEC;  // Calculate command time
            fprintf(stdout, "Time taken for this command: %lf\n", command_time);
            total_time += command_time; // Increment total time
        }

        fprintf(stdout, "Total time taken to execute commands: %lf\n", total_time);

        free(buffer);
        free(command);

    return 0;
}
