#include <stdio.h>
#include <stdlib.h>


int main(){
	int v[100];
	int i=0;

	while(i<100){
		v[i] = random();
		i++;
	}

	printf("vector initialized\n");

}
