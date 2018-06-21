/*
//
//Seong Kon Kim
//Oct 21, 2016
//format.h
*/

#include <stdio.h>
#include <string.h>
#include "format.h"

void floatToString(float,char*,int); // changing the float t ostring using intToString.

int main(int argc, char** argv){
	 if(argc!= 3){
		fprintf(stderr,"ERROR\n");
	 }else if(strlen(argv[1]) !=32){
		 fprintf(stderr,"ERROR\n");
	 }else{
		 int count;// counting the digits
		 int base =1, result = 0;
		 char intArray[11];
		 char *ptr = argv[1];

		//checking if it is binary.
		for(count = 0; count < strlen(ptr);count++){
			if(ptr[count] != '0' && ptr[count] != '1'){
				fprintf(stderr,"ERROR\n");
			}
		}

/////////changing to int
		if(strcmp(argv[2],"int") == 0){
			//positive
			if(ptr[0] == '0'){
				while(count > 0){
					if(ptr[count-1] == '1'){
						result += base;
					}
					base = base*2;
					count--;
				}
				intToString(result,intArray,0);
				printf("%s\n",intArray);
			//negative
			}else if(ptr[0] == '1'){
				int digit = 0; 
				//going through the bit sequence and chaging 0 to 1, 1 to 0
				for(;digit < strlen(ptr);digit++){
					if(ptr[digit] == '0'){
						ptr[digit]='1';
					}else{
						ptr[digit] = '0';
					}
				}
				while(count > 0){
					if(ptr[count-1] == '1'){
						result += base;
					}
					base = base*2;
					count--;
				}
				result++;
				intArray[0] = '-'; //because it's negative. 
				intToString(result,intArray,1);
				printf("%s\n",intArray);
				
			}
////////change to float			
		}else if(strcmp(argv[2],"float") == 0){
			floatFormat(ptr);
		}	
	}
}


//floatint point Format
char* floatFormat(char* ptr){
	char exponent[10];
	char floatArray[10];
	float baseF =0.5;
	float flo = 0.0;
	int base = 1, minus = 0, point = 0;
	int bit; //goingthrough bit sequence
	int exp = 0;
	int zero =0, NaN = 0, inf = 0;			
	
	//sign
	if(ptr[0] == '1'){
		minus = 1;
		floatArray[0] = '-';
		floatArray[1] = '\0';
		ptr++;
	}else{
		floatArray[0] = '\0';
		ptr++;
	}
	
	//exponent
	for(bit = 0;bit<8;bit++){
		exp += (ptr[7-bit]-'0')*base;
		base = base*2;
	}
	exp = exp-127;
	ptr = ptr+8;

	//mantissa
	//when the mantissa either 0 or +/-denormalized real number
	if(exp == -127){

		//getting the float value
		for(bit=0; bit < strlen(ptr);bit++){
			flo += (ptr[bit]-'0') * baseF;
			baseF = baseF* 0.5;
		}
		//zero
		if(flo == 0.0){
			exp =0;
			zero = 1;
		//exp is going to equal to -bias +1 	
		}else{
			exp++;
			//because exp = -126 making
			baseF= 0.5;
			baseF = mul(0.5,-exp);
			flo *= baseF;
			exp =0;
			while(flo < 1){
				flo *= 10;
				exp--;
			}
		}

		
	//when the mantissa either infinity or Not a number
	}else if(exp == 128){
		for(bit = 0; bit< strlen(ptr);bit++){
			if(ptr[bit] != '0'){
				NaN = 1;
				break;
			}
		}
		if(NaN==0){
			inf =1;
		}
		
		
	//when exponent is between -126 to 127
	}else{
		flo = 1.0;		
		
		for(bit =0; bit<strlen(ptr);bit++){
				flo += (ptr[bit]-'0') * baseF;
				baseF *= 0.5;
		}

		//if exponent < 0
		if(exp < 0){
			baseF= 0.5;
			baseF = mul(0.5,-exp);
			flo *= baseF;
			exp =0;
			while(flo < 1){
				flo *= 10;
				exp--;
			}
		//if exponent >0	
		}else if(exp > 0){
			base = 2;
			base = mul(2,exp);
			flo *=base;
			exp =0;
			while(flo>10){
				flo /=10;
				exp++;
			}
			
		}
	}
	//getting the number of decimal points when exponent < 0
	if(exp < -6){
		point = 6;
	}else{
		point = 6+exp;
	}
		
	//getting the exponent value
	if(exp < 0){
		exponent[0] = 'e';
		exponent[1] = '-';
		intToString(-exp,exponent,2);
			
	}else if(exp>0){
		exponent[0] = 'e';
		intToString(exp,exponent,1);
	}else{
		exponent[0] = 'e';
		exponent[1] = '0';
		exponent[2] = '\0';
	}
		//zero
	if(zero ==1){
		strcat(floatArray,"0.0\0");
		strcat(floatArray,exponent);
		printf("%s\n",floatArray);
	//infite number
	}else if(inf== 1){
		if(minus == 1){
			floatArray[0] = 'n';
			floatArray[1] = '\0';
		}else{
			floatArray[0] = 'p';
			floatArray[1] = '\0';
		}
		strcat(floatArray,"inf");
		printf("%s\n",floatArray);
	//Not a Number
	}else if(NaN ==1){
		if(minus == 1){
			floatArray[0] = '\0';
		}
		strcat(floatArray,"NaN");
		printf("%s\n",floatArray);
	//normalized & denomalized
	}else{
		floatToString(flo,floatArray,point);
		strcat(floatArray,exponent);
		printf("%s\n",floatArray);
	}
	return NULL;
}
//putting the value into string with value,destination,available spot
int intToString(int exp, char *array,int es){
	int temp = exp;
	int count = -1;
	int final;
	if(temp == 0){
		array[0] = '0';
		array[1] = '\0';
		return 1;
	}
	while(temp>0){
		temp /= 10;
		count++;
	}
	final = es+count+1;
	array[es+count+1] = '\0';
	for(; count>=0;count--){
			temp = exp%10;
			array[es+count] = temp +'0';
		exp = exp/10;
	}
	return final;
}

//chaging float to String using intToString() with 
void floatToString(float flo, char *array,int point){
	int temp = flo;
	float dec = flo -temp;
	int end;
	if(array[0] == '-'){
		end = intToString(temp,array,1);
	}else{
		end = intToString(temp,array,0);
	}
	array[end++]= '.';
	if(dec*10 <1){
		array[end++] = '0';
	}
	for(;point >0; point--){
		dec *= 10;
	}
	temp = dec;
	if((dec-temp) >= 0.5){
		temp++;
	}
	
	
	int last = intToString(temp,array,end);
	//taking out the useless zeros
	while(array[last-1] == '0'){
		if(array[last-1] == '0'){
			array[last-1] = '\0';
			last--;
		}
	}
}

float mul (float x,int n){
    int i; /* Variable used in loop counter */
    float number = 1;

    for (i = 0; i < n; ++i)
        number *= x;

    return(number);
}




