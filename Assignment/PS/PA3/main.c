#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Node{
    struct _Node *next;
    int r, c;
} Node;

typedef struct _Queue{
    Node *front;
    Node *rear;
    int size;
} Queue;

void enqueue(Queue *q, int r, int c){
    Node *new = (Node*)malloc(sizeof(Node));
    new->next = NULL;
    new->r = r;
    new->c = c;

    if(q->size == 0)    q->front = new;
    else                q->rear->next = new;    // link with current rear and new
    q->rear = new;  // enqueue new node to rear
    q->size++;
}

int* dequeue(Queue *q){
    int *ret = (int*)malloc(sizeof(int)*2);
    Node *cur;

    if(q->size == 0)    return ret;

    /* get idx for pixel location & change queue front */
    cur = q->front;
    ret[0] = cur->r;
    ret[1] = cur->c;
    q->front = cur->next;

    /* free queue front Node & reduce queue size */
    free(cur);
    q->size--;

    return ret;
}

void print_pixel(int arr[1001][1001], int size){
    for(int i = 1; i < size+1; i++){
        for(int j = 1; j < size+1; j++){
            printf("%5d", arr[i][j]);
        }
        printf("\n");
    }
}

int main(){
    char cmd_type[10];
    int size;
    int pixel[1001][1001];

    while(1){
        scanf("%s", cmd_type);

        if(strcmp(cmd_type, "new") == 0){
            scanf("%d", &size);
            for(int i = 0; i < size+1; i++){
                for(int j = 0; j < size+1; j++){
                    if(i == 0 || j == 0)    pixel[i][j] = -1;
                    else                    pixel[i][j] = 0;
                }
            }
// print_pixel(pixel, size);
        }   
        else if(strcmp(cmd_type, "rect") == 0){
            int r1, c1, r2, c2, color;
            scanf("%d %d %d %d %d", &r1, &c1, &r2, &c2, &color);
            for(int i = r1; i < r2+1; i++){
                for(int j = c1; j < c2+1; j++){
                    pixel[i][j] = color;
                }
            }
// print_pixel(pixel, size);
        }
        else if(strcmp(cmd_type, "fill") == 0){
            int r, c, color;
            scanf("%d %d %d", &r, &c, &color);

            /* visit[][]: check whether visit */
            int **visit = (int**)malloc(sizeof(int*)*(size+1));
            for(int i = 0; i < size+1; i++){
                visit[i] = (int*)malloc(sizeof(int)*(size+1));
                for(int j = 0; j < size+1; j++){
                    visit[i][j] = 0;
                }
            }

            Queue *queue = (Queue*)malloc(sizeof(Queue));
            queue->front = NULL;
            queue->rear = NULL;
            queue->size = 0;
            
            /* using queue */
            int target = pixel[r][c], change_cnt = 0;
            enqueue(queue, r, c);
            visit[r][c] = 1;
            while(queue->size > 0){
                int *idx = dequeue(queue);
                pixel[idx[0]][idx[1]] = color;
                change_cnt++;

                if(idx[0] != 1 && !visit[idx[0]-1][idx[1]] && pixel[idx[0]-1][idx[1]] == target){
                    enqueue(queue, idx[0]-1, idx[1]);
                    visit[idx[0]-1][idx[1]] = 1;
                }
                if(idx[0] != size && !visit[idx[0]+1][idx[1]] && pixel[idx[0]+1][idx[1]] == target){
                    enqueue(queue, idx[0]+1, idx[1]);
                    visit[idx[0]+1][idx[1]] = 1;
                }
                if(idx[1] != 1 && !visit[idx[0]][idx[1]-1] && pixel[idx[0]][idx[1]-1] == target){
                    enqueue(queue, idx[0], idx[1]-1);
                    visit[idx[0]][idx[1]-1] = 1;
                }
                if(idx[1] != size && !visit[idx[0]][idx[1]+1] && pixel[idx[0]][idx[1]+1] == target){
                    enqueue(queue, idx[0], idx[1]+1);
                    visit[idx[0]][idx[1]+1] = 1;
                }
                
                free(idx);
            }
            printf("%d\n", change_cnt);
// print_pixel(pixel, size);
            /* free malloc memory */
            for(int i = 0; i < size+1; i++){
                free(visit[i]);
            }
            free(visit);
        }
        else if(strcmp(cmd_type, "count") == 0){
            /* sorting & check */
            int cnt[256] = {0, };
            int color[256];
            for(int i = 0; i < 256; i++){
                color[i] = i;
            }

            /* count each color */
            for(int i = 1; i < size+1; i++){
                for(int j = 1; j < size+1; j++){
                    cnt[pixel[i][j]]++;
                }
            }

            /* sorting */
            for(int i = 1; i < 256; i++){
                int temp_cnt = cnt[i], temp_color = color[i], j;
                for(j = i-1; j >= 0 && cnt[j] < temp_cnt; j--){
                    cnt[j+1] = cnt[j];
                    color[j+1] = color[j];
                }
                cnt[j+1] = temp_cnt;
                color[j+1] = temp_color;
            }

            /* print */
            for(int i = 0; i < 256 && cnt[i] > 0; i++){
                printf("%d:%d\n", color[i], cnt[i]);
            }
        }
        else if(strcmp(cmd_type, "exit") == 0)   break;
    }

    return 0;
}