#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main ( int argc, char *argv[] )
{
    int i, pid, sleepTime = 0;
    printf("Parent ID: %d\n\n", getpid());

for(i = 0; i < atoi(argv[1]); i++) {
    pid = fork();
    if(pid < 0) {
        printf("Error");
        exit(1);
    } else if (pid == 0) {
        printf("Child (%d): %d\n", i + 1, getpid());
        srandom(time(NULL));
        sleepTime = (int) (random()%11);
        sleep(sleepTime);
        wait();
        printf("%d slept for %d seconds\n", getpid(), sleepTime);
        exit(0);
    } else  {
        wait(NULL);
    }
}
exit(0);

}
