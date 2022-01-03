all: create single-thread multi-thread

create: create_matrix.c
	gcc -o create create_matrix.c -Wall -Werror
single-thread: compare.c
	gcc -o single-thread compare.c -Wall -Werror
multi-thread: matrix_multiply.c
	gcc -o multi-thread matrix_multiply.c -lpthread -Wall -Werror
clean:
	rm create single-thread multi-thread Matrix_A Matrix_B compare_result Result_Matrix
