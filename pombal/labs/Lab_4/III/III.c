#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>

// TODO: shared memory would be useful

typedef struct{
    unsigned int start;
    unsigned int middle1;
    unsigned int middle2;
    unsigned int end;
    unsigned int m_7_array[3];
    unsigned int m_19_array[3];
    unsigned int m_7_19_array[3];
} com_struct;

int main(void)
{
    unsigned int i = 0;
    char* memname = "shmlab4";

        com_struct* sh_package = mmap(NULL, sizeof(com_struct), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

        sh_package->start = 0;
        sh_package->middle1 = UINT_MAX / 3;
        sh_package->middle2 = UINT_MAX / 3 * 2;
        sh_package->end = UINT_MAX;
        for(i = 0; i < 3; i++){
            sh_package->m_7_array[i] = 0;
            sh_package->m_19_array[i] = 0;
            sh_package->m_7_19_array[i] = 0;
        }

        pid_t pid[3];

        for (i=0; i < 3; ++i) {
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
                for(i = sh_package->start; i < sh_package->middle1; i++){
                    if(i % 7 == 0){
                        sh_package->m_7_array[0]++;
                    }
                    if(i % 19 == 0){
                        sh_package->m_19_array[0]++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        sh_package->m_7_19_array[0]++;
                    }
                }
                exit(EXIT_SUCCESS);
                break;
            case 1:
                for(i = sh_package->middle1 + 1; i < sh_package->middle2; i++){
                    if(i % 7 == 0){
                        sh_package->m_7_array[1]++;
                    }
                    if(i % 19 == 0){
                        sh_package->m_19_array[1]++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        sh_package->m_7_19_array[1]++;
                    }
                }
                exit(EXIT_SUCCESS);
                break;
            case 2:
                for(i = sh_package->middle2 + 1; i < sh_package->end; i++){
                    if(i % 7 == 0){
                        sh_package->m_7_array[2]++;
                    }
                    if(i % 19 == 0){
                        sh_package->m_19_array[2]++;
                    }
                    if((i % 7 == 0) && (i % 19 == 0)){
                        sh_package->m_7_19_array[2]++;
                    }
                }
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


        unsigned int total_m_7 = sh_package->m_7_array[0] + sh_package->m_7_array[1] + sh_package->m_7_array[2];
        unsigned int total_m_19 = sh_package->m_19_array[0] + sh_package->m_19_array[1] + sh_package->m_19_array[2];
        unsigned int total_m_7_19 = sh_package->m_7_19_array[0] + sh_package->m_7_19_array[1] + sh_package->m_7_19_array[2];
        fprintf(stdout,"m 7    %d\n", total_m_7);
        fprintf(stdout,"m   19 %d\n", total_m_19);
        fprintf(stdout,"m 7 19 %d\n", total_m_7_19);

        munmap(sh_package, sizeof(com_struct));
        shm_unlink(memname);

    return 0;
}
