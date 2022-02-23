#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define TILE_SIZE 256 	//Change according to L1 Cache Memory Size
#define Nthread 12	//Change according to number of CPU core

typedef struct _MatrixInfo{
	int rows;
	int columns;
	int matrix_size;
} MatrixInfo;

typedef struct _ThreadParameter{
	int Ntile;
	int *A_Mat, *B_Mat, *res_Mat;
	MatrixInfo res_Info;
//A_Info, B_Info;
} Parameter;

void PrintMatrix(int *mat, MatrixInfo *info){
	for(int i = 0; i < info->rows; i++){
		for(int j = 0; j < info->columns; j++){
			printf("%d ", mat[i*info->rows + j]);
		}
		printf("\n");
	}
}

int *ReadMatrix(char *file_name, MatrixInfo *info){
printf("In ReadMatrix\n");
	/* Open & Checking file */
	FILE *fp = fopen(file_name, "r");
	if(fp == NULL){
		printf("In ReadMatrix func, fopen failed!\n");
		exit(1);
	}
	
	/* Setting MatrixInfo */
	char *read_line = NULL;
	size_t len = 0;
	ssize_t nread;
	info->rows = 0;
	while((nread = getline(&read_line, &len, fp)) != -1){
		int length = strlen(read_line);
		info->columns = (length)/2;
		info->rows++;
	}
	free(read_line);
printf("finish matrixinfo\n");
	/* Allocating value to array */
	info->matrix_size = info->rows*info->columns;
	int *mat = (int*)malloc(sizeof(int)*info->matrix_size);
printf("matrix_size: %d\n", info->matrix_size);
printf("finish malloc mat\n");
	fseek(fp, 0, SEEK_SET);
	int temp;
	for(int i = 0; i < info->rows; i++){
		for(int j = 0; j < info->columns; j++){
			fscanf(fp, "%d", &temp);
			mat[i*info->rows + j] = temp;
		}
	}
	
	fclose(fp);
	return mat;
}

int ModifyGCD(int rows, int columns){
printf("in ModifyGCD\n");
	int ret = 0, temp;
	for(int i = 1; i<= rows && i<= columns; i++){
		if(((rows%i == 0) && (columns%i == 0)) == 0){
			temp = i;
			if(abs(TILE_SIZE-temp) <= abs(TILE_SIZE-ret)){
				ret = temp;
			}
			else	break;
		}
	}

	return ret;
}
/*
void *ThreadFunc(void *arg){

}
*/
int main(int argc, char **argv){
	/* Checking Excution Format */
	if(argc != 3){
		printf("Excution Format is [Excution File] [Matrix_A File] [Matrix_B File]\n");
		exit(1);
	}

	/* Setting Input Matrix */
	MatrixInfo A_Info, B_Info;
	int *A_Mat = ReadMatrix(argv[1], &A_Info);
	int *B_Mat = ReadMatrix(argv[2], &B_Info);

	/* Check operation is possible */
	if(A_Info.columns != B_Info.rows){
		printf("Operation is impossible! Check Matrix\n");
		exit(1);
	}

	/* Setting Result Matrix */
	MatrixInfo res_Info;
	res_Info.rows= A_Info.rows;
	res_Info.columns = B_Info.columns;
	int *res_Mat = (int*)malloc(sizeof(int)*(res_Info.rows*res_Info.columns));
printf("finish setting result_mat\n");
	/* Setting clock time 
	struct timespec start, end;
	double excution_time = 0;
*/
	/* Make multi-thread 
	int tile_row, tile_col;
	tile_row = res_Info.rows/TILE_SIZE;
	tile_col = res_Info.columns/TILE_SIZE;
	pthread_t *multi_thread = (pthread_t*)malloc(sizeof(pthread_t)*Nthread);
	Parameter *thr_par;
*/
	/* Setting number of tiles for each thread */



int a = ModifyGCD(res_Info.rows, res_Info.columns);
printf("ModifyGCD: %d\n", a);

/*
	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int i = 0; i < Nthread; i++){
		int rc;
		thr_par.A_Mat = A_Mat;
		thr_par.B_Mat = B_Mat;
		thr_par.res_Mat = res_Mat;
//		thr_par.A_Info = A_Info;
//		thr_par.B_Info = B_Info;
		thr_par.res_Info = res_Info;

		if((rc = pthread_create(&(multi_thread), NULL, 
						ThreadFunc, (void*)thr_par)) != 0){
			perror("Multi Thread Create Error!");
		}
	}
*/
	/* Write result-matrix to file */
	FILE *res_file = fopen("Result", "w");
	if(res_file == NULL){
		printf("Result file fopen failed!\n");
		exit(1);
	}
	for(int i = 0; i < res_Info.rows; i++){
		for(int j = 0; j < res_Info.columns; j++){
			fprintf(res_file, " %d ", res_Mat[i*res_Info.rows + j]);
		}
		fprintf(res_file, "\n");
	}
	fclose(res_file);

	/* free allocated memory */
	free(A_Mat);
	free(B_Mat);
	free(res_Mat);

	return 0;
}
