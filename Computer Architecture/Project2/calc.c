/*
//
//Seong Kon Kim
//Dec 9, 2016
//calc.c
*/

#include <stdio.h>
#include <string.h>
#include "calc.h"

void AnswerConversion(int,char*); //chaging the last value to string to print out.

int main (int argc, char **argv){
	
	if(argc != 5){
		fprintf(stderr,"Error\n");
		return 0;
	}else{
		int answer = 0; 
		//argv[2] change to decimal
		int decimalOne = Conversion(argv[2]);
		if(error == 1){
			fprintf(stderr,"Error\n");
			return 0;
		}

		//argv[3] change to decimal
		int decimalTwo = Conversion(argv[3]);
		if(error == 1){
			fprintf(stderr,"Error\n");
			return 0;
		}

		//arithmetic operators
		if(argv[1][0] == '+' && argv[1][1] == '\0' && error ==0){
			answer = decimalOne + decimalTwo;
		}else if(argv[1][0] == '-' && argv[1][1] == '\0' && error ==0){
			answer = decimalOne - decimalTwo;
		}else if(argv[1][0] == '*' && argv[1][1] == '\0' && error ==0){
			answer = decimalOne * decimalTwo;
		}else{
			fprintf(stderr,"Error\n");
			return 0;
		}
		//change to what answerType wants(argv[4])
		AnswerConversion(answer, argv[4]);
		if(error == 1){
			fprintf(stderr,"Error\n");
			return 0;
		}
		return 0;
	}
}

//changing the type to deciamal
int Conversion(char *ptr){
	int index,minus=0,count = 0; 
	int result = 0;//final answer
	int num = 0;
	if(ptr[0] == '-'){
		minus = 1;//minus exist
		ptr++;
	}
	//decimal computation
	if(ptr[0] == 'd'){
		ptr++;
		//looking for error and counting the length of number
		for(index=0;ptr[index] != '\0'; index++){
			if(ptr[index] >= '0' && ptr[index] <= '9'){
				count++;
			}else{
				error = 1;
				break;
			}
		}
		//getting the actual value
		for(index=0; index<count; index++){
			result = result*10 +(ptr[index]-'0');
		}	
	//binary to decimal
	}else if(ptr[0] == 'b'){
		ptr++;
		//looking for error and counting the length of number
		for(index=0; ptr[index] != '\0'; index++){
			if(ptr[index]== '0' || ptr[index] =='1'){
				count++;
			}else{
				error = 1;
				return 0;
			}
		}
		//getting the actual value
		for(index=0; index<count; index++){
			num = num*10 +(ptr[index]-'0');
		}
	
		//converting binary to decimal
		int base = 1, rem;
		while(num>0){
			rem = num % 10;
			result = result + rem * base;
			num = num / 10 ;
			base = base * 2;			
		}
	//octal to decimal	
	}else if(ptr[0] == 'o'){
		ptr++;
		//looking for error and counting the length of number
		for(index=0; ptr[index] != '\0'; index++){
			if(ptr[index]>= '0' && ptr[index] <='7'){
				count++;
			}else{
				error = 1;
				return 0;
			}
		}

		//converting octal to decimal
		int base = 1;
		while(count > 0){
			result = result + ((ptr[count-1]-'0')*base);
			base = base * 8;
			count--;
		}
	//hex to decimal
	}else if(ptr[0] == 'x'){
		ptr++;
		//looking for error and counting the length of number
		for(index=0;ptr[index] != '\0'; index++){
			if(ptr[index] >= '0' && ptr[index] <= 'F'){
				count++;
			}else{
				error = 1;
				return 0;
			}
		}
		int base = 1;
		//converting hex to decimal
		while(count > 0){
			if(ptr[count-1] >= 'A' && ptr[count-1] <= 'F'){
				result = result + ((ptr[count-1]-'0'-7)*base);
			}else{
				result = result + ((ptr[count-1]-'0')*base);
			}
			base = base * 16;
			count--;
			
		}
	}else{
		error=1;
		return 0;
	}
	if(minus == 1){
		result = -result;
		return result;
	}else{
		return result;
	}
}
//changing the answer into the answerType
void AnswerConversion(int answer, char *d){
	int rem, base = 1;
	int index = 0,j,minus =0;
	char result[33];
	
	//checking if there is minust in the answer
	if(answer < 0){
		answer = -answer;
		minus = 1;
	}
	
	//if the answer is in decimal
	if(d[0] == 'd' && d[1] == '\0'){
		//putting the variable into string
		if(answer == 0){
			result[index++] = answer + '0';
		}else{
			while(answer != 0){
				rem = answer%10;
				result[index++] = rem + '0';
				answer /= 10;
			}
		}
		if(minus == 1){
			
			result[index++] =  'd';
			result[index] = '-';
		}else{
			result[index] = 'd';
		}
		for(j=index; j>=0; j--){
			printf("%c",result[j]);
		}
		printf("\n");
	//if the answer is in decimal	
	}else if(d[0] == 'b' && d[1] == '\0'){
		long long binaryNumber = 0;
		if(answer == 0){
			result[index++] = answer + '0';
		}else{
			while(answer != 0){
				rem = answer % 2;
				binaryNumber = binaryNumber + rem*base;
				answer = answer/2;
				base = base *10;
			}
		}
		//putting the variable into string
		while(binaryNumber != 0){
			rem = binaryNumber%10;
			result[index++] = rem + '0';
			binaryNumber /= 10;
		}
		if(minus == 1){
			result[index++] =  'b';
			result[index] = '-';
		}else{
			result[index] = 'b';
		}
		for(j=index; j>=0; j--){
			printf("%c",result[j]);
		}
		printf("\n");
		
	//if the answer is in Octal	
	}else if(d[0] == 'o' && d[1] == '\0'){
		int octalNumber = 0;
		if(answer == 0){
			result[index++] = answer + '0';
		}else{
			while(answer != 0){
				rem = answer % 8;
				octalNumber = octalNumber + rem*base;
				answer = answer/8;
				base = base*10;
			}
		}
		//putting the variable into string
		while(octalNumber != 0){
			rem = octalNumber%10;
			result[index++] = rem + '0';
			octalNumber /= 10;
		}
		if(minus == 1){
			result[index++] =  'o';
			result[index] = '-';
		}else{
			result[index] = 'o';
		}
		for(j=index; j>=0; j--){
			printf("%c",result[j]);
		}
		printf("\n");
	//if the answer is in Hex		
	}else if(d[0] == 'x' && d[1] == '\0'){
		if(answer == 0){
			result[index++] = answer + '0';
		}else{
			while(answer != 0){
				rem = answer % 16;
				if(rem <10){
					result[index++] = rem + '0';
				}else{
					result[index++] = rem + '7';
				
				}
				answer = answer/16;
			}
		}
		if(minus == 1){
			result[index++] =  'x';
			result[index] = '-';
		}else{
			result[index] = 'x';
		}
		for(j=index; j>=0; j--){
			printf("%c",result[j]);
		}
		printf("\n");
	//Error		
	}else{
		error = 1;
	}
}
