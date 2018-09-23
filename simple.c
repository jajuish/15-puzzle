#include<stdio.h>
#include<stdlib.h>

void print_board(int a[][4]) {
	
	int i,j,k;
	
	printf("\nBoard:\n");
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

int the_move(int accepted[]) {

	int i,j,move;
	
	printf("\nYour move: ");
	scanf("%d",&move);
	
	if(move != accepted[0] && move != accepted[1] && move != accepted[2] && move != accepted[3]) {
		printf("Invalid move.");
		move = the_move(accepted);
	}
	
	return move;
}

int main(int argc,char *argv[]) {
	
	/* This is the initial configuration. Change the board here to change the initial board */
	int initial[4][4]={	{1,2,3,4},
						{5,6,0,8},
						{9,10,7,11},
						{13,14,15,12}},
		final[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
	int i,j,k,t,continued=1;
	int *accepted,move;
	
	printf("Zero is the empty space. The final board configuration: ");
	print_board(final);
	printf("\n----START GAME----\n");
	
	while(continued == 1) {
		
		continued = 0;
		
		print_board(initial);
		accepted = accepted_moves(initial);
		
		printf("\n\nAccepted moves: ");
		for(i=0;i<4;i++) {
			if(accepted[i] != 0)
				printf("%d ",accepted[i]);
		}
		
		move = the_move(accepted);
		
		for(i=0;i<4;i++)
			for(j=0;j<4;j++) {
				if(initial[i][j] == move)
					initial[i][j] = 0;
				else if(initial[i][j] == 0)
					initial[i][j] = move;
			}
		
		for(i=0;i<4;i++)
			for(j=0;j<4;j++)
				if(initial[i][j] != final[i][j])
					continued = 1;
	}
	
	printf("You win!");
	
    return 0;	
}
