#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void print_board(int a[][4]) {
	
	int i,j,k;
	
	printf("Board:\n");
	//printf("\t\t0\t1\t2\t3\n\n");
	for(i=0;i<4;i++) {
		//printf("%d\t\t",i);
		for(j=0;j<4;j++)
			printf("%d\t",a[i][j]);
		printf("\n");
	}
}

int * accepted_moves(int a[][4]) {

	int i,j,k,curr_i,curr_j;
	static int accepted[4]={0,0,0,0};

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(a[i][j] == 0) {
				curr_i=i,curr_j=j;
				if(!(i-1 < 0))
					accepted[0] = a[i-1][j];
				else
					accepted[0] = 0;
				if(!(j+1 > 3))
					accepted[1] = a[i][j+1];
				else
					accepted[1] = 0;
				if(!(i+1 > 3))
					accepted[2] = a[i+1][j];
				else
					accepted[2] = 0;
				if(!(j-1 < 0))
					accepted[3] = a[i][j-1];
				else
					accepted[3] = 0;
			}
	return accepted;
}

void para(int initial[][4],int accepted[],int prev_mov) {
	
	int final[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
	int move,i,j,k,yes=0;
	
	#pragma omp parallel shared(accepted) num_threads(4)
	{
		int t = omp_get_thread_num(),l,m,n;
		int move,move_i,move_j,zero_i,zero_j;
		int mat[4][4],continued = 0;
		
		for(l=0;l<4;l++)
			for(m=0;m<4;m++)
				mat[l][m] = initial[l][m];
		
		if(t == 0)
			move = accepted[0];
		else if(t == 1)
			move = accepted[1];
		else if(t == 2)
			move = accepted[2];
		else if(t == 3)
			move = accepted[3];

		printf("\nthread %d: %d\n",t,move);
		
		if(move != 0 && move != prev_mov) {
			yes = 1;
			for(i=0;i<4;i++)
				for(j=0;j<4;j++) {
					if(mat[i][j] == move)
						move_i=i,move_j=j;
					else if(mat[i][j] == 0)
						zero_i=i,zero_j=j;
				}
			
			mat[move_i][move_j] = 0;
			mat[zero_i][zero_j] = move;
			print_board(mat);
		
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					if(mat[i][j] != final[i][j])
						continued = 1;
		
			if(continued == 0) {
				printf("Reached the final state with thread %d\n",t);
				exit(0);
			}
			else {
				printf("Not reached the final state yet.\n");
			
				int *new;
				new = accepted_moves(mat);
				printf("Next set of steps: %d %d %d %d\n",new[0],new[1],new[2],new[3]);
				
				para(mat,new,move);
			}
		}
	}
}

int main(int argc,char *argv[]) {

	int initial[4][4]={	{1,2,3,4},
						{5,6,0,8},
						{9,10,7,11},
						{13,14,15,12}};
	int *accepted,move;
	
	//print_board(initial);
	
	omp_set_nested(1);
	
	accepted = accepted_moves(initial);
	
	para(initial,accepted,0);
	
    return 0;	
}
