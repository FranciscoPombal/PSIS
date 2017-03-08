#include <stdio.h>
#include <stdlib.h>
static int mat1[5000][5000];
static int mat2[5000][5000];

static int v1[5000];
static int v2[5000];

void init_mat_good(int[5000][5000]);
void init_mat_bad(int[5000][5000]);
void extract_column(int[5000][5000], int[5000], int);
void extract_row(int[5000][5000], int[5000], int);
int mult_vect(int[5000],int[5000]);

void init_mat_good(int mat[5000][5000]){
	int i, j;
	for (i=0; i <5000; i++){
		for (j=0; j <5000; j++){
			mat[i][j] = (int)random();
		}
	}
}

void init_mat_bad(int mat[5000][5000]){
	int i, j;
	for (j=0; j <5000; j++){
		for (i=0; i <5000; i++){
			mat[i][j] = (int)random();
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

int mult_vect(int va[5000],int vb[5000]){
	int res = 0;
	int i;

	for(i=0; i < 5000; i++)
		res += va[i]*vb[i];
	return res;
}

int main(){

	init_mat_good(mat1);
	init_mat_bad(mat2);

	extract_row(mat1, v1, 134);
	extract_column(mat2, v2, 1340);

	printf("result %d\n", mult_vect(v1, v2));

}
