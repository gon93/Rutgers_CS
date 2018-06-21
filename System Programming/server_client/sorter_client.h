/*****
*
*	Define structures and function prototypes for your sorter
*
*
*
******/

//Suggestion: define a struct that mirrors a record (row) of the data set
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

