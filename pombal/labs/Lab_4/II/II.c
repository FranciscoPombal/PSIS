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
    unsigned int start, middle1, middle2, end;
} com_struct;

int main(void){
    com_struct limits;

    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    unsigned int l = 0;

        limits.start = 0;
        limits.middle1 = UINT_MAX / 3;
        limits.middle2 = UINT_MAX / 3 * 2;
        limits.end = UINT_MAX;

        const char* memname = "shared_memory_lab_4_2";

        // region_size == 4096
        const size_t region_size = (size_t)sysconf(_SC_PAGE_SIZE);

        int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
        if(fd == -1){
            fprintf(stdout, "shm_open error.");
        }

        int r = ftruncate(fd, (long int)region_size);
        if(r != 0){
            fprintf(stdout, "ftruncate");
        }

        unsigned int* ptr = (unsigned int*)mmap(NULL, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(ptr == MAP_FAILED){
            fprintf(stdout, "mmap");
        }
        close(fd);

        pid_t pid[3];

        unsigned int z = 0;
        for(z = 0; z < 128; z++) {
            *ptr = 0;
        }

        for (i=0; i < 3; ++i) {
            if((pid[i] = fork()) == -1){
                return (1); // handle this error as you want
            }
            if(pid[i] == 0){
                break;
            }
        }

        setbuf(stdout, NULL);
        switch(i){
            case 0:
                for(j = limits.start; j < limits.middle1; j++){
                    if(j % 7 == 0){
                        (*ptr)++;
                    }
                    if(j % 19 == 0){
                        (*(ptr+24))++;
                    }
                    if((j % 7 == 0) && (j % 19 == 0)){
                        (*(ptr+48))++;
                    }
                }
                exit(EXIT_SUCCESS);
                break;
            case 1:
                for(k = limits.middle1 + 1; k < limits.middle2; k++){
                    if(k % 7 == 0){
                        (*(ptr+8))++;
                    }
                    if(k % 19 == 0){
                        (*(ptr+32))++;
                    }
                    if((k % 7 == 0) && (k % 19 == 0)){
                        (*(ptr+56))++;
                    }
                }
                exit(EXIT_SUCCESS);
                break;
            case 2:
                for(l = limits.middle1 + 1; l < limits.middle2; l++){
                    if(l % 7 == 0){
                        (*(ptr+16))++;
                    }
                    if(l % 19 == 0){
                        (*(ptr+40))++;
                    }
                    if((l % 7 == 0) && (l % 19 == 0)){
                        (*(ptr+64))++;
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


    unsigned int total_m_7 = *ptr + *(ptr+8) + *(ptr+16);
    unsigned int total_m_19 = *(ptr+24) + *(ptr+32) + *(ptr+40);
    unsigned int total_m_7_19 = *(ptr+48) + *(ptr+56) + *(ptr+64);
        fprintf(stdout,"m 7    %d\n", total_m_7);
        fprintf(stdout,"m   19 %d\n", total_m_19);
        fprintf(stdout,"m 7 19 %d\n", total_m_7_19);

    return 0;
}
