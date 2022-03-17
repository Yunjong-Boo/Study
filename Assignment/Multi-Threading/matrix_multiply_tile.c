#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define TILE_SIZE 1000
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct _MatrixInfo{
	int rows;
	int columns;
} MatrixInfo;

typedef struct _ThreadParameter{
	int **A_Mat, **B_Mat, **result_Mat;
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

void BlockMultiply(Parameter *arg, int **res_temp, int A_row, int A_col, int B_col){
//printf("==================before==================\n");
//PrintMatrix(&(arg->result_Info), arg->result_Mat);
	/* Input computation value to res_temp array */
	int temp;
	for(int k = A_col; k < A_col+TILE_SIZE; k++){
		for(int i = A_row; i < A_row+TILE_SIZE; i++){
			temp = arg->A_Mat[i][k];
			for(int j = B_col; j < B_col+TILE_SIZE; j++){
				res_temp[i-A_row][j-B_col] += temp * arg->B_Mat[k][j];
			}
		}
	}
	
	/* Setting Mutex to prevent result_Mat's value colision */
	for(int i = 0; i < TILE_SIZE; i++){
		pthread_mutex_lock(&mutex_lock);
		for(int j = 0; j < TILE_SIZE; j++){
//			pthread_mutex_lock(&mutex_lock);
			arg->result_Mat[i+A_row][j+B_col] += res_temp[i][j];
			res_temp[i][j] = 0;
//			pthread_mutex_unlock(&mutex_lock);
		}
		pthread_mutex_unlock(&mutex_lock);
	}
//printf("===================after=================\n");
//PrintMatrix(&(arg->result_Info), arg->result_Mat);
}

void *MatrixMultiply(void *arg){
	Parameter *thr_arg = (Parameter*)arg;
	
	/* prevent result_matrix value's colision */
	int **res_temp = (int**)malloc(sizeof(int*)*TILE_SIZE);
	for(int i = 0; i < TILE_SIZE; i++){
		res_temp[i] = (int*)malloc(sizeof(int)*TILE_SIZE);
		for(int j = 0; j < TILE_SIZE; j++){
			res_temp[i][j] = 0;
		}
	}

	/* Compute Tile-Matrix according each Tile's row/col */
	int A_row, A_col, B_col;
	A_row = thr_arg->row_idx*TILE_SIZE;
	A_col = thr_arg->col_idx*TILE_SIZE;
	for(int i = 0; i < (thr_arg->result_Info.columns/TILE_SIZE); i++){
		B_col = i*TILE_SIZE;
		BlockMultiply(thr_arg, res_temp, A_row, A_col, B_col);
	}
	
	/* free res_temp */
	for(int i = 0; i < TILE_SIZE; i++){
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
	int tile_row, tile_col, Nthread;
	tile_row = result_Info.rows/TILE_SIZE;
	tile_col = result_Info.columns/TILE_SIZE;
	Nthread = tile_row*tile_col;
	pthread_t *multi_thread = (pthread_t*)malloc(sizeof(pthread_t)*Nthread);
	Parameter *parameter = (Parameter*)malloc(sizeof(Parameter)*Nthread);
printf("tile size: %d\n", TILE_SIZE);
printf("Nthread: %d\n", Nthread);

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int i = 0; i < Nthread; i++){
		int rc;
		parameter[i].A_Mat = A_Mat;
		parameter[i].B_Mat = B_Mat;
		parameter[i].result_Mat = result_Mat;
		parameter[i].result_Info = result_Info;
		parameter[i].common = A_Info.columns;
		parameter[i].row_idx = i/tile_col;
		parameter[i].col_idx = i%tile_col;
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
	FILE *res_file = fopen("Result_Tile", "w");
	if(res_file == NULL){
		printf("Result_Tile fopen failed!\n");
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
