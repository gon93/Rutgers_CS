#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorter_server.h"



 



 void mergesort(Row* rows, int index, int size, int type){
	//printf("hineini\t\tzut\n");
	if(size==1)return;	//base case, a single row
        int ii=0;
	int size1= size/2;
	int size2=size -(size/2);			
	
	mergesort(rows,index,size1,type);
	mergesort(rows,index+size1, size2,type);
	
	int c=0;
	int d=0;
	
	//printf("values: index=%d, size1=%d, start2=%d, size2=%d, size=%d\n",index,size1,index+size1, size2, size);									

						
	
	//make temp rows
	Row* rs1= malloc((size1)*sizeof(Row));
	for(ii=index; ii<index+size1; ii++){
		rs1[ii-index]=rows[ii];
	}
	Row* rs2= malloc(size2*sizeof(Row));
	for(ii=index+size1; ii<index+size1+size2; ii++){
		rs2[ii-(index+size1)]=rows[ii];
	}


	/*for(c=0; c<size1; c++){
		for(d=0; d<rs1[c].size; d++){
			printf("%s\t",rs1[c].elements[d]);
		}
		printf("\n");
	}
	printf("put Rs1\n\n");
	for(c=0; c<size2; c++){
		for(d=0; d<rs2[c].size; d++){
			printf("%s\t",rs2[c].elements[d]);
		}
		printf("\n");
	}
	printf("put Rs2\n");*/


										//printf("merge\tsize1=%d\tsize2=%d\n",size1,size2);
	merge(rs1,size1,rs2,size2,rows,index,type);	//modify the copy;

	/*for(c=0; c<size2; c++){
		for(d=0; d<rs2[c].size; d++){
			printf("%s\t",rs2[c].elements[d]);
		}
		printf("\n");
	}
	printf("put Rs2\n");*/

	
	
	/*for(c=0; c<size; c++){
		for(d=0; d<rows[c].size; d++){
			printf("%s\t",rows[c].elements[d]);
		}
		printf("\n");
	}
	printf("put result\n");*/
	free(rs1);
	free(rs2);
		//printf("jdrgierjhgierhgu\n\n\n\n");	
	
 }

 void merge(Row* mr1, int size1, Row* mr2, int size2, Row* result,int index,int type){
 	int indexR1=0;
 	int indexR2=0;
 	int indexRes=index;
 	//int resultSize=0;
 	while(indexR1<size1 || indexR2<size2){
		if(indexR1<size1 && indexR2<size2){           //mr1 and mr2 both have stuff. Find the smaller one and add it

			int compres = compare(mr1[indexR1], mr2[indexR2], type);		//			printf("%d\t%s %s %s\n",compres,mr1[indexR1].elements[col],(compres>0) ? ">":"<=",mr2[indexR2].elements[col]);
                        if(compres>0){                                      //the thing in mr1 is bigger. Add the thing in mr2.
                          result[indexRes]=mr2[indexR2];			//printf("yo\n");
                          indexR2=indexR2+1;
			  indexRes=indexRes+1;
                        }else{                                              //the thing in mr2 is bigger of they are equal. Add the thing in mr1.
                          result[indexRes]=mr1[indexR1];			//printf("yohoho\n");	
                          indexR1=indexR1+1;
                          indexRes=indexRes+1;
                        }
			
		}else if(indexR1<size1){                         //only mr1 has stuff left, add things from there
                        result[indexRes]=mr1[indexR1];				//printf("Things happen here\n");
                        indexR1=indexR1+1;
			indexRes=indexRes+1;
		}else if(indexR2<size2){                         //only mr2 has stuff left, add things from there
                        result[indexRes]=mr2[indexR2];			//printf("Things happen here too\n");
                        indexR2=indexR2+1;
                        indexRes=indexRes+1;
		}

	}

 }

 //1 if row1 is bigger, -1 if row2 is bigger, 0 if they are equal
 int compare(Row row1, Row row2, int type){
	if(type==0){
		int trim1=0;
		int trim2=0;
		while(trim1<strlen(row1.colterm)){
			if(row1.colterm[trim1]==' '){
				trim1=trim1+1;
			}else if(row1.colterm[trim1]=='\t'){
				trim1=trim1+1;
			}else{
				break;
			}
		}
		while(trim2<strlen(row2.colterm)){
			if(row2.colterm[trim2]==' '){
				trim2=trim2+1;
			}else if(row2.colterm[trim2]=='\t'){
				trim2=trim2+1;
			}else{
				break;
			}
		}
		return strcmp(&row1.colterm[trim1],&row2.colterm[trim2]);
	}
	int i1=atoi(row1.colterm);
	int i2=atoi(row2.colterm);
	if(i1>i2)return 1;
	if(i1<i2)return -1;
        return 0;

 }






