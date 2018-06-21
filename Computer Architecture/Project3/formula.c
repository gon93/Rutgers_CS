/*
//
//Seong Kon Kim
//Nov6,2016
//formula.c
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ctype.h>
#include "nCr.h"
#include "formula.h"

int main(int argc, char* argv[]){
	unsigned int time;
	gettimeofday(&begin,NULL);
	if(argc != 2){
		fprintf(stderr,"Error\n");
	}else if(strcmp(argv[1],"-h")==0){
		printf("Usage: formula <positive integer>\n");
	}else{
		int r,n;
		for(n =0; n <strlen(argv[1]);n++){
			if(!isdigit(argv[1][n])){
				fprintf(stderr,"Error\n");
				return 0;
			}
		}
		n = atoi(argv[1]);
		
		if(n< 0 || nCr(n,1) == 0){
			fprintf(stderr,"Error\n");			
		}else{

			printf("1+x^%d = 1 ",n);
			for(r=1; r <= n;r++){	
				printf("+ %d*x^%d ",nCr(n,r),r);
			}
			printf("\n");
		}
		gettimeofday(&end,NULL);
		time =  end.tv_usec -begin.tv_usec;
		printf("Time Required = %d microseconds\n",time);
	}
	return 0;
}	
