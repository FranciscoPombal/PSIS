#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
//#include <sys/types.h>

#ifndef NUM_CHILD_PROCESSES
#define NUM_CHILD_PROCESSES 10
#endif

#ifndef NUM_SECONDS_SLEEP
#define NUM_SECONDS_SLEEP 10
#endif

int main(void)
{

    int i = 0;
    int pid = 0;
    unsigned int nsleep = 0;
    int killed_child = 0;
    int child_status = 0;
    int total_sleep = 0;
    int child_ret_val = 0;

        for(i = 0; i < NUM_CHILD_PROCESSES; i++){
            pid = fork();
            if(pid < 0){
                printf("Error");
                exit(EXIT_FAILURE);
            }else if(pid == 0){
                //child
                srand((unsigned)time(NULL));
                nsleep = rand() % (NUM_SECONDS_SLEEP + 1);
                sleep(nsleep);
                exit((int)nsleep);
            }else{
                //parent
                // Note: here, killed_child == pid
                killed_child = wait(&child_status);
                // if child exited cleanly
                if(WIFEXITED(child_status) == true){
                    child_ret_val = WEXITSTATUS(child_status);
                    fprintf(stdout, "child pid: %d (from wait()) %d (from pid var)| exited with return code: %d | child sleep time: %d\n", killed_child, pid, child_status, child_ret_val);
                    total_sleep += child_ret_val;
                }
            }
        }

        fprintf(stdout, "Total sleep time: %d\n", total_sleep);

    return 0;
}
