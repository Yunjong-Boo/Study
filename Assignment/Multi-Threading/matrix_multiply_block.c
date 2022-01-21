#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define BLOCK_SHORT_SIDE 1000

typedef struct _MatrixInfo{
	int rows;
	int columns;
} MatrixInfo;

typedef struct _ThreadParameter{
	int **A_Mat, **B_Mat, **result_Mat;
	int common, block_row;
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

void BlockMultiply(Parameter *arg, int A_block_row, int B_block_col){
	int A_row , B_col, temp;
	A_row = A_block_row*BLOCK_SHORT_SIDE;
	B_col = B_block_col*BLOCK_SHORT_SIDE;
	for(int k = 0; k < arg->common; k++){
		for(int i = A_row; i < A_row+BLOCK_SHORT_SIDE; i++){
			temp = arg->A_Mat[i][k];
			for(int j = B_col; j < B_col+BLOCK_SHORT_SIDE; j++){
				arg->result_Mat[i][j] += temp * arg->B_Mat[k][j];
			}
		}
	}
}

void *MatrixMultiply(void *arg){
	Parameter *thr_arg = (Parameter*)arg;
	for(int i = 0; i < (thr_arg->result_Info.columns/BLOCK_SHORT_SIDE); i++){
		BlockMultiply(thr_arg, thr_arg->block_row, i);
	}

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
	int Nthread;
	Nthread = A_Info.rows/BLOCK_SHORT_SIDE;
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
		parameter[i].block_row = i;
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
	FILE *res_file = fopen("Result_Block", "w");
	if(res_file == NULL){
		printf("Result_Modify fopen failed!\n");
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
