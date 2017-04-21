#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>

#ifndef NUM_SECONDS
#define NUM_SECONDS 15
#endif

void alarm_signal_handler(int);
static int end = 0;

// signal handler function
void alarm_signal_handler(int sig)
{
    fprintf(stdout, "Program end. Signal identifier: %d\n", sig);
    end = 1;

    return;
}

int main(void)
{
    long long int i = 0;
    long long int odd = 0;
    int fd = 0;
    int r = 0;
    long long int* shared_package = NULL;
    const char* memname = "lab4_4_shm";

        srand((unsigned int)time(NULL));

        // arm alarm
        alarm(NUM_SECONDS);

        // SIGALRM == 14
        signal(SIGALRM, &alarm_signal_handler);

        fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);
        if(fd == -1){
            fprintf(stdout, "shm_open error\n");
            exit(EXIT_FAILURE);
        }

        r = ftruncate(fd, sizeof(long long int));
        if (r != 0) {
            fprintf(stdout, "fdtruncate error\n");
            exit(EXIT_FAILURE);
        }

        shared_package = mmap(NULL, sizeof(long long int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        *shared_package = 0;
        i = *shared_package;

            while(end == 0){
                if(i != *shared_package){
                    i = *shared_package;
                    if(i % 2 != 0){
                        odd++;
                    }
                }
            }

            fprintf(stdout, "Found %llu odd numbers total\n", odd);

        close(fd);
        munmap(shared_package, sizeof(long long int));
        shm_unlink(memname);

    return 0;
}
