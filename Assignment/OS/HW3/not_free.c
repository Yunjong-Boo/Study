#include <stdio.h>
#include <stdlib.h>

int main(){
	int *test = (int*)malloc(sizeof(int));
	*test = 10;

	return 0;
}
