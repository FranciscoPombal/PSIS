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
  alarm(rand() % 11);
  ///signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
  //printf("Hello");
  //signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
}

int main(int argc, char const *argv[]) {

  signal(SIGALRM, ALARMhandler);
  srand(time(NULL));
  alarm(rand() % 11);

  while(1){
    i+=1;
    fprintf(stdout, "%d\n", i);
  }

  exit(0);
}
