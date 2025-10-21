/*
 ============================================================================
 Name        : data_type.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"


struct bit
{
	uint8_t b1:1;
	uint8_t b2:1;
	uint8_t b3:2;
	uint8_t b4:1;
	uint8_t b5:1;
	uint8_t b6:2;
    uint8_t reserve[3];

};


union Reg_t
{
	struct bit bits;
	uint16_t shorts[2];
	uint32_t word;
};

void sample(void)
{
	printf("sample\n");
}



void (*funt_ptr[])(void) = {sample ,sample,sample};


int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */


	uint32_t val=0x1122;
	union Reg_t* reg_p=&val;

	reg_p->shorts[1]=0x3322;



	printf("%x",val);


	return EXIT_SUCCESS;
}
