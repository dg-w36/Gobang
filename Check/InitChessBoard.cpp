#include <stdio.h>
int main(){
	FILE *fp;
	fp = fopen("chess_board.txt","w");
	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 6; j++){
			fprintf(fp,"0 ");
		}
		fprintf(fp,"\n");
	}
	return 0;
}
