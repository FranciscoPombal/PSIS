#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(void)
{

    unsigned int i = 0;

        unsigned int vals[3] = {UINT_MAX / 3, UINT_MAX / 3 * 2, UINT_MAX};
        unsigned int results[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        pid_t pid[3];

        for (i=0; i < 3; ++i) {
            if((pid[i] = fork()) == -1){
                return (1); // handle this error as you want
            }
            if(pid[i] == 0){
                break;
            }
        }

        switch(i){
            case 0:
                for(i = 0; i < vals[0]; i++){
                    if(i % 7 == 0){
                        results[0]++;
                    }
                    if(i % 19 == 0){
                        results[1]++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        results[2]++;
                    }
                }
                fprintf(stdout, "child %d\nm_7 | m_19 | m_7_19\n%d | %d | %d\n", i+1, results[0], results[1], results[2]);
                exit(EXIT_SUCCESS);
                break;
            case 1:
                for(i = vals[0] + 1; i < vals[1]; i++){
                    if(i % 7 == 0){
                        results[3]++;
                    }
                    if(i % 19 == 0){
                        results[4]++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        results[5]++;
                    }
                }
                fprintf(stdout, "child %d\nm_7 | m_19 | m_7_19\n%d | %d | %d\n", i+1, results[3], results[4], results[5]);
                exit(EXIT_SUCCESS);
                break;
            case 2:
                for(i = vals[1] + 1; i < vals[2]; i++){
                    if(i % 7 == 0){
                        results[6]++;
                    }
                    if(i % 19 == 0){
                        results[7]++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        results[8]++;
                    }
                }
                fprintf(stdout, "\nchild %d\nm_7 | m_19 | m_7_19\n%d | %d | %d\n\n", i+1, results[6], results[7], results[8]);
                exit(EXIT_SUCCESS);
                break;
            default:
                ;
        }

        int status;
        int n = 3;

        pid_t pids;
        while (n > 0) {
            pids = wait(&status);
            printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
            --n;
        }

    return 0;
}
