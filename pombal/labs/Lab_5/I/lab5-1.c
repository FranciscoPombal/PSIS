#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
// for pipes:
#define _GNU_SOURCE
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <unistd.h>

int main(void)
{
    int i = 0;
    int m_7_19 = 0;
    int m_19 = 0;
    int m_7 = 0;

    int pipefd[3][2];
    pid_t pid[3];

        for(i = 0; i < 3; ++i){
            if(pipe(pipefd[i]) != 0){
                fprintf(stderr, "pipe error.\n");
                exit(EXIT_FAILURE);
            }
            if((pid[i] = fork()) == -1){
                fprintf(stderr, "fork error.\n");
                exit(EXIT_FAILURE);
            }
            if(pid[i] == 0){
                break;
            }
        }

        switch(i){
            case 0:
                // close unused pipe write end
                close(pipefd[0][1]);

                while(read(pipefd[0][0], &i, sizeof(int)) > 0){
                    if((i % 7) == 0){
                        m_7++;
                    }
                }

                fprintf(stdout,"m 7\t%d\n", m_7);
                close(pipefd[0][0]);
                exit(EXIT_SUCCESS);
                break;
            case 1:
                // close unused pipe write end
                close(pipefd[1][1]);
                //close repeated pipes
                close(pipefd[0][0]);
                close(pipefd[0][1]);

                while(read(pipefd[1][0], &i, sizeof(int)) > 0){
                    if((i % 19) == 0){
                        m_19++;
                    }
                }

                fprintf(stdout,"m 19\t%d\n", m_19);
                close(pipefd[1][0]);
                exit(EXIT_SUCCESS);
                break;
            case 2:
                // close unused pipe write end
                close(pipefd[2][1]);
                //close repeated pipes
                close(pipefd[0][0]);
                close(pipefd[0][1]);
                close(pipefd[1][0]);
                close(pipefd[1][1]);

                    while(read(pipefd[2][0], &i, sizeof(int)) > 0){
                        if(((i % 19) == 0) && ((i % 7) == 0)){
                            m_7_19++;
                        }
                    }

                fprintf(stdout,"m 7 19\t%d\n", m_7_19);
                close(pipefd[2][0]);
                exit(EXIT_SUCCESS);
                break;
            default:
                ;
        }

        i = 0;
        do{
            i++;
            write(pipefd[0][1], &i, sizeof(int));
            write(pipefd[1][1], &i, sizeof(int));
            write(pipefd[2][1], &i, sizeof(int));
        }while(i < 999999);
        close(pipefd[0][1]);
        close(pipefd[1][1]);
        close(pipefd[2][1]);

        wait(NULL);

    return 0;
}
