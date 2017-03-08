#include <stdio.h>
#include <stdlib.h>
int mat[10000][10000];

int main(){
	int i, j;

	for (j=0; j <10000; j++){
		for (i=0; i <10000; i++){
			mat[i][j] = random();
		}
	}

	int count = 0;
	for (j=0; j <10000; j++){
		for (i=0; i <10000; i++){
			if(mat[i][j]%2 ==0)
				count++;
		}
	}
	printf("pair values: %d\n", count);
}