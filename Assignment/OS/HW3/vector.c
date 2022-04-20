#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_arr(int *arr, int size){
	printf("-----------print arr(size: %d)-------------\n", size);
	for(int i = 0; i < size; i++){
		printf("%3d", arr[i]);
	}
	printf("\n-------------------------------------------\n");
}

int main(){
	/* get arr size & make arr using malloc */
	int arr_size;
	printf("Please input arr size: ");
	scanf("%d", &arr_size);
	int *arr = (int*)malloc(sizeof(int)*arr_size);

	/* initialize arr using rand func */
	srand(time(NULL));
	for(int i = 0; i < arr_size; i++){
		arr[i] = rand()%10;
	}
	print_arr(arr, arr_size);

	/* add arr until exit cmd */
	int todo;
	while(1){
		printf("\nChoose what you want to do!\n");
		printf("1. add \t 2. exit\n");
		printf("Please choose: ");
		scanf("%d", &todo);
		
		if(todo == 1){
			printf("==============ADD=============\n");
			arr_size++;
			arr = (int*)realloc(arr, sizeof(int)*arr_size);
			arr[arr_size-1] = rand()%10;
			print_arr(arr, arr_size);
		}
		else if(todo == 2)	break;
		else{
			printf("Invalid input! Please choose valid input!\n");
		}
	}

	free(arr);
	return 0;
}
