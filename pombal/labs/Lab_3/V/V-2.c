#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#ifndef NUM_MAX_SECONDS
#define NUM_MAX_SECONDS 10
#endif

#ifndef AUX_SECONDS
#define AUX_SECONDS 3
#endif

static int i = 0;

void alarm_signal_handler(int);

// signal handler function
void alarm_signal_handler(int sig)
{
    i = 0;

    fprintf(stdout, "Counter reset. Signal identifier: %d\n", sig);

    // sleep for a bit just to see the printf
    sleep(AUX_SECONDS);

    // re-arm alarm
    alarm(rand() % (NUM_MAX_SECONDS + 1));

    return;
}

int main(void)
{
    struct sigaction new_action;
    new_action.sa_handler = alarm_signal_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    // rand seed
    srand((unsigned)time(NULL));

    // arm alarm
    alarm(rand() % (NUM_MAX_SECONDS + 1));

    // SIGALRM == 14
    sigaction(SIGALRM, &new_action, NULL);

        while(true){
            i += 1;
            fprintf(stdout, "%d\n", i);
        }

    //return 0;
}
