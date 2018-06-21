/*
*********************************************
*  213 System Programming                   *
*  Seong Kon Kim                            *
*  Zitian Qui                               *
*  Alon Flor                                *
*  Dec 14,2017                              *
*  sorter_cleint.h                          *
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

			
#endif

