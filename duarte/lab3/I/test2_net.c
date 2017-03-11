#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ( int argc, char *argv[] )
{
    int i, pid, sleepTime = 0;

for(i = 0; i < atoi(argv[1]); i++) {
    pid = fork();
    if(pid < 0) {
        printf("Error");
        exit(1);
    } else if (pid == 0) {
        printf("Child (%d): %d\n", i + 1, getpid());
        sleepTime = random()%11;
        sleep(sleepTime);
        wait();
        printf("%d slept for %d seconds\n", getpid(), &sleepTime);
        exit(0);
    } else  {
        wait(NULL);
    }
}

}
