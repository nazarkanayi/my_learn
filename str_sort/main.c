/*
 * main.c
 *
 *  Created on: 22-Jun-2018
 *      Author: nazar
 */


#include <stdio.h>
#include <string.h>

void str_sort(char *names[],int len)
{
	int i,j;
  char *tmp;

	for(i=0;i<(len-1);i++)
	{
		for(j=i+1;j<len;j++)
		{

			if(strcmp(names[i],names[j])>0)
			{
				tmp=names[i];
				names[i]=names[j];
				names[j]=tmp;
			}
		}
	}
}

int main(int argc,char *name[])
{
	char *names[3]={"nazar","shafi","haneefa"};

	str_sort(names,3);

	for(int i=0;i<3;i++)
	{
		printf("%s ",names[i]);
	}


	return 0;
}
