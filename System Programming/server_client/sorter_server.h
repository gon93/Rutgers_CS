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

int o;
int i;
int iparryc=0;
int thesize=50000000;
int ttid_size=10000;/*the size of setting the int* and [thread-t* */
int ttid_cnt=0;//the cnt idex of ttid
int c_data_size = 10000;
int* arrayosocketfd; //array that holdsthe c_socket 
pthread_t* ttid;//array that hold tid
char** singleclient;
int singleclientcounter=0;
//client dat.saving
char** c_data;
int row_num = 0;
char** iparry;
int theheader=0;
int* iparryse;
pthread_mutex_t connection_lock;/*name of lock*/
void* connection_handler(void *arguments);
int print_ip = 0;
int ip_count = 0;

//Imported stuff
FSRow*pointers[1000];	//pointers to rows, one pointer per file's worth of rows. Will only traverse through FilesToSort rows
void sort(char** raw,char*secondArg,int c_sock  );
//pthread_mutex_t m;
int FilesToSort=0;	//number of files we are actually sorting (used to traverse pointers)
int sizeRemaining[1000];//number of additions to the main file remaining for each pointer in pointers
int universalType=1;	//sort on string or numbers?	0 = String, 1 = number;
char*header;

//End Imported Stuff

