
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define STATEINITIAL 0
#define STATEZERO 1
#define STATEOCTAL 2
#define STATEHEX 3 
#define STATEFLOAT 4
#define STATEDECIMAL 5
#define STATEMAL 6

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
    char *original;
	char *ptr;// counting where it left off
	char error; //the location for invalid.
	int count; // counting the numbers in tokens
	int ec; //0 or 1 Error check
    int stateNumber;//the token type

};
typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 *
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
TokenizerT *TKCreate( char * ts ) {

    if(strlen(ts) == 0)
        return NULL; //when there is nothing on the string

    TokenizerT *tk = malloc(sizeof(TokenizerT));
    tk->original = malloc(strlen(ts)+1);//to avoid the overflow when copying.;
    tk->ptr = malloc(strlen(ts))+1;
	strcpy(tk->original, ts);
	tk->ptr = tk->original;
	tk->ec = 0;
    return tk;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
void TKDestroy( TokenizerT * tk ) {
    tk->count = 0;
	tk->stateNumber = 0;
	tk->ptr =NULL;
	tk->error = 0;
    tk->original = NULL;
    free(tk);
}
 void NewToken(TokenizerT *tk){
	 tk->stateNumber =0;
	 tk->error=0;
	 tk->ec = 0;
 }

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
 //making varibles to initial value
char *TKGetNextToken( TokenizerT * tk ) {
    tk->stateNumber = STATEINITIAL;
	int i=0;
	tk->count = 0;
	if(isspace(tk->ptr[i])&& tk->ptr[i+1] != '\0'){
		for(i=0;i<strlen(tk->ptr)+1;i++){
			if(isspace(tk->ptr[i])){
				tk->count++;
			}else{
				break;
			}
		}
		for(; i <= tk->count;i++){
			tk->ptr++;
		}
		tk->original = tk->ptr;
		tk->count = 0;
	}
	
	//only one token with one error
	if(!isdigit(tk->ptr[0]) && (isspace (tk->ptr[1]) || tk->ptr[1] =='\0')){
		tk->error = tk->ptr[0];
		tk->ec = 1;
		tk->count++;	
	//(1): zero when token is '0'
	}else if(tk->ptr[0] =='0' && (isspace (tk->ptr[1]) || tk->ptr[1] =='\0')){
		tk->stateNumber = STATEZERO;
		tk->count++;//token has 1 variable
		char*token = (char *) malloc(tk->count + 1 );
		strncpy(token,tk->original,tk->count);
		token[tk->count] = '\0';
		tk->ptr++;
		return token;
	//when the token start with 0
	}else if(tk->ptr[0] == '0' && !isdigit(tk->ptr[1]) && tk->ptr[1] !='e' && tk->ptr[1] != 'E' && tk->ptr[1] != '.' && tk->ptr[1] != 'x' && tk->ptr[1] != 'X'){
		tk->stateNumber = STATEZERO;
		tk->count++;//token has 1 variable
		char*token = (char *) malloc(tk->count + 1 );
		strncpy(token,tk->ptr,tk->count);
		token[tk->count] = '\0';
		tk->ptr++;
		return token;
	//if the first char is 1-9
	}else if(tk->ptr[0]== '0'){
		tk->count++;
		for(i= 1; i <= strlen(tk->ptr)+1; i++){
			//when it hits the end of the token
			if(tk->ptr[i] == '\0' || isspace(tk->ptr[i])){
				break;

		//(2)Octal: 1-7 after '0'
			}else if((tk->ptr[i] >= '0' && tk->ptr[i] <='7') && (tk->stateNumber == STATEINITIAL || tk->stateNumber == STATEOCTAL)){
				tk->stateNumber = STATEOCTAL;
				tk->count++;
				
		// (3)Hex: number after 0x
			}else if(tk->ptr[1]=='x' || tk->ptr[1] == 'X'){
				tk->count++;
				//to be HEX you need to have number after 0x
				if(isspace(tk->ptr[i+1])){
					tk->stateNumber = STATEMAL;
					break;
				//if there is number
				}else if(isxdigit(tk->ptr[i+1])){
					i++;
					//when it knows that its hex than it will continue looking for erros in the system.
					for(; i <=strlen(tk->ptr)+1; i++){					
						if(isspace(tk->ptr[i]) ||tk->ptr[i] == '\0'){
							break;
						}else if(isxdigit(tk->ptr[i])){
							tk->stateNumber = STATEHEX;
							tk->count++;
						}else if(!isxdigit(tk->ptr[i])){
							tk->error = tk->ptr[tk->count];
							tk->ec = 1;
							break;
						}
					}
					break;
				}
				//if nothing is equals
				else if(tk->ptr[i+1] == '\0'){
					tk->stateNumber=STATEMAL;
					break;
				}else{
					tk->stateNumber = STATEMAL;
					tk->error = tk->ptr[tk->count];
					tk->ec = 1;
					break;
				}
				
			//(4)Float: 0.Numbers(1-9)
			}else if(tk->ptr[1] == '.'){
				tk->count++;
				if(isspace(tk->ptr[i+1]) || tk->ptr[i+1] == '\0'){
					tk->stateNumber = STATEMAL;
					break;
				}else if(!isdigit(tk->ptr[i+1])){
					tk->stateNumber = STATEMAL;
					tk->error = tk->ptr[i+1];
					tk->ec = 1;
					break;
				}else if(isdigit(tk->ptr[i+1])){
					i+=2;
					tk->stateNumber = STATEFLOAT;
					for(; i <=strlen(tk->ptr)+1;i++){
						if(isspace(tk->ptr[i]) || tk->ptr[i] == '\0'){
							tk->count++;
							break;
						}else if(isdigit(tk->ptr[i])){
							tk->count++;
						//if the Float number == E,e (REPEAT PROCESS FOR tk->ptr[0] is 1-9)
						}else if(tk->ptr[i] == 'e' || tk->ptr[i] == 'E'){							
							if(tk->ptr[i+1] =='\0' || isspace(tk->ptr[i+1])){
								tk->error = tk->ptr[i];
								tk->count++;
								tk->ec =1;
								break;
							//when it is + and -
							}else if(tk->ptr[i+1] == '+' || tk->ptr[i+1] == '-'){
								i+=2;// to pass e and + togeether
								tk->count+=2;
								if(tk->ptr[i] == '\0' || isspace(tk->ptr[i])){
									tk->stateNumber = STATEMAL;
									tk->error = tk->ptr[i-1];
									tk->ec = 1;
									break;
								}else{
									for(;i<=strlen(tk->ptr);i++){
										if(isdigit(tk->ptr[i])){
											tk->count++;
										}else if(isspace(tk->ptr[i]) || tk->ptr[i] == '\0'){
											tk->count++;
											break;
										}else if(!isdigit(tk->ptr[i])){
											tk->error = tk->ptr[i];
											tk->count++;
											tk->ec = 1;
											break;
										}
									}
									break;
								}
							//# + E + # = *
							}else if(isdigit(tk->ptr[i+1])){
								tk->count++;
								i++;
								for(;i<strlen(tk->ptr)+1;i++){
									if(isdigit(tk->ptr[i])){
									tk->count++;
									}else if(isspace(tk->ptr[i]) || tk->ptr[i] == '\0'){
										tk->count++;
										break;
									}else{
										tk->count++;
										tk->error = tk->ptr[i];
										break;
									}
								}
								break;
							}
						}else if(!isdigit(tk->ptr[i])){
							tk->count++;
							tk->error = tk->ptr[i];
							tk->ec = 1;
							break;
						}
					}
				}
				break;
			//if its not number from the beginning.	
			}else if(!isdigit(tk->ptr[i])){
					tk->error = tk->ptr[i];
					tk->ec =1;
					break;
			//(4)Error: error after 0
			}else{
				if(tk->stateNumber == STATEINITIAL){
					tk->stateNumber = STATEZERO;
				}
				tk->error = tk->ptr[tk->count];
				tk->ec=1;
				break;
			}
		}
	//if the first char is 1-9 and length = 1
	}else if(isdigit(tk->ptr[0]) && (isspace (tk->ptr[1]) || tk->ptr[1] =='\0')){
		tk->stateNumber = STATEDECIMAL;
		tk->count++;//token has 1 variable
		char*token = (char *) malloc(tk->count + 1 );
		strncpy(token,tk->ptr,tk->count);
		token[tk->count] = '\0';
		tk->ptr++;
		return token;
	//has a escape char after the number
	}else if(isdigit(tk->ptr[0]) && !isdigit(tk->ptr[1]) && tk->ptr[1] !='e' && tk->ptr[1] != 'E' && tk->ptr[1] != '.'){
		tk->stateNumber = STATEDECIMAL;
		tk->count++;//token has 1 variable
		char*token = (char *) malloc(tk->count + 1 );
		strncpy(token,tk->ptr,tk->count);
		token[tk->count] = '\0';
		tk->ptr++;
		return token;
	//if the first char is 1-9
	}else if(isdigit(tk->ptr[0])){
		tk->count++;
		for(i = 1; i<= strlen(tk->ptr)+1; i++){
			//Decimal: having a variable 
			if(tk->ptr[i] == '\0' || isspace(tk->ptr[i])){
				tk->stateNumber = STATEDECIMAL;
				break;
			}else if(isdigit(tk->ptr[i])){
				tk->stateNumber = STATEDECIMAL;
				tk->count++;
			//Float: starting with "1."
			}else if(tk->ptr[1] == '.'){
				tk->count++;
				if(isspace(tk->ptr[i+1]) || tk->ptr[i+1] == '\0'){
					tk->stateNumber = STATEMAL;
					break;
				}else if(!isdigit(tk->ptr[i+1])){
					tk->stateNumber = STATEMAL;
					tk->error = tk->ptr[i+1];
					tk->ec = 1;
					break;
				}else if(isdigit(tk->ptr[i+1])){
					i+=2;
					tk->stateNumber = STATEFLOAT;
					for(; i <=strlen(tk->ptr)+1;i++){
						if(isspace(tk->ptr[i]) || tk->ptr[i] == '\0'){
							tk->count++;
							break;
						}else if(isdigit(tk->ptr[i])){
							tk->count++;
						//if the Float number == E,e (REPEAT PROCESS FOR tk->ptr[0] is 1-9)
						}else if(tk->ptr[i] == 'e' || tk->ptr[i] == 'E'){
							if(tk->ptr[i+1] =='\0' || isspace(tk->ptr[i+1])){
								tk->error = tk->ptr[i];
								tk->count++;
								tk->ec =1;
								break;
							//when it is + and -
							}else if(tk->ptr[i+1] == '+' || tk->ptr[i+1] == '-'){
								i+=2;// to pass e and + togeether
								tk->count+=2;
								if(tk->ptr[i] == '\0' || isspace(tk->ptr[i])){
									tk->stateNumber = STATEMAL;
									tk->error = tk->ptr[i-1];
									tk->ec = 1;
									break;
								}else{
									for(;i<=strlen(tk->ptr);i++){
										if(isdigit(tk->ptr[i])){
											tk->count++;
										}else if(isspace(tk->ptr[i]) || tk->ptr[i] == '\0'){
											tk->count++;
											break;
										}else if(!isdigit(tk->ptr[i])){
											tk->error = tk->ptr[i];
											tk->count++;
											tk->ec = 1;
											break;
										}
									}
									break;
								}
							//# +'.' + # + E + # = *
							}else if(isdigit(tk->ptr[i+1])){
								tk->count++;
								i++;
								for(;i<strlen(tk->ptr)+1;i++){
									if(isdigit(tk->ptr[i])){
									tk->count++;
								}else if(isspace(tk->ptr[i]) || tk->ptr[i] == '\0'){
									tk->count++;
									break;
								}else{
									tk->count++;
									tk->error = tk->ptr[i];
									break;
									}
								}
								break;
							}
							//right here
						}else if(!isdigit(tk->ptr[i])){
							tk->count++;
							tk->error = tk->ptr[i];
							tk->ec = 1;
							break;
						}
					}
				}
				break;
			//when # + E or e
			}else if(tk->ptr[i] == 'e' || tk->ptr[i] == 'E'){
				if(tk->ptr[i+1] =='\0' || isspace(tk->ptr[i+1])){
					printf("decimal %c\n",tk->ptr[0]);
					char*token = (char *) malloc(tk->count + 1 );
					strncpy(token,tk->ptr,tk->count);
					token[tk->count] = '\0';
					tk->ptr++;
					return token;
					break;
					
				}else{
					i++;
					tk->count++;
					tk->stateNumber = STATEFLOAT;
					for(;i<= strlen(tk->ptr)+1;i++){
						if(isdigit(tk->ptr[i])){
							tk->count++;
						}else if(isspace(tk->ptr[i]) || tk->ptr[i] == '\0'){
							break;
						}else if(tk->ptr[i] == '+' || tk->ptr[i] == '-'){
							i++;
							tk->count++;
							if(tk->ptr[i] == '\0' || isspace(tk->ptr[i])){
									tk->stateNumber = STATEMAL;
									break;
								}else{
									for(;i<=strlen(tk->ptr);i++){
										if(isdigit(tk->ptr[i])){
											tk->count++;
										}else if(isspace(tk->ptr[i]) || tk->ptr[i] == '\0'){
											break;
										}else if(!isdigit(tk->ptr[i])){
											tk->error = tk->ptr[i];
											tk->ec = 1;
											break;
										}
									}
									break;
								}
						}else if(!isdigit(tk->ptr[i])){
							tk->error = tk->ptr[i];
							tk->ec = 1;
							break;
						}
					}
					break;
				}
			}else if(!isdigit(tk->ptr[i])){
				tk->error = tk->ptr[i];
				tk->ec =1;
				break;
				//(4)Error: error after 0
			}else{
				if(tk->stateNumber == STATEINITIAL){
					tk->stateNumber = STATEZERO;
				}
				tk->error = tk->ptr[tk->count];
				tk->ec=1;
				break;
			}
		}
	
	}else if(!isdigit(tk->ptr[i]) && isprint(tk->ptr[i])) {

		tk->error = tk->ptr[i];
		tk->ec = 1;
		tk->count++;	
	}
	//moving the count to get the left Tokens
    for(i=1; i <= tk->count;i++){
		tk->ptr++;
	}
	char*token = (char *) malloc(tk->count + 1 );
	strncpy(token,tk->original,tk->count);
	token[tk->count] = '\0';
	return token;
}

//printing out error
void Error(TokenizerT *tk, char er){
	if(tk->ec == 1 && !isspace(er)){
		printf("invalid [%#2x]\n",er);
		if(strlen(tk->ptr)!=0 && tk->count > 1){
			tk->ptr++;
		}
	}
}

//printing out the output with types.
 void printToken(TokenizerT *tk, char* s){
	if(tk->stateNumber == 1){
		printf("zero %s\n", s);
	}else if(tk->stateNumber == 2){
		printf("octal %s\n", s);
	}else if(tk->stateNumber == 3){
		 printf("hex %s\n", s);
	 }else if(tk->stateNumber == 4){
		 printf("float %s\n", s);
	 }else if(tk->stateNumber == 5){
		 printf("decimal %s\n", s);
	 }else if(tk->stateNumber == 6){
		 printf("malFunction %s\n",s);
	 }
	Error(tk,tk->error);	 
 }
 
/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out thfe tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
int main(int argc, char **argv) {//argc == size of the argv
	
	if(argc < 2){
		printf("Nothing inputed!");
	}else{
		TokenizerT *Tokenizer;
		
		if(strlen(argv[1]) == 0){
			printf("There is no token available!");
		}else{
			Tokenizer = TKCreate(argv[1]);
			NewToken(Tokenizer);
			while(strlen(Tokenizer->ptr) != 0){
				Tokenizer->original = Tokenizer->ptr;
					char* token = TKGetNextToken(Tokenizer);	
					printToken(Tokenizer,token);
					NewToken(Tokenizer);
			}
		TKDestroy(Tokenizer);
		}
	}
	return 0;
}
