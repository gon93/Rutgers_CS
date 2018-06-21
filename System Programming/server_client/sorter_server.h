/*
*********************************************
*  213 System Programming                   *
*  Seong Kon Kim                            *
*  Zitian Qui                               *
*  Alon Flor                                *
*  Dec 14,2017                              *
*  sorter_server.h                          *
*********************************************
*/

#ifndef _mydefs
#define _mydefs
 struct aRow{
        char colterm[50];
   int argCount;
	int originalPos;
 };
 typedef struct aRow Row;

struct FinalSortRow{
			char row [10000];
			char colTerm[50];
			//struct RowLinkedListElement * next;
};
typedef struct FinalSortRow FSRow;


//Suggestion: prototype a mergesort function
void mergesort(Row* rows, int index, int size, int type);
void merge(Row* mr1, int size1, Row* mr2, int size2, Row* result,int index,int type);
int compare(Row row1, Row row2, int type);

#endif

