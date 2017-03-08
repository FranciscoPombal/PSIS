#include <stdio.h>
#include <stdlib.h>
int mat1[5000][5000];
int mat2[5000][5000];

int v1[5000];
int v2[5000];

void init_mat_good(int mat[5000][5000]){
	int i, j;
	for (i=0; i <5000; i++){
		for (j=0; j <5000; j++){
			mat[i][j] = random();
		}
	}
}

void init_mat_bad(int mat[5000][5000]){
	int i, j;
	for (j=0; j <5000; j++){
		for (i=0; i <5000; i++){
			mat[i][j] = random();
		}
	}
}
void extract_column(int mat[5000][5000], int v[5000], int i){
	int j;

	for(j=0; j<5000; j++){
		v[j]= mat[i][j];
	}
}
void extract_row(int mat[5000][5000], int v[5000], int j){
	int i;

	for(i=0; i<5000; i++){
		v[i]= mat[i][j];
	}
}

int mult_vect(int v1[5000],int v2[5000]){
	int res = 0;
	int i;

	for(i=0; i < 5000; i++)
		res += v1[i]*v2[i];
	return res;
}
int main(){
	
	init_mat_good(mat1);
	init_mat_bad(mat2);

	extract_row(mat1, v1, 134);
	extract_column(mat2, v2, 1340);

	printf("result %d\n", mult_vect(v1, v2));

}