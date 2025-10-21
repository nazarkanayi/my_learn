/*
 ============================================================================
 Name        : array_test_sample.c
 Author      : nazar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {

	typedef enum
	{
		one,
		two
	}name_t;

	int arr[2][2]={{1,2},{3,4}};
	name_t  name;
	printf("%d\n",arr);

	printf("%d\n",&arr+1);

	printf("%d",sizeof(name_t));
	return 0;
}
