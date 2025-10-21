/*
 ============================================================================
 Name        : str_max_occur_char.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char maxi_occur_char(char *str)
{
	int i;
	char table[256]={0};
	int len=strlen(str);
	int max=-1;
	char result;
	for(i=0;i<len;i++)
	{
		table[(int)str[i]]++;
	}

	for(i=0;i<len;i++)
	{
		if(max<table[(int)str[i]])
		{
			max=table[(int)str[i]];
			result=str[i];
		}
	}
	return result;
}

int main(void) {

	char arry[]="nazafffffffffr";
	printf("%c ",maxi_occur_char(arry));
	return EXIT_SUCCESS;
}
