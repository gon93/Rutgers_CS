#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mergechar(char**,int,int);
void merge(char**,int,int,int);

void mergechar(char** arr,int left,int right){
    
	if (left < right) {
		int mid = (left+right)/2;
		mergechar(arr,left,mid);
    mergechar(arr,mid+1,right);
		merge(arr,left,mid,right);
	}
  
}


void merge(char** arr, int left, int mid, int right)
{
  int i,j,k;
  
  int num_left = mid-left+1;
  int num_right = right - mid;
  
  char** l_array = malloc((sizeof(char*) * num_left)+1);
  char** r_array = malloc((sizeof(char*) * num_right)+1);
  
  //putting the values in the array
   
  for(i = 0; i < num_left; i++)
  {
      l_array[i] = malloc(sizeof(char*)+600);
      strcpy(l_array[i],arr[left+i]);
  }
   for(i = 0; i < num_right; i++)
  {
      r_array[i] = malloc(sizeof(char*)+600);
      strcpy(r_array[i],arr[mid+1+i]);
  }
  
  i = 0;
  j = 0;
  k = left;
  
  //merge the arrays back to one
  while(i < num_left && j < num_right)
  {
      if(strcmp(l_array[i],r_array[j]) < 0)
          strcpy(arr[k++],l_array[i++]);
      else
          strcpy(arr[k++],r_array[j++]);
  }
  while(i<num_left){
      strcpy(arr[k++],l_array[i++]);
  }
  while(j<num_right){
      strcpy(arr[k++],r_array[j++]);
  }

}







