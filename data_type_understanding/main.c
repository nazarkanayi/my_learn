/*
 * main.c
 *
 *  Created on: 10-Jun-2018
 *      Author: nazar
 */


#include <stdio.h>
#include <stdint.h>

struct book
{
 short a;
 short b;
 int c;

 long d;
};

typedef struct book book_s;
//int a=10;


typedef union MyUnion
{
    struct MyStruct
    {
        uint16_t val_1;
        uint8_t  val_2;
        uint8_t  val_3;
    } data;
    uint8_t data_array[4];  // NB: was unint8_t!!!
} MyUnion;

struct bit
{
	uint8_t b0:1;
	uint8_t b2:1;
	uint16_t b3:10;
};

union reg
{
	struct bit bits;
	uint8_t byte[4];
	uint32_t word;
};




void sample(int ,int)
{

}
struct hnadler
{
	int a;
	void (*ptr)(int ,int);
};

void (*ptr[3])(int,int)={sample,sample,sample};
int main(void)
{
//	int a= 5;
//	volatile int *ptr= &a;
//	                *ptr++;
//	              //  p--;
//	                printf("%d %d",*ptr,a);

int i;
	book_s book={  .a = 4, .b=6};

printf("%d \n",book.a);


MyUnion uni;

uni.data.val_1=0x5665;

uni.data.val_2=0x77;

uni.data.val_3=0x88;
for(i=0;i<4;i++)
{
	printf(" %x ",uni.data_array[i]);
}

printf("\n");



union reg regi={};

regi.byte[0]=0xF;
regi.word=0x56565656;
printf("%x  %x %x \n",regi.bits.b0,regi.bits.b2,regi.bits.b3);

printf("%x ",regi.word);


struct hnadler hdl;

hdl.ptr=sample;



}
