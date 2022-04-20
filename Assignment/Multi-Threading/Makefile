all: create multi-thread-block multi-thread-tile optimize optimize-queue optimize-mqueue

create: create_matrix.c
	gcc -o create create_matrix.c -Wall -Werror
multi-thread-block: matrix_multiply_block.c
	gcc -o multi-thread-block matrix_multiply_block.c -lpthread -Wall -Werror
multi-thread-tile: matrix_multiply_tile.c
	gcc -o multi-thread-tile matrix_multiply_tile.c -lpthread -Wall -Werror
optimize: Optimize.c
	gcc -o optimize Optimize.c -lpthread -Wall -Werror
optimize-queue: Optimize_Queue.c
	gcc -o optimize-queue -O2 Optimize_Queue.c -lpthread -Wall -Werror
optimize-mqueue: Optimize_MQueue.c
	gcc -o optimize-mqueue -O2 Optimize_MQueue.c -lpthread -Wall -Werror
clean:
	rm create multi-thread-block multi-thread-tile optimize optimize-queue optimize-mqueue Matrix_A Matrix_B Result_Block Result_Tile Result_Optimize Result_Queue Result_BQueue
