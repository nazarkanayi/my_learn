/*
 ============================================================================
 Name        : string_a_ab.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strshift(char *str)
{
	int len=strlen(str),i;

	str=str+len-1;
	i=len;
	while(i>0)
	{
		*(str+1)= *str;
		i--;
		str--;
	}

	*(str+len+3)=0;
}
int main(void) {
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	char data[100]="aaaaaaaaaaaaaab";
	volatile int len;
	len=strlen(data);

	for(int i=0;i<len;i++)
	{

         if(data[i]=='a')
         {
        	 strshift(&data[i+1]);
        	 data[i+1]='b';
        	 len++;
        	 i++;
         }
	}

	printf("%s",&data[0]);

	return EXIT_SUCCESS;
}
