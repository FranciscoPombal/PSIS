#include <stdio.h>
#include <stdlib.h>


int main(){
    int v[100];
    int i = 0;

    for (i=0; i<100; i+=1){
    v[i] = random();
    }

    printf("vector initialized\n");

}
