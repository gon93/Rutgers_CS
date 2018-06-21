/*
*mystery.c
*Seong Kon Kim
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mystery.h"

int main(int argc,char* argv[]){
	if(argc != 2){
		printf("Error\n");
	}else{
		int num;
		num = atoi(argv[1]);
		int fib[201];
		printf("Value:	%d\n" ,dothething(num,fib));
	}
}
int dothething(int num,int fib[]){
	int count =0;
	while(count <= num){
		if(count <= 1){
			fib[count] = count;
			count++;
		}else{
			fib[count] = add(fib[count-1],fib[count-2]);
			count++;
		}
	}	
	return fib[count-1]; 
}

int add(int a, int b){
	return a+b;
}
