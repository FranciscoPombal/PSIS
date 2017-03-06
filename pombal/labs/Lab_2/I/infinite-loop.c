#include <stdio.h>
#include <stdlib.h>


int main(){
	int v[100];
	int i;

    // Why does the program work with a while loop and not a for loop?
    while(i<100){
        v[i] = random();
        i++;
    }

    /*
	for (i=0; i<100; i+1){
		v[i] = random();
    }
    */

	printf("vector initialized\n");

}
