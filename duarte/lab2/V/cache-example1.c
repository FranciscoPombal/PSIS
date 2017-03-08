#include <stdio.h>
#include <stdlib.h>
	int mat[10000][10000];

int main(){
	int i, j;

	for (i=0; i <10000; i++){
		for (j=0; j <10000; j++){
			mat[i][j] = random();
		}
	}

		int count = 0;
	for (i=0; i <10000; i++){
		for (j=0; j <10000; j++){
			if(mat[i][j]%2 ==0)
				count++;
		}
	}
		printf("pair values: %d\n", count);
}