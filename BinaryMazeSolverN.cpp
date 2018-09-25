#include<iostream>
int maze[100][100];
char solution[100][100];
void printM(int N){
		int i,j;
		printf("***SOLUTION***\nS-SOURCE P-PATH W-WALL D-DESTINATION\n");
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if(i==N-1&&j==N-1)
				printf("|D|");
				else if(i==0&&j==0)
				printf("|S| ");
				else
				printf("|%c| " ,solution[i][j]);
			}
			printf("\n");
		}
	}
bool Safe(int x, int y, int N) {
		// check if x and y are in limits and cell is not blocked
		if (x >= 0 && y >= 0 && x < N  && y < N && maze[x][y] != 0) {
			return true;
		}
		return false;
	}
bool findPath(int x, int y, int N,const char direction) {
		// check if maze[x][y] is feasible to move
		if(x==N-1 && y==N-1){//we have reached
			solution[x][y] = 1;
			return true;
		}
		if (Safe(x, y, N)) {
			// move to maze[x][y]
			solution[x][y] = 'P';			
			// now rat has four options, either go right OR go down or left or go up
			if(direction!='u' && findPath(x+1, y, N, 'd')){ //go down
				return true;
			}
			//else go down
			if(direction!='l' && findPath(x, y+1, N,'r')){ //go right
				return true;
			}
			if(direction!='d' && findPath(x-1, y, N, 'u')){ //go up
				return true;
			}
			if(direction!='r' &&  findPath(x, y-1, N, 'l')){ //go left
				return true;
			}
			//if none of the options work out BACKTRACK undo the move
			solution[x][y] = 'W';
			return false;
		}
		return false;
	}
void solveMaze(int N) {
		if (findPath(0, 0, N, 'd')) 
		{
			printM(N);
		}
		else
		{
			printf("NO SOLUTION PATH FOUND\n");
		}
		
	}
int main()
 {
		int N;		
		printf("Enter the size of the maze\n");
		scanf("%d",&N);
		printf("Enter the maze\n");			
		int i,j;
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				solution[i][j] = 'W';
				printf("Enter the status of Cell %d , %d ",i+1,j+1);
				scanf("%d",&maze[i][j]);
			}
		}
		solveMaze(N);
		return 0;
}

