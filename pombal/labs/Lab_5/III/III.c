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
    int pipefd_ctp[2];  // pipe file descriptors for child -> parent communication
    int pipefd_ptc[2];  // pipe file descriptors for parent -> child communication
    unsigned long long int partial_count = 0;
    unsigned long long int total_count = 0;

        // determine number of child processes
        if(argc == 1){
            n_child = 0;
        }
        if(argc == 2){
            if(sscanf(argv[1], "%d", &n_child) == 0)
            n_child = 0;
        }
        fprintf(stdout, "number of children %d\n", n_child);

        if(n_child == 0){
            // normal sequential processing
            for(i = 2; i < 99999; i++){
                n = random() % 999999;
                if(verify_prime(n)){
                    fprintf(stdout, "%d is prime\n", n);
                }
            }
        }else{
            // pipe before creating child processes, so that parent and all children have access to the file descriptors
            pipe(pipefd_ctp);
            pipe(pipefd_ptc);
            //create n_clid processes
            for(i = 0; i < n_child; i++){
                pid = fork();
                if(pid < 0){
                    // error
                    fprintf(stderr, "fork error.\n");
                    exit(EXIT_FAILURE);
                }else if(pid == 0){
                    // child
                    // close the write end of parent -> child
                    close(pipefd_ptc[1]);
                    // close the read end of child -> parent
                    close(pipefd_ctp[0]);
                    // read data from parent
                    while(read(pipefd_ptc[0], &i, sizeof(int)) > 0){
                        if(verify_prime(i)){
                            fprintf(stdout, "%d is prime\n", i);
                            partial_count++;
                        }
                    }
                    // write partial count to parent
                    write(pipefd_ctp[1], &partial_count, sizeof(unsigned long long int));
                    // close write end of child -> parent pipe
                    close(pipefd_ctp[1]);
                    // close the read end of parent -> child
                    close(pipefd_ptc[0]);
                    exit(EXIT_SUCCESS);
                }
            }

            // parent
            // close read end of parent -> child pipe
            close(pipefd_ptc[0]);
            for(i = 2; i < 999999; i++) {
                n = random() % 999999;
                // write data to child
                write(pipefd_ptc[1], &n, sizeof(int));
            }
            // close write end of parent -> child pipe
            close(pipefd_ptc[1]);

            // close write end of child -> parent pipe
            close(pipefd_ctp[1]);
            // read data from child
            while(read(pipefd_ctp[0], &partial_count, sizeof(unsigned long long int)) > 0){
                total_count += partial_count;
            }
            // close read end of child -> parent pipe
            close(pipefd_ctp[0]);

            fprintf(stdout, "Total number of primes: %llu\n", total_count);

            wait(NULL);

        }

    return 0;
}
