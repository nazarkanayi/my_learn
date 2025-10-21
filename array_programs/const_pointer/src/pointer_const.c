/*
 ============================================================================
 Name        : pointer_const.c
 Author      : nazar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void sample(void)
{
	printf("sample\n");
}
int main(void) {

 int a=10;

  int * p;

 const int * const q=&a;

 p=&a;
// q=&a;
 printf(" %p %d \n",p,*p);
 *(p++);

 printf(" %p %d \n",p,*p);


 void (*funt_ptr[])(void) = {sample ,sample,sample};

 funt_ptr[0]();
 funt_ptr[1]();
 funt_ptr[2]();
 return 0;

}
