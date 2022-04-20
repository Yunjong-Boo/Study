#include <stdio.h>
#include <stdlib.h>

int main(){
	int *data = (int*)malloc(sizeof(int)*100);

	for(int i = 0; i < 100; i++){
		data[i] = 0;
	}
	free(data);

	printf("print after free: %d\n", data[15]);

	return 0;
}
