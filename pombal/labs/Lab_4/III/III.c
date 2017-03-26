#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>

typedef struct{
    unsigned int start;
    unsigned int middle1;
    unsigned int middle2;
    unsigned int end;
    unsigned int m_7_array;
    unsigned int m_19_array;
    unsigned int m_7_19_array;
} com_struct;

#ifndef NUM_CHILDS
#define NUM_CHILDREN 3
#endif

int main(void)
{
    unsigned int i = 0;

        com_struct* sh_package = mmap(NULL, NUM_CHILDREN*sizeof(com_struct), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);


        for(i = 0; i < NUM_CHILDREN; i++){
            sh_package[i].start = 0;
            sh_package[i].middle1 = UINT_MAX / 3;
            sh_package[i].middle2 = UINT_MAX / 3 * 2;
            sh_package[i].end = UINT_MAX;
        }

        for(i = 0; i < NUM_CHILDREN; i++){
            sh_package[i].m_7_array = 0;
            sh_package[i].m_19_array = 0;
            sh_package[i].m_7_19_array = 0;
        }

        pid_t pid[NUM_CHILDREN];

        for(i = 0; i < NUM_CHILDREN; ++i){
            if((pid[i] = fork()) == -1){
                fprintf(stdout, "Fork error\n");
                exit(EXIT_FAILURE);
            }
            if(pid[i] == 0){
                break;
            }
        }

        switch(i){
            case 0:
                for(i = sh_package[0].start; i < sh_package[0].middle1; i++){
                    if(i % 7 == 0){
                        sh_package[0].m_7_array++;
                    }
                    if(i % 19 == 0){
                        sh_package[0].m_19_array++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        sh_package[0].m_7_19_array++;
                    }
                }
                exit(EXIT_SUCCESS);
                break;
            case 1:
                for(i = sh_package[1].middle1 + 1; i < sh_package[1].middle2; i++){
                    if(i % 7 == 0){
                        sh_package[1].m_7_array++;
                    }
                    if(i % 19 == 0){
                        sh_package[1].m_19_array++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        sh_package[1].m_7_19_array++;
                    }
                }
                exit(EXIT_SUCCESS);
                break;
            case 2:
                for(i = sh_package[2].middle2 + 1; i < sh_package[2].end; i++){
                    if(i % 7 == 0){
                        sh_package[2].m_7_array++;
                    }
                    if(i % 19 == 0){
                        sh_package[2].m_19_array++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        sh_package[2].m_7_19_array++;
                    }
                }
                exit(EXIT_SUCCESS);
                break;
            default:
                ;
        }


        int status;
        int n = NUM_CHILDREN;

        pid_t pids;
        while(n > 0){
            pids = wait(&status);
            printf("Child with PID %ld exited with status 0x%x.\n", (long)pids, status);
            --n;
        }

        unsigned int total_m_7 = sh_package[0].m_7_array + sh_package[1].m_7_array + sh_package[2].m_7_array;
        unsigned int total_m_19 = sh_package[0].m_19_array + sh_package[1].m_19_array + sh_package[2].m_19_array;
        unsigned int total_m_7_19 = sh_package[0].m_7_19_array + sh_package[1].m_7_19_array + sh_package[2].m_7_19_array;
        fprintf(stdout,"m 7    %d\n", total_m_7);
        fprintf(stdout,"m   19 %d\n", total_m_19);
        fprintf(stdout,"m 7 19 %d\n", total_m_7_19);

        munmap(sh_package, sizeof(com_struct));

    return 0;
}
