#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

static int i = 0;

void  ALARMhandler(int sig)
{
  i = 0; //Reset i
  printf("\nCounter was reset!\n");
  alarm(rand() % 11); //Set up alarm
}

int main(int argc, char const *argv[]) {

  struct sigaction new_action;

  new_action.sa_handler = ALARMhandler;
  sigemptyset (&new_action.sa_mask);
  new_action.sa_flags = 0;

  sigaction(SIGALRM, &new_action, NULL); //Initialie signal
  srand(time(NULL));
  alarm(rand() % 11);

  while(1){
    i+=1;
    sleep(1);
    fprintf(stdout, "%d\n", i);
  }

  exit(0);
}
