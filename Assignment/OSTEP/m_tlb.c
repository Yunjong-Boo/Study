#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	/* Check excution format */
	if(argc != 3){
		perror("Check excution format: [excutable file] [the number of pages] [the number of trials]");
		exit(1);
	}

	/* Setting excution param to int */
	int num_pages = atoi(argv[1]);
	int num_trials = atoi(argv[2]);
	
	/* Get pagesize & Make arr */
	int page_size = getpagesize();
	int jump = page_size/sizeof(int);
	int *arr = (int*)malloc(sizeof(int)*(num_pages*page_size));

	/* Access page & Time measurement */
	struct timespec start, end;
	if(clock_gettime(CLOCK_MONOTONIC, &start)){
		printf("clock_gettime start fail!\n");
		exit(1);
	}
	for(int i = 0; i < num_trials; i++){
		for(int j = 0; j < jump*num_pages; j += jump){
			arr[j] += 1;
		}
	}
	if(clock_gettime(CLOCK_MONOTONIC, &end)){
		printf("clock_gettime end fail\n");
		exit(1);
	}

	/* Get average time(nanosec) & Print [num_pages] [avet] */
	float avet = (float)((end.tv_sec-start.tv_sec)*1E9 + (end.tv_nsec-start.tv_nsec))/(num_trials*num_pages);
	printf("%d %f\n",num_pages, avet);
	
	/* free memory */
	free(arr);
	return 0;
}
