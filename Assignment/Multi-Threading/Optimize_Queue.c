#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define STANDARD_SIZE 256 	//Change according to L1 Cache Memory Size
#define Nthread 12	//Change according to number of CPU core
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct _Node{
	struct _Node *next;
	int tile_id;
} Node;

typedef struct _Queue{
	Node *front;
	Node *rear;
	int size;
} Queue;

typedef struct _MatrixInfo{
	int rows;
	int columns;
} MatrixInfo;

typedef struct _TileInfo{
	int Ntile;
	int size, rows, columns, common;
} TileInfo;

typedef struct _ThreadParameter{
	int **A_Mat, **B_Mat, **res_Mat;
	MatrixInfo res_Info;
	TileInfo tile_Info;
	Queue *queue;
} Parameter;

void PrintMatrix(int **mat, MatrixInfo *info){
	for(int i = 0; i < info->rows; i++){
		for(int j = 0; j < info->columns; j++){
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}

int **ReadMatrix(char *file_name, MatrixInfo *info){
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

	/* Allocating value to array */
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

int ModifyGCD(int rows, int columns){
	int ret = 0, temp;
	for(int i = 1; i<= rows && i<= columns; i++){
		if((rows%i == 0) && (columns%i == 0)){
			temp = i;
			if(abs(STANDARD_SIZE-temp) <= abs(STANDARD_SIZE-ret)){
				ret = temp;
			}
			else	break;
		}
	}

	return ret;
}

void initQueue(Queue *queue){
	queue->front = queue->rear = NULL;
	queue->size = 0;
}

void enqueue(Queue *queue, int idx){
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->tile_id = idx;
	newNode->next = NULL;

	/* if queue is empty */
	if(queue->size == 0)	queue->front = newNode;
	/* if queue is not empty */
	else	queue->rear->next = newNode;
	queue->rear = newNode;
	queue->size++;
}

int dequeue(Queue *queue){
	/* Check queue is empty */
	if(queue->size == 0){
		printf("in dequeue func queue is empty!\n");
		exit(1);
	}

	Node *temp = queue->front;
	int ret = temp->tile_id;
	queue->front = temp->next;
	queue->size--;
	free(temp);

	return ret;
}

void MatrixMultiply(Parameter *arg, int *A_sub, int A_row_idx, int common_idx, int B_col_idx, int *res_temp){

	int *B_sub = (int*)malloc(sizeof(int)*(arg->tile_Info.size*arg->tile_Info.size));
	for(int i = 0; i < arg->tile_Info.size; i++){
		for(int j = 0; j < arg->tile_Info.size; j++){
			B_sub[i*arg->tile_Info.size+j] = arg->B_Mat[common_idx+i][B_col_idx+j];
		}
	}

	int temp;
	for(int k = 0; k < arg->tile_Info.size; k++){
		for(int i = 0; i < arg->tile_Info.size; i++){
			temp = A_sub[(i*arg->tile_Info.size)+k];
			for(int j = 0; j < arg->tile_Info.size; j++){
				res_temp[(i*arg->tile_Info.size)+j] += 
					temp * B_sub[(k*arg->tile_Info.size)+j];
			}
		}
	}

	/* Using mutex to prevent resul_Mat's value colision */
	for(int i = 0; i < arg->tile_Info.size; i++){
		pthread_mutex_lock(&mutex_lock);
		for(int j = 0; j < arg->tile_Info.size; j++){
			arg->res_Mat[A_row_idx+i][B_col_idx+j] += 
					res_temp[i*arg->tile_Info.size+j];
			res_temp[i*arg->tile_Info.size+j] = 0;
		}
		pthread_mutex_unlock(&mutex_lock);
	}

	free(B_sub);
}

void *ThreadFunc(void *thr_par){
	Parameter *arg = (Parameter*)thr_par;

	/* Setting */
	int *A_sub = (int*)malloc(sizeof(int)*(arg->tile_Info.size*arg->tile_Info.size));
	int *res_temp = (int*)malloc(sizeof(int)*(arg->tile_Info.size*arg->tile_Info.size));
	int tile_idx;

	while(1){
		pthread_mutex_lock(&mutex_lock);
		if(arg->queue->size == 0)	break;
		tile_idx = dequeue(arg->queue);
		pthread_mutex_unlock(&mutex_lock);

//printf("check enqueue is success: %d\n", tile_idx);

		int A_row_idx = (tile_idx/arg->tile_Info.common)*arg->tile_Info.size;
		int common_idx = (tile_idx%arg->tile_Info.common)*arg->tile_Info.size;
		/* Allocating A_Matrix's sub matrix */
		for(int i = 0; i < arg->tile_Info.size; i++){
			for(int j = 0; j < arg->tile_Info.size; j++){
				A_sub[(i*arg->tile_Info.size)+j] = 
						arg->A_Mat[A_row_idx+i][common_idx+j];
			}
		}

		for(int i = 0 ; i < arg->tile_Info.columns; i++){
			int B_col_idx = i*arg->tile_Info.size;
			MatrixMultiply(arg, A_sub, A_row_idx, common_idx, B_col_idx, res_temp);
		}
	}
	pthread_mutex_unlock(&mutex_lock);

	/* free allocated memory */
	free(A_sub);
	free(res_temp);

	return NULL;
}

int main(int argc, char **argv){
	/* Checking Excution Format */
	if(argc != 3){
		printf("Excution Format is [Excution File] [Matrix_A File] [Matrix_B File]\n");
		exit(1);
	}

	/* Setting Input Matrix */
	MatrixInfo A_Info, B_Info;
	int **A_Mat = ReadMatrix(argv[1], &A_Info);
	int **B_Mat = ReadMatrix(argv[2], &B_Info);

	/* Check operation is possible */
	if(A_Info.columns != B_Info.rows){
		printf("Operation is impossible! Check Matrix\n");
		exit(1);
	}

	/* Setting Result Matrix */
	MatrixInfo res_Info;
	res_Info.rows= A_Info.rows;
	res_Info.columns = B_Info.columns;
	int **res_Mat = (int**)malloc(sizeof(int*)*res_Info.rows);
	for(int i = 0; i < res_Info.rows; i++){
		res_Mat[i] = (int*)malloc(sizeof(int)*res_Info.columns);
	}
	
	/* Setting clock time */
	struct timespec start, end;
	double excution_time = 0;

	/* Preparation process to divide Matrix into Tiles */
	TileInfo tile_Info;
	tile_Info.size = ModifyGCD(res_Info.rows, res_Info.columns);
	tile_Info.rows = res_Info.rows/tile_Info.size;
	tile_Info.columns = res_Info.columns/tile_Info.size;
	tile_Info.common = A_Info.columns/tile_Info.size;
	tile_Info.Ntile = tile_Info.rows*tile_Info.columns;

	/* Make queue for scheduling */
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	initQueue(queue);
	for(int i = 0; i < tile_Info.Ntile; i++){
		enqueue(queue, i);
	}

	/* Create multi-thread */
	pthread_t *multi_thread = (pthread_t*)malloc(sizeof(pthread_t)*Nthread);
	Parameter *thr_par = (Parameter*)malloc(sizeof(Parameter)*Nthread);
	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int i = 0; i < Nthread; i++){
		int rc;
		thr_par[i].tile_Info = tile_Info;
		thr_par[i].A_Mat = A_Mat;
		thr_par[i].B_Mat = B_Mat;
		thr_par[i].res_Mat = res_Mat;
		thr_par[i].res_Info = res_Info;
		thr_par[i].queue = queue;
		if((rc = pthread_create(&(multi_thread[i]), NULL, 
						ThreadFunc, (void*)(thr_par+i))) != 0){
			perror("Multi Thread Create Error!");
		}
	}

	/* Join multi-thread */
	void *thr_ret;
	for(int i = 0; i < Nthread; i++){
		int rc;
		if((rc = pthread_join(multi_thread[i], &thr_ret)) != 0){
			perror("Multi Thread Join Failed!");
		}
	}

	/* Print excution_time */
	clock_gettime(CLOCK_MONOTONIC, &end);
	excution_time = end.tv_sec - start.tv_sec;
	printf("Multi-Threading Excution Time: %f\n", excution_time);

	/* Write result-matrix to file */
	FILE *res_file = fopen("Result_Queue", "w");
	if(res_file == NULL){
		printf("Result file fopen failed!\n");
		exit(1);
	}
	for(int i = 0; i < res_Info.rows; i++){
		for(int j = 0; j < res_Info.columns; j++){
			fprintf(res_file, " %d ", res_Mat[i][j]);
		}
		fprintf(res_file, "\n");
	}
	fclose(res_file);

	/* free allocated memory */
	for(int i = 0; i < A_Info.rows; i++){
		free(A_Mat[i]);
	}
	free(A_Mat);
	for(int i = 0; i < B_Info.rows; i++){
		free(B_Mat[i]);
	}
	free(B_Mat);
	for(int i = 0; i < res_Info.rows; i++){
		free(res_Mat[i]);
	}
	free(res_Mat);
	free(multi_thread);

	return 0;
}
