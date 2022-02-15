#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define COMMON_SIZE 1000
#define Nthread 6
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct _MatrixInfo{
	int rows;
	int columns;
} MatrixInfo;

typedef struct _ThreadParameter{
	int **A_Mat, **B_Mat, **result_Mat;
	int *A_row, *B_col;
	int common, row_idx, col_idx;
	MatrixInfo result_Info;
} Parameter;

void PrintMatrix(MatrixInfo *info, int **mat){
	for(int i = 0; i < info->rows; i++){
		for(int j = 0; j < info->columns; j++){
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}

int **ReadMatrix(char *file_name, MatrixInfo *info){
	FILE *fp = fopen(file_name, "r");
	if(fp == NULL){
		printf("In ReadMatrix func, fopen failed!\n");
		exit(1);
	}

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

	int **mat = (int**)malloc(sizeof(int*)*info->rows);
	for(int i = 0; i < info->rows; i++){
		mat[i] = (int*)malloc(sizeof(int)*info->columns);
	}
	
	fseek(fp, 0, SEEK_SET);
	int temp;
	for(int i = 0; i < info->rows; i++){
		for(int j = 0; j < info->columns; j++){
			fscanf(fp, "%d", &temp);
			mat[i][j] = temp;
		}
	}
	
	fclose(fp);
	return mat;
}

void BlockMultiply(Parameter *arg, int ***res_temp, int A_row, int A_col, int B_col, int Ntile){
//printf("==================before==================\n");
//PrintMatrix(&(arg->result_Info), arg->result_Mat);
	/* Input computation value to res_temp array */
	int temp;
	for(int k = A_col; k < A_col+TILE_SIZE; k++){
		for(int i = A_row; i < A_row+TILE_SIZE; i++){
			temp = arg->A_Mat[i][k];
			for(int j = B_col; j < B_col+TILE_SIZE; j++){
				res_temp[Ntile][i-A_row][j-B_col] += temp * arg->B_Mat[k][j];
			}
		}
	}
	
//printf("===================after=================\n");
//PrintMatrix(&(arg->result_Info), arg->result_Mat);
}

void *ThreadFunc(void *arg){
	Parameter *thr_arg = (Parameter*)arg;
	int Ntile = (thr_arg->result_Info.columns/COMMON_SIZE); 
	
	/* prevent result_matrix value's colision */
	int ***res_temp = (int***)malloc(sizeof(int**)*Ntile);
	for(int i = 0; i < Ntile; i++){
		res_temp[i] = (int**)malloc(sizeof(int*)*thr_arg->size);
		for(int j = 0; j < thr_arg->A_row[]; j++){
			res_temp[i][j] = (int*)malloc(sizeof(int)*COMMON_SIZE);
			for(int k = 0; k < thr_arg->B_col[]; k++){
				res_temp[i][j][k] = 0;
			}
		}
	}

	/* Compute Tile-Matrix according each Tile's row/col */
	int A_row, A_col, B_col;
	A_row = thr_arg->row_idx*TILE_SIZE;
	A_col = thr_arg->col_idx*TILE_SIZE;
	for(int i = 0; i < Ntile; i++){
		B_col = i*TILE_SIZE;
		BlockMultiply(thr_arg, res_temp, A_row, A_col, B_col, i);
	}
	
	for(int i = 0; i < Ntile; i++){
		B_col = i*TILE_SIZE;
		for(int j = 0; j < TILE_SIZE; j++){
			pthread_mutex_lock(&mutex_lock);
			for(int k = 0; k < TILE_SIZE; k++){
				thr_arg->result_Mat[j+A_row][k+B_col] += res_temp[i][j][k];
			}
			pthread_mutex_unlock(&mutex_lock);
		}
	}
	
	/* free res_temp */
	for(int i = 0; i < Ntile; i++){
		for(int j = 0; j < thr_arg->size; j++){
			free(res_temp[i][j]);
		}
		free(res_temp[i]);
	}
	free(res_temp);

	return NULL;
}

int main(int argc, char **argv){
	/* check input files */
	if(argc != 3){
		printf("Excution format is [excution file] [Matrix_A file] [Matrix_B file]\n");
		perror("file open");
	}

	/* setting Matrix */
	MatrixInfo A_Info, B_Info;
	int **A_Mat = ReadMatrix(argv[1], &A_Info);
	int **B_Mat = ReadMatrix(argv[2], &B_Info);

	/* check operation is possible */
	if(A_Info.columns != B_Info.rows){
		printf("Operation is impossible! Check Matrix\n");
		exit(1);
	}

	/* setting result Matrix */
	MatrixInfo result_Info;
	result_Info.rows = A_Info.rows;
	result_Info.columns = B_Info.columns;
	int **result_Mat = (int**)malloc(sizeof(int*)*result_Info.rows);
	for(int i = 0; i < result_Info.rows; i++){
		result_Mat[i] = (int*)malloc(sizeof(int)*result_Info.columns);
		for(int j = 0; j < result_Info.columns; j++){
			result_Mat[i][j] = 0;
		}
	}
	
	/* setting clock time */
	struct timespec start, end;
	double excute_time = 0;

	/* make multi-thread */
	int tile_row[Nthread], tile_col[Nthread];
	// setting tile_row[] //
	if(result_Info.rows % Nthread == 0){
		for(int i = 0; i < Nthread; i++){
			tile_row[i] = result_Info.rows/Nthread;
		}
	}
	else{
		int rest = result_Info.rows;
		for(int i = 0; i < Nthread-1; i++){
			tile_row[i] = result_Info.rows/Nthread;
			rest -= tile_row[i];
		}
		tile_row[Nthread-1] = rest;
	}
	// setting tile_col[] //
	if(result_Info.columns % Nthread == 0){
		for(int i = 0; i < Nthread; i++){
			tile_col[i] = result_Info.columns/Nthread;
		}
	}
	else{
		int rest = result_Info.columns;
		for(int i = 0; i < Nthread-1; i++){
			tile_col[i] = result_Info.columns/Nthread;
			rest -= tile_col[i];
		}
		tile_col[Nthread-1] = rest;
	}

	pthread_t *multi_thread = (pthread_t*)malloc(sizeof(pthread_t)*Nthread);
	Parameter *parameter = (Parameter*)malloc(sizeof(Parameter)*Nthread);

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int i = 0; i < Nthread; i++){
		int rc;
		parameter[i].A_Mat = A_Mat;
		parameter[i].B_Mat = B_Mat;
		parameter[i].result_Mat = result_Mat;
		parameter[i].result_Info = result_Info;
		parameter[i].common = A_Info.columns;
		parameter[i].A_row = tile_row;
		parameter[i].B_col = tile_col;
//		parameter[i].row_idx = i/tile_col;
//		parameter[i].col_idx = i%tile_col;
		if((rc = pthread_create(&(multi_thread[i]), NULL, 
						MatrixMultiply, (void*)(parameter+i))) != 0){
			perror("Thread create error");
			exit(1);
		}
	}

	void *thr_ret;
	for(int i = 0; i < Nthread; i++){
		int rc;
		if((rc = pthread_join(multi_thread[i], &thr_ret)) != 0){
			perror("Thread join failed!\n");
			exit(1);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	excute_time = end.tv_sec - start.tv_sec;
	printf("Multi-Threading excution time: %f\n", excute_time);

	/* write the multiply result to file */
	FILE *res_file = fopen("Result_Optimize", "w");
	if(res_file == NULL){
		printf("Result_Optimize fopen failed!\n");
		exit(1);
	}
	for(int i = 0; i < result_Info.rows; i++){
		for(int j = 0; j < result_Info.columns; j++){
			fprintf(res_file, " %d ", result_Mat[i][j]);
		}
		fprintf(res_file, "\n");
	}
	fclose(res_file);

	/* free malloc memory */
	for(int i = 0; i < A_Info.rows; i++){
		free(A_Mat[i]);
	}
	free(A_Mat);
	for(int i = 0; i < B_Info.rows; i++){
		free(B_Mat[i]);
	}
	free(B_Mat);
	for(int i = 0; i < result_Info.rows; i++){
		free(result_Mat[i]);
	}
	free(result_Mat);
	free(multi_thread);
	free(parameter);

	return 0;
}
