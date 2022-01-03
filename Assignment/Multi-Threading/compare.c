#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

typedef struct _Info{
	int **a, **b, **re;
	int common, row_idx, column_idx;
} Info;

int **readfile(FILE *fp, char *file_name, int *rows, int *columns){
	fp = fopen(file_name, "r");

	char *line;
	size_t len = 0;
	ssize_t nread;
	while((nread = getline(&line, &len, fp)) != -1){
		int length = strlen(line);
		*columns = (length)/2;
		(*rows)++;
	}
	free(line);

	int **mat = (int**)malloc(sizeof(int*)*(*rows));
	for(int i = 0; i < *rows; i++){
		mat[i] = (int*)malloc(sizeof(int)*(*columns));
	}

	fseek(fp, 0, SEEK_SET);
	int temp;
	for(int i = 0; i < *rows; i++){
		for(int j = 0; j < *columns; j++){
			fscanf(fp, "%d", &temp);
			mat[i][j] = temp;
		}
	}

	fclose(fp);
	return mat;
}

void calculate(Info info){
	int sum;
	for(int i = 0; i < info.column_idx; i++){
		sum = 0;
		for(int j = 0; j < info.common; j++){
			sum += info.a[info.row_idx][j] * info.b[j][i];
		}
		info.re[info.row_idx][i] = sum;
	}
}

int main(int argc, char **argv){
	FILE *afp = NULL, *bfp = NULL;
	int arows, acolumns, brows, bcolumns;

	int **a_mat = readfile(afp, argv[1], &arows, &acolumns);
	int **b_mat = readfile(bfp, argv[2], &brows, &bcolumns);

	int **result = (int**)malloc(sizeof(int*)*arows);
	for(int i = 0; i < arows; i++){
		result[i] = (int*)malloc(sizeof(int)*bcolumns);
	}

	struct timespec start, end;
	Info *info = (Info*)malloc(sizeof(Info)*arows);

	clock_gettime(CLOCK_MONOTONIC, &start);
	for(int i = 0; i < arows; i++){
		info[i].a = a_mat;
		info[i].b = b_mat;
		info[i].re = result;
		info[i].common = brows;
		info[i].row_idx = i;
		info[i].column_idx = bcolumns;
		calculate(info[i]);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);

	double excute_time = end.tv_sec - start.tv_sec;
	printf("excution time: %f\n", excute_time);
	
	FILE *out = fopen("compare_result", "w");
	for(int i = 0; i < arows; i++){
		for(int j = 0; j < bcolumns; j++){
			fprintf(out, "  %d  ", result[i][j]);
//			printf("%d ", result[i][j]);
		}
		fprintf(out, "\n");
//		printf("\n");
	}
	fclose(out);

	/* free malloc memory */
	for(int i = 0; i < arows; i++){
		free(a_mat[i]);
	}
	free(a_mat);
	for(int i = 0; i < brows; i++){
		free(b_mat[i]);
	}
	free(b_mat);
	for(int i = 0; i < arows; i++){
		free(result[i]);
	}
	free(result);
	free(info);

	return 0;
}
