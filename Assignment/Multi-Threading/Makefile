all: create single-thread multi-thread multi-thread-block multi-thread-tile multi-thread-optimize optimize optimize-queue optimize-bqueue

create: create_matrix.c
	gcc -o create create_matrix.c -Wall -Werror
single-thread: compare.c
	gcc -o single-thread compare.c -Wall -Werror
multi-thread: matrix_multiply.c
	gcc -o multi-thread matrix_multiply.c -lpthread -Wall -Werror
multi-thread-block: matrix_multiply_block.c
	gcc -o multi-thread-block matrix_multiply_block.c -lpthread -Wall -Werror
multi-thread-tile: matrix_multiply_tile.c
	gcc -o multi-thread-tile matrix_multiply_tile.c -lpthread -Wall -Werror
multi-thread-optimize: matrix_multiply_optimize.c
	gcc -o multi-thread-optimize matrix_multiply_optimize.c -lpthread -Wall -Werror
optimize: Optimize.c
	gcc -o optimize Optimize.c -lpthread -Wall -Werror
optimize-queue: Optimize_Queue.c
	gcc -o optimize-queue Optimize_Queue.c -lpthread -Wall -Werror
optimize-bqueue: Optimize_BQueue.c
	gcc -o optimize-bqueue Optimize_BQueue.c -lpthread -Wall -Werror
clean:
	rm create single-thread multi-thread multi-thread-block multi-thread-tile multi-thread-optimize optimize optimize-queue optimize-bqueue Matrix_A Matrix_B compare_result Result_Matrix Result_Block Result_Tile Result Result_Optimize Result_Queue Result_BQueue
