#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable : 4996)

typedef struct _Land{
	char terrain;
	int popul;
	int height;
} Land;

typedef struct _PopulIdx{
	int r1, r2, c1, c2;
	int idx;
} PopulIdx;

void print_Land(int N, int M, Land *land){
	printf("====================PrintLand====================\n");
	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			printf("%c %d %d\t\t", land[i*M+j].terrain, land[i*M+j].popul, land[i*M+j].height);
		}
		printf("\n\n");
	}
}

void check_edge(int *check, int N, int M, int j, int k){
	/* Initialize check arr */
	for(int i = 0; i < 4; i++){
		check[i] = 1;
	}

	/* if edge set check[priority] = 0 */
	if(j == 0)		check[3] = 0;
	else if(j == N-1)	check[2] = 0;
	if(k == 0)		check[1] = 0;
	else if(k == M-1)	check[0] = 0;
}

void climb_highest(Land *land, int *check, int temp_popul, int row, int col, int M){
	/* find highest */
	int h_row = row, h_col = col, highest = land[row*M+col].height;
	if(highest < land[row*M+(col+check[0])].height){
		highest = land[row*M+(col+check[0])].height;
		h_row = row;
		h_col = col+check[0];
	}
	if(highest < land[row*M+(col-check[1])].height){
		highest = land[row*M+(col-check[1])].height;
		h_row = row;
		h_col = col-check[1];
	}
	if(highest < land[(row+check[2])*M+col].height){
		highest = land[(row+check[2])*M+col].height;
		h_row = row+check[2];
		h_col = col;
	}
	if(highest < land[(row-check[3])*M+col].height){
		highest = land[(row-check[3])*M+col].height;
		h_row = row-check[3];
		h_col = col;
	}
	
	/* Move people to highest */
	if(!(h_row == row && h_col == col)){
		land[h_row*M+h_col].popul += temp_popul;
		land[row*M+col].popul -= temp_popul;
	}
}

void settle_city(int gap, int N, int M, Land *land, int Nsub, int Msub, int row1, int col1){
	int edge_size = gap*2+1;
	if(edge_size > Nsub && edge_size > Msub){
		/*  */
		land[(row1-1)*M+(col1-1)].terrain = 'C';
		int max = 0;
		for(int i = row1-1; i < Nsub+row1-1; i++){
			for(int j = col1-1; j < Msub+col1-1; j++){
				max += land[i*M+j].popul;
				land[i*M+j].popul = 0;
			}
		}
		land[(row1-1)*M+(col1-1)].popul = max;
	}
	else if(edge_size > Nsub){
		/* Get max & max_idx */
		int max = 0, temp, c;
		for(int i = col1-1; i < Msub-edge_size+col1; i++){
			temp = 0;
			for(int j = i; j < edge_size+i; j++){
				for(int k = row1-1; k < Nsub+row1-1; k++){
					temp += land[k*M+j].popul;
				}
			}
			if(temp > max){
				max = temp;
				c = i;
			}
		}
		
		/* Set land using max_idx */
		land[(row1-1)*M+(gap+c)].terrain = 'C';
		for(int i = c; i < edge_size+c; i++){
			for(int j = row1-1; j < Nsub+row1-1; j++){
				land[j*M+i].popul = 0;
			}
		}
		land[(row1-1)*M+(gap+c)].popul = max;
	}
	else if(edge_size > Msub){
		/* Get max & max_idx */
		int max = 0, temp, r;
		for(int i = row1-1; i < Nsub-edge_size+row1; i++){
			temp = 0;
			for(int j = i; j < edge_size+i; j++){
				for(int k = col1-1; k < Msub+col1-1; k++){
					temp += land[j*M+k].popul;
				}
			}
			if(temp > max){
				max = temp;
				r = i;
			}
		}

		/* Set land using max_idx */
		land[(r+gap)*M+(col1-1)].terrain = 'C';
		for(int i = r; i < edge_size+r; i++){
			for(int j = col1-1; j < Msub+col1-1; j++){
				land[i*M+j].popul = 0;
			}
		}
		land[(r+gap)*M+(col1-1)].popul = max;
	}
	else{
		int max = 0, temp, r, c;
		for(int i = row1-1; i < Nsub-edge_size+row1; i++){
			for(int j = col1-1; j < Msub-edge_size+col1; j++){
				temp = 0;
				for(int k = i; k < edge_size+i; k++){
					for(int m = j; m < edge_size+j; m++){
						temp += land[k*M+m].popul;
					}
				}
				if(temp > max){
					max = temp;
					r = i;
					c = j;
				}
			}
		}

		/* Set land using max idx*/
		land[(r+gap)*M+(c+gap)].terrain = 'C';
		for(int i = r; i < edge_size+r; i++){
			for(int j = c; j < edge_size+c; j++){
				land[i*M+j].popul = 0;
			}
		}
		land[(r+gap)*M+(c+gap)].popul = max;
	}
}

int main(){
	char cmd_type[11];
	Land *land;
	int N, M;
	PopulIdx *popul_idx;
	int p_idx = 0;

	clock_t start, end;

	while(1){
		scanf("%s", cmd_type);

	start = clock();

		if(!strcmp(cmd_type, "new")){
			/* Get land size & Initialize land */
			scanf("%d %d", &N, &M);
			int land_size = N*M;
			land = (Land*)malloc(sizeof(Land)*land_size);
			for(int i = 0; i < N; i++){
				for(int j = 0; j < M; j++){
					land[i*M+j].terrain = 'P';
					land[i*M+j].height = 0;
					land[i*M+j].popul = 0;
				}
			}
	end = clock();
	printf("new excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}
		else if(!strcmp(cmd_type, "sett")){
			/* Get location & Get terrain string */
			int r1, c1, r2, c2;
			scanf("%d %d %d %d", &r1, &c1, &r2, &c2);
			int terrain_len = (r2-r1+1)*(c2-c1+1)+1;	//strlen(terrain)+1(NULL)
			char *terrain = (char*)malloc(terrain_len);
			scanf("%s", terrain);

			/* Change the terrian */
			int idx = 0;
			for(int i = r1-1; i < r2; i++){
				for(int j = c1-1; j < c2; j++){
					land[i*M+j].terrain = terrain[idx++];
				}
			}

			/* free */
			free(terrain);
	end = clock();
	printf("sett excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}	
		else if(!strcmp(cmd_type, "seth")){
			/* Get location & Get height arr */
			int r1, c1, r2, c2;
			scanf("%d %d %d %d", &r1, &c1, &r2, &c2);
			int height_len = (r2-r1+1)*(c2-c1+1);
			int *height = (int*)malloc(sizeof(int)*height_len);
			for(int i = 0; i < height_len; i++){
				scanf("%d", &height[i]);
			}

			/* Change the height */
			int idx = 0;
			for(int i = r1-1; i < r2; i++){
				for(int j = c1-1; j < c2; j++){
					land[i*M+j].height = height[idx++];
				}
			}

			/* free */
			free(height);
	end = clock();
	printf("seth excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}
		else if(!strcmp(cmd_type, "setp")){
			/* Get location & Get popul arr */
			int r1, c1, r2, c2;
			scanf("%d %d %d %d", &r1, &c1, &r2, &c2);
			int popul_len = (r2-r1+1)*(c2-c1+1);
			int *popul = (int*)malloc(sizeof(int)*popul_len);
			for(int i = 0; i < popul_len; i++){
				scanf("%d", &popul[i]);
			}
			
			/* Change the popul */
			int idx = 0;
			for(int i = r1-1; i < r2; i++){
				for(int j = c1-1; j < c2; j++){
					land[i*M+j].popul = popul[idx++];
				}
			}

			/* free */
			free(popul);

			/* Store location where people exist to popul_idx */
			if(p_idx == 0){
				popul_idx = (PopulIdx*)malloc(sizeof(PopulIdx));
				popul_idx[p_idx].r1 = r1;
				popul_idx[p_idx].c1 = c1;
				popul_idx[p_idx].r2 = r2;
				popul_idx[p_idx].c2 = c2;
				p_idx++;
			}
			else{
				PopulIdx *temp = (PopulIdx*)malloc(sizeof(PopulIdx)*p_idx);
				for(int i = 0; i < p_idx; i++){
					temp[i].r1 = popul_idx[i].r1;
					temp[i].c1 = popul_idx[i].c1;
					temp[i].r2 = popul_idx[i].r2;
					temp[i].c2 = popul_idx[i].c2;
				}
				free(popul_idx);
				popul_idx = (PopulIdx*)malloc(sizeof(PopulIdx)*(p_idx+1));
				for(int i = 0; i < p_idx; i++){
					popul_idx[i].r1 = temp[i].r1;
					popul_idx[i].c1 = temp[i].c1;
					popul_idx[i].r2 = temp[i].r2;
					popul_idx[i].c2 = temp[i].c2;
				}
				free(temp);
				popul_idx[p_idx].r1 = r1;
				popul_idx[p_idx].c1 = c1;
				popul_idx[p_idx].r2 = r2;
				popul_idx[p_idx].c2 = c2;
				p_idx++;
			}
	end = clock();
	printf("setp excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}
		else if(!strcmp(cmd_type, "climb")){
			/* Make temp_popul for people move only one time */
			int **temp_popul = (int**)malloc(sizeof(int*)*p_idx);
			for(int i = 0; i < p_idx; i++){
				int len = (popul_idx[i].r2-popul_idx[i].r1+1)*(popul_idx[i].c2-popul_idx[i].c1+1);
				temp_popul[i] = (int*)malloc(sizeof(int)*len);
				int idx = 0;
				for(int j = popul_idx[i].r1-1; j < popul_idx[i].r2; j++){
					for(int k = popul_idx[i].c1-1; k < popul_idx[i].c2; k++){
						temp_popul[i][idx++] = land[j*M+k].popul;
					}
				}
			}

			/* loop for the number of p_idx */
			for(int i = 0; i < p_idx; i++){
				/* rectangular array traversal */
				int idx = 0;
				for(int j = popul_idx[i].r1-1; j < popul_idx[i].r2; j++){
					for(int k = popul_idx[i].c1-1; k < popul_idx[i].c2; k++){
						int check[4];
						/* Check whether idx is edge */
						check_edge(check, N, M, j, k);
						climb_highest(land, check, temp_popul[i][idx++], j, k, M);
					}
				}
			}

			/* free */
			for(int i = 0; i < p_idx; i++){
				free(temp_popul[i]);
			}
			free(temp_popul);
	end = clock();
	printf("climb excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}
		else if(!strcmp(cmd_type, "scatter")){
			/**/
			int **temp_popul = (int**)malloc(sizeof(int*)*p_idx);
			for(int i = 0; i < p_idx; i++){
				int len = (popul_idx[i].r2-popul_idx[i].r1+1)*(popul_idx[i].c2-popul_idx[i].c1+1);
				temp_popul[i] = (int*)malloc(sizeof(int)*len);
				int idx = 0;
				for(int j = popul_idx[i].r1-1; j < popul_idx[i].r2; j++){
					for(int k = popul_idx[i].c1-1; k < popul_idx[i].c2; k++){
						temp_popul[i][idx++] = land[j*M+k].popul;
					}
				}
			}

			for(int i = 0; i < p_idx; i++){
				int idx = 0;
				for(int j = popul_idx[i].r1-1; j < popul_idx[i].r2; j++){
					for(int k = popul_idx[i].c1-1; k < popul_idx[i].c2; k++){
						int cnt = 0, check[4];
						check_edge(check, N, M, j, k);
						for(int m = 0; m < 4; m++){
							if(check[i])	cnt++;
						}
						int scatter_num = temp_popul[i][idx++]/cnt;
						if(check[0])	land[j*M+k+1].popul += scatter_num;
						if(check[1])	land[j*M+k-1].popul += scatter_num;
						if(check[2])	land[(j+1)*M+k].popul += scatter_num;
						if(check[3])	land[(j-1)*M+k].popul += scatter_num;
						land[j*M+k].popul -= (scatter_num*cnt);
					}
				}
				if(popul_idx[i].r1 > 1)		popul_idx[i].r1--;
				if(popul_idx[i].c1 > 1)		popul_idx[i].c1--;
				if(popul_idx[i].r2 < N)		popul_idx[i].r2++;
				if(popul_idx[i].c2 < M)		popul_idx[i].c2++;
			}

			/* free */
			for(int i = 0; i < p_idx; i++){
				free(temp_popul[i]);
			}
			free(temp_popul);
	end = clock();
	printf("scatter excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}
		else if(!strcmp(cmd_type, "settle")){
			int gap;
			scanf("%d", &gap);
			
			int Nsub, Msub, r1, c1, r2, c2;
			r1 = popul_idx[0].r1;
			c1 = popul_idx[0].c1;
			r2 = popul_idx[0].r2;
			c2 = popul_idx[0].c2;
			for(int i = 0; i < p_idx; i++){
				if(popul_idx[i].r1 < r1)	r1 = popul_idx[i].r1;
				if(popul_idx[i].c1 < c1)	c1 = popul_idx[i].c1;
				if(popul_idx[i].r2 > r2)	r2 = popul_idx[i].r2;
				if(popul_idx[i].c2 > c2)	c2 = popul_idx[i].c2;
				Nsub = r2-r1+1;
				Msub = c2-c1+1;
			}
			// settle_city(int gap, int N, int M, Land *land, int N, int M, int row1, int col1)
			settle_city(gap, N, M, land, Nsub, Msub, r1, c1);
	end = clock();
	printf("settle excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}
		else if(!strcmp(cmd_type, "print")){
			int row, col;
			scanf("%d %d", &row, &col);
			printf("%c %d %d\n", land[(row-1)*M+(col-1)].terrain, land[(row-1)*M+(col-1)].popul, land[(row-1)*M+(col-1)].height);
	end = clock();
	printf("print excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}
		else if(!strcmp(cmd_type, "count")){
			/* count each terrain */
			int p = 0, m = 0, c = 0;
			for(int i = 0; i < N; i++){
				for(int j = 0; j < M; j++){
					if(land[i*M+j].terrain == 'P')		p += land[i*M+j].popul;
					else if(land[i*M+j].terrain == 'M')	m += land[i*M+j].popul;
					else if(land[i*M+j].terrain == 'C')	c += land[i*M+j].popul;
				}
			}
			printf("%d %d %d\n", p, m, c);
	end = clock();
	printf("count excution time: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
//print_Land(N, M, land);
		}
		else if(!strcmp(cmd_type, "exit")){
			free(popul_idx);
			free(land);
			break;
		}
	}

	return 0;
}
