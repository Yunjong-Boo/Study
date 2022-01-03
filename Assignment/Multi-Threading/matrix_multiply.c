#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef struct _MatrixInfo{
	int rows;
	int columns;
} MatrixInfo;

typedef struct _ThreadParameter{
	int **A_Mat, **B_Mat, **result_Mat;
	int common, row_idx;
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

void *MatrixMultiply(void *arg){
	Parameter *thr_arg = (Parameter*)arg;
	int sum;
	for(int i = 0; i < thr_arg->result_Info.columns; i++){
		sum = 0;
		for(int j = 0; j < thr_arg->common; j++){
			sum += (thr_arg->A_Mat[thr_arg->row_idx][j]) * (thr_arg->B_Mat[j][i]);
		}
		thr_arg->result_Mat[thr_arg->row_idx][i] = sum;
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
	}

	/* setting clock time */
	struct timespec start, end;
	double excute_time = 0;

	/* make multi-thread */
	pthread_t *multi_thread = (pthread_t*)malloc(sizeof(pthread_t)*result_Info.rows);
	Parameter *parameter = (Parameter*)malloc(sizeof(Parameter)*result_Info.rows);

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int i = 0; i < result_Info.rows; i++){
		int rc;
		parameter[i].A_Mat = A_Mat;
		parameter[i].B_Mat = B_Mat;
		parameter[i].result_Mat = result_Mat;
		parameter[i].result_Info = result_Info;
		parameter[i].common = B_Info.rows;
		parameter[i].row_idx = i;
		if((rc = pthread_create(&(multi_thread[i]), NULL, 
						MatrixMultiply, (void*)(parameter+i))) != 0){
			perror("Thread create error");
			exit(1);
		}
	}

	void *thr_ret;
	for(int i = 0; i < result_Info.rows; i++){
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
	FILE *res_file = fopen("Result_Matrix", "w");
	if(res_file == NULL){
		printf("Result_Matrix fopen failed!\n");
		exit(1);
	}
	for(int i = 0; i < result_Info.rows; i++){
		for(int j = 0; j < result_Info.columns; j++){
			fprintf(res_file, "  %d  ", result_Mat[i][j]);
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
