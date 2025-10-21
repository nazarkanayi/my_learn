/*
 ============================================================================
 Name        : Sort_Arry_and_Linklist.c
 Author      : nazar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define ARRAY_MAX  10
int array[ARRAY_MAX]={0};
int arry_count =0;


void selection_sort(int *arr,int len)
{
	int i,j,tmp;

	for(i=0;i <len-1;i++)
	{
		for (j=i+1; j<len ;j++)
		{
			if(arr[i] < arr[j])
			{
				tmp =arr[i];
				arr[i]=arr[j];
				arr[j]=tmp;

			}
		}
	}


}

void insert_ascending(int num)
{
	int i=0,pos;
  if(arry_count >= ARRAY_MAX)
  {
	  printf("array max \n");

	  return;
  }


   for(i=0;i<arry_count ;i++)
   {
	   if(num <= array[i])
	   {
		   break;
	   }
   }

  pos=i;
  arry_count++;

  for(i=arry_count -1; i>pos ;i--)
  {
	  array[i]=array[i-1];
  }


  array[pos]=num;


}

int main(void) {

	int i=0,num;
	char input[32];
	printf("enter number \n");

	while(i < ARRAY_MAX)
	{
		fgets(input,sizeof(input),stdin);
		if(*input == '\n')

		  break;
		 if(1==sscanf(input, "%d", &num))
			 insert_ascending(num);

	}


	for(i=0;i<arry_count;i++)
	{
		printf("%d ",array[i]);
	}

	printf("\n");

	selection_sort(array,arry_count);

	for(i=0;i<arry_count;i++)
		{
			printf("%d ",array[i]);
		}

	return EXIT_SUCCESS;
}
