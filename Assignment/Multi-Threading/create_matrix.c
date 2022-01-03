#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void create_matrix(FILE *fp, char *file_name){
	printf("-------------Creating Matrix------------\n");
	/* make int array using malloc because array size is too big */
	int rows, columns;
	printf("Please input the rows: ");
	scanf("%d", &rows);
	printf("Please input the columns: ");
	scanf("%d", &columns);
	int **matrix = (int**)malloc(sizeof(int*)*rows);
	for(int i = 0; i < rows; i++){
		matrix[i] = (int*)malloc(sizeof(int)*columns);
	}

	/* allocate value into array using rand func */
	srand(time(NULL));
	if((fp = fopen(file_name, "w")) == NULL){
		perror("file open failed!\n");
		exit(1);
	}
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			matrix[i][j] = rand()%5;
			fprintf(fp, "%d ", matrix[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	/* free malloc memory */
	for(int i = 0; i < rows; i++){
		free(matrix[i]);
	}
	free(matrix);
}

int main(){
	FILE *afp = NULL, *bfp = NULL;
	create_matrix(afp, "Matrix_A");
	create_matrix(bfp, "Matrix_B");

	return 0;
}
