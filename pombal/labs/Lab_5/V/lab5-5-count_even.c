#include <limits.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

#include "lab5-5.h"

#ifndef FIFO_NAME1
#define FIFO_NAME1 "lab5_fifo1"
#endif

#ifndef FIFO_NAME2
#define FIFO_NAME2 "lab5_fifo2"
#endif

void error_and_die(const char*)__attribute__((noreturn));

void error_and_die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(void)
{
    char *memname = "odd_even";
    int i = 0;
    long int num = 0;
    int fd2 = 0;

        int fd = shm_open(memname, O_RDWR, 0666);
        if(fd == -1){
            error_and_die("shm_open");
        }


        shm_region *ptr = mmap(0, sizeof(shm_region), PROT_READ, MAP_SHARED, fd, 0);
        if (ptr == MAP_FAILED){
            error_and_die("mmap");
        }
        close(fd);

        mkfifo(FIFO_NAME1, 0666);
        fd = open(FIFO_NAME1, O_RDWR);

        mkfifo(FIFO_NAME2, 0666);
        fd2 = open(FIFO_NAME2, O_RDWR);

            i = 0;
            while(read(fd, &num, sizeof(long int)) > 0){
                if(num % 2 == 0){
                    printf("%ld is even\n", num);
                    i++;
                }
            }

            fprintf(stdout,"There are %d even numbers\n", i);

    exit(EXIT_SUCCESS);
}
