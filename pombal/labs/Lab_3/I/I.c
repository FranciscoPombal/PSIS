#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#ifndef NUM_CHILD_PROCESSES
#define NUM_CHILD_PROCESSES 10
#endif

#ifndef NUM_SECONDS_SLEEP
#define NUM_SECONDS_SLEEP 10
#endif

int main(void) {

    int i = 0;
    int pid = 0;
    unsigned int nsleep = 0;
    time_t t;

        for(i = 0; i < NUM_CHILD_PROCESSES; i++){
            pid = fork();
            if(pid < 0){
                printf("Error");
                exit(EXIT_FAILURE);
            }else if(pid == 0){
                srand((unsigned) time(&t));
                nsleep = rand() % (NUM_SECONDS_SLEEP + 1);
                sleep(nsleep);
                printf("Child (%d): %d | Slept for %d seconds\n", i + 1, getpid(), nsleep);
                exit(EXIT_SUCCESS);
            }
        }

    return 0;
}
