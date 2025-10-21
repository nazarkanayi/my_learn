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


	i=len;
	while(i>1)
	{
		*(str)= *(str+1);
		i--;
		str++;
	}

	*(str)=0;
}
int main(void) {
	//puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	char data[100]="ababbabggggfffdfaaababababababababaaaabaaab";
	volatile int len;
	len=strlen(data);

	for(int i=0;i<len;i++)
	{

         if(data[i]=='a' && data[i+1]=='b' )
         {
        	 strshift(&data[i+1]);
        	 len--;

         }
	}

	printf("%s",&data[0]);

	return EXIT_SUCCESS;
}
