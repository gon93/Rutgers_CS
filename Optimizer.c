/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2017                              *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

int main()
{
	Instruction *head;
	Instruction *current;
	Instruction *temp;

	int regList[30];
	int memList[30];
	int countR = 0; //register count
	int countM = 0; //memory count
	int i,j,a;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}
	/*YOUR CODE GOES HERE*/
	current = head;
	while(current->opcode != OUTPUTAI){
		current-> critical = 'n';
		current = current ->next;
	}
	//when current -> opcode = OUTPUTAI
	current -> critical = 'c';

	
	memList[countM++] = current -> field2;
	current = current->prev;
	
	while(current->prev != NULL){
		switch(current->opcode){
			case OUTPUTAI:
				break;
			case LOADI:
				if(current->field2 == regList[countR-1]){
					countR--;
					current = current->prev;
				}else if(current ->field1 == 1024){
					head = current;
					current = current -> prev;
				}else{
					temp = current -> prev;
					temp -> next = current -> next;
					free(current);
					current = temp;
				}
				break;

			case LOADAI:
				if(current->field3 == regList[countR-1]){
					memList[countM++] = current->field2;
					countR--;
					current = current ->prev;
				}else{
					temp = current->prev;
					temp -> next = current -> next;
					free(current);
					current = temp;
				}
				break;

			case STOREAI:
				if(current->field3 == memList[countM-1]&&countM !=0){
					memList[countM-1] = '\0';
					countM--;
					regList[countR++] = current->field1;
					current = current->prev;
				}else{
					temp = current->prev;
					temp -> next = current ->next;
					free(current);
					current = temp;
				}
				break;
					
			case ADD:
			case SUB:
			case MUL:
			case DIV:
				if(current->field3 == regList[countR-1]){
					regList[countR-1] = current->field1;
					regList[countR++] = current ->field2;
					current = current-> prev;
				}else{
					temp = current->prev;
					temp ->	next = current -> next;
					free(current);
					current = temp;	
				}
				break;
		}
		for(i = 0; i < countM;++i){
			for(j = i + 1; j < countM;++j){
				if(memList[i] > memList[j]){
					a = memList[i];
					memList[i] = memList[j];
					memList[j] = a;
				}
			}
		}
	}

	if (head) 
		PrintInstructionList(stdout, head);
	
	return EXIT_SUCCESS;
}

