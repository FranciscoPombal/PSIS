#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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

        if(argc == 1){
            n_child = 0;
        }
        if(argc == 2){
            if(sscanf(argv[1], "%d", &n_child) == 0)
            n_child = 0;
        }

        fprintf(stdout, "number of children %d\n", n_child);

        if(n_child == 0){
            int i, n;
            for(i = 2; i < 99999; i++){
                n = random() % 999999;
                if(verify_prime(n)){
                    fprintf(stdout, "%d is prime\n", n);
                }
            }
        }else{
            //create n_clid processes

            //generate random numbers
            //send those numbers to the child processes

        }

    return 0;
}
