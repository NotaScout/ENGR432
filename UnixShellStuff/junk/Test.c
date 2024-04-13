#include <stdio.h>
#include <stdlib.h>

int i;
int j;

int main(void){
int matrix1[1000][1000];
int matrix2[1000][1000];
int result [1000][1000];
	
	for(i=0;i<1000;i++){
		for(j=0;j<1000;j++){
			matrix1[i][j] = rand() % 100;
			matrix2[i][j] = rand() % 100;
		}
}
	
	
printf("\nResult:\n");
printf(matrix1);
printf(matrix2);
//printf(Result);
return 0;
}