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
    int pipefd[2];
    int pipefd2[2];
    unsigned long long int partial_count = 0;
    unsigned long long int sum = 0;

        if(argc == 1){
            n_child = 0;
        }
        if(argc == 2){
            if(sscanf(argv[1], "%d", &n_child) == 0)
            n_child = 0;
        }

        fprintf(stdout, "number of children %d\n", n_child);

        if(n_child == 0){
            for(i = 2; i < 99999; i++){
                n = random() % 999999;
                if(verify_prime(n)){
                    fprintf(stdout, "%d is prime\n", n);
                }
            }
        }else{
            //create n_clid processes
            pipe(pipefd);
            pipe(pipefd2);
            for(i = 0; i < n_child; i++){
                pid = fork();
                if(pid < 0){
                    fprintf(stderr, "fork error.\n");
                    exit(EXIT_FAILURE);
                }else if(pid == 0){
                    //child
                    close(pipefd2[0]);
                    while(read(pipefd[0], &i, sizeof(int)) > 0){
                        if(verify_prime(i)){
                            fprintf(stdout, "%d is prime\n", i);
                            partial_count++;
                        }
                    }
                    write(pipefd2[1], &partial_count, sizeof(unsigned long long int));
                    close(pipefd2[1]);
                    exit(EXIT_SUCCESS);
                }
            }

            for(i = 2; i < 999999; i++) {
                n = random() % 999999;
                write(pipefd[1], &n, sizeof(int));
            }
            close(pipefd[0]);
            close(pipefd[1]);
            while(read(pipefd2[0], &partial_count, sizeof(unsigned long long int)) > 0){
                sum += partial_count;
            }

            fprintf(stdout, "Total number of primes: %llu\n", sum);

            close(pipefd2[0]);
            close(pipefd2[1]);

            wait(NULL);

        }

    return 0;
}
