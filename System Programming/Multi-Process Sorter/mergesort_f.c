/*
*********************************************
*  213 System Programming                   *
*  Seong Kon Kim                            *
*  Oct 2,2017                               *
*  mergesort_f.c                            *
*********************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mergechar_f(float*,int,int);
void merge_f(float*,int,int,int);

void mergechar_f(float* arr,int left,int right){

	if (left < right) {
		int mid = (left+right)/2;
		mergechar_f(arr,left,mid);
    mergechar_f(arr,mid+1,right);
    merge_f(arr,left,mid,right);;
	}
}


void merge_f(float* arr, int left, int mid, int right)
{
  int i,j,k;
  
  int num_left = mid-left+1;
  int num_right = right - mid;
  
  float *l_array = malloc((sizeof(float) * num_left) + 1);
  float *r_array = malloc((sizeof(float) * num_right) + 1);
  
  //putting the values in the array
   
  for(i = 0; i < num_left; i++)
  {
      l_array[i] = arr[left+i];
  }
   for(i = 0; i < num_right; i++)
  {
      r_array[i] = arr[mid+1+i];
  }
  
  i = 0;
  j = 0;
  k = left;
  
  //merge the arrays back to one
  while(i < num_left && j < num_right)
  {
      if(l_array[i] < r_array[j]){
        arr[k++] = l_array[i++];
      }else{
        arr[k++] = r_array[j++];
      }
  }
  while(i<num_left){
      arr[k++] = l_array[i++];
  }
  while(j<num_right){
      arr[k++] = r_array[j++];
  }

}
