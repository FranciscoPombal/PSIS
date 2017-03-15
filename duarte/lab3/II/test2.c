#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main ( int argc, char *argv[] )
{
    int i, pid, sleepTime = 0, totalSleepTime=0;
    int killedChild=0, childStatus=0, childReturnValue=0;
    printf("Parent ID: %d\n\n", getpid());

for(i = 0; i < atoi(argv[1]); i++) {
    pid = fork();
    if(pid < 0) {
        printf("Error");
        exit(1);
    } else if (pid == 0) { //child
        printf("Child (%d): %d\n", i + 1, getpid());
        srandom(time(NULL));
        sleepTime = (int) (random()%11);
        sleep(sleepTime);
        printf("%d slept for %d seconds\n", getpid(), sleepTime);
        exit(sleepTime);
    } else  { //parent
        killedChild = wait(&childStatus);
        if(WIFEXITED(childStatus)){
            childReturnValue = WEXITSTATUS(childStatus);
            fprintf(stdout, "Child pid: %d  Child sleep time: %d\n", killedChild, childReturnValue);
            totalSleepTime += childReturnValue;
        }
    }
}
fprintf(stdout, "\nMy children slept for %d seconds!\n", totalSleepTime);
exit(0);

}
