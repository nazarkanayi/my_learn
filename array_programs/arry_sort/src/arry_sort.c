/*
 ============================================================================
 Name        : arry_sort.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>


void buble_sort(int *arr,int count)
{
	int i,j,tmp;

	for(i=0;i<(count-1);i++)
	{
		for(j=0;j<(count-1-i);j++)
		{
			if(arr[j]>arr[j+1])
			{
				tmp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=tmp;
			}
		}
	}

}

void selection_sort(int *arr,int count)
{
 int i,j,tmp;

 for(i=0;i<(count-1);i++)
 {
      for(j=i+1;j<count;j++)
      {
         if(arr[i]>arr[j])
         {
        	 tmp=arr[i];
        	 arr[i]=arr[j];
        	 arr[j]=tmp;
         }
      }
 }

}

int main(void) {

	int arry[5]={26,87,3,43,67};

	selection_sort(arry,5);
	for(int i=0;i<5;i++)
	{
		printf("%d ",arry[i]);
	}
	printf("\n");
	return EXIT_SUCCESS;
}
