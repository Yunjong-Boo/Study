#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Node{
	struct _Node *next;
	int data;
} Node;

typedef struct _List{
	Node *head;
	int size;
} List;

void add_Node(List *list, int val){
	/* initialize new Node  */
	Node *new = (Node*)malloc(sizeof(Node));
	new->next = NULL;
	new->data = val;

	/* find idx where the data add & add to list */
	Node *cur = list->head;
	if(list->head == NULL){
		list->head = new;
	}
	else{
		while(cur->next != NULL){
			cur = cur->next;
		}
		cur->next = new;
	}

	list->size++;
}

void delete_List(List *list){
	Node *cur = list->head;

	for(int i = 0; i < list->size; i++){
		list->head = list->head->next;
		free(cur);
		cur = list->head;
	}
	free(list);
}

void print_Node(List *list){
	printf("--------------print node(size: %d)-----------------\n", list->size);
	Node *cur = list->head;
	for(int i = 0; i < list->size; i++){
		printf("%3d", cur->data);
		cur = cur->next;
	}
	printf("\n---------------------------------------------------\n");
	free(cur);
}

int main(){
	/* get list size & make linkedlist using malloc */
	int list_size;
	printf("Please input list size: ");
	scanf("%d", &list_size);

	/* make head Node & link other Nodes*/
	srand(time(NULL));
	List *list = (List*)malloc(sizeof(List));
	list->head = NULL;
	list->size = 0;
	
	for(int i = 0; i < list_size; i++){
		add_Node(list, (rand()%10));
	}
	print_Node(list);

	/* add list until exit cmd */
	int todo;
	while(1){
		printf("\nChoose what you want to do!\n");
		printf("1. add \t 2. exit\n");
		printf("Please choose: ");
		scanf("%d", &todo);

		if(todo == 1){
			printf("===============ADD==============\n");
			add_Node(list, (rand()%10));
			print_Node(list);
		}
		else if(todo == 2)	break;
		else{
			printf("Invalid input! Please choose valid input!\n");
		}
	}

	/* free all Node in list */
	delete_List(list);

	return 0;
}
