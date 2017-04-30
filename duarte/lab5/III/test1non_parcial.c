#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int verify_prime(int);

int verify_prime(int n){
    int i = 0;

        for(i = 2; i < n; i++){
            if((n % i) == 0){
                return 0;
            }
        }

    return 1;
}

int main(int argc, char const *argv[])
{
    int n_child = 0;
    int i = 0;
    int n = 0;
    pid_t pid = 0;
    int pipefd[2], pipechildren[2];
    int n_prime=0, yes=1, ifprime=0;

        if(argc == 1){
            n_child = 0;
        }
        if(argc == 2){
            if(sscanf(argv[1], "%d", &n_child) == 0)
            n_child = 0;
        }

        fprintf(stdout, "number of children %d\n", n_child);

        if(n_child == 0){ //When there is only 1 child
            for(i = 2; i < 99999; i++){
                n = random() % 999999;
                if(verify_prime(n)){
                    fprintf(stdout, "%d is prime\n", n);
                }
            }
        }

        else{  //When there are multiple children
            pipe(pipefd);
            pipe(pipechildren);
            for(i = 0; i < n_child; i++){
                pid = fork();
                if(pid < 0){
                    fprintf(stderr, "fork error.\n");
                    exit(EXIT_FAILURE);
                }else if(pid == 0){ //CHILD
                    //child
                    close(pipefd[1]);
                    close(pipechildren[0]);
                    while(read(pipefd[0], &i, sizeof(int)) > 0){
                        if((yes = verify_prime(i))){

                        }
                        write(pipechildren[1], &yes, sizeof(int));
                    }
                    exit(EXIT_SUCCESS);
                }
            }
            close(pipechildren[1]);
            for(i = 2; i < 99999; i++) { //PARENT
                n = random() % 99999;
                write(pipefd[1], &n, sizeof(int));
                read(pipechildren[0], &ifprime, sizeof(int));
                printf("%d\n", ifprime);
                n_prime += ifprime;
                ifprime=0;
            }

            close(pipefd[1]);

            wait(NULL);
            printf("\nPrime numbers count = %d\n", n_prime);

        }

    return 0;
}
