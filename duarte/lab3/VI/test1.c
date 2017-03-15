#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <string.h>

int main(int argc, char const *argv[]) {

  char command[1000];
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  pid_t pid;


  //Open script
  FILE* script = NULL;
  printf("%s\n", argv[1]);
  char* buffer;
  buffer = (char*)malloc(1024*sizeof(char));

  script = fopen(argv[1],"r");
  if(script == NULL)
    {
        printf("\n fopen() Error!!!\n");
        return 1;
    }
  printf("\nfopen() Sucess\n");

  //Read and execute command from script
  while (!(feof(script))) {

    //Read a line from script
    getline(&line, &len, script);
    //Create another process for the new command
    pid = fork();

    if(pid==0){ //Child process (New command)
      execl("/bin/sh", script, (char  *) NULL);
      exit(0);
    }
    else{ //Parent
      wait();
    }

  }
  fclose(script);
  exit(0);
}
