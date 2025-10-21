/*
 ============================================================================
 Name        : str_palindrm.c
 Author      : nazar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_palindrom(char *start,char *end)
{
	char *l_start,*l_end;

	l_start=start;

	l_end=end;

	while(l_start < l_end)
	{
		if(*l_start != *l_end)
		{
			return;
		}

		l_start++;
		l_end--;
	}

   for(l_start=start; l_start <=end;l_start++)
   {
      putchar(*l_start);
   }

   printf("\n");
}

int main(void) {

   char name[30];

   printf("enter string\n");

   scanf("%s",name);

   char *start;

   char *end;

   start=name;
   end=&name[strlen(name)-1];
   char *local_end;
   for(;start <end;start++)
   {
	   for(local_end=end;start<local_end; local_end--)
	   {
		   print_palindrom(start,local_end);

	   }
   }





   //print_palindrom(name,&name[strlen(name)-1]);


	return EXIT_SUCCESS;
}
