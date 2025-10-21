/*
 * ringBuff.c
 *
 *  Created on: 08-Oct-2022
 *      Author: nazar
 */


#include "ringBuff.h"


void ring_buff_init(SharedBMem_t  * Buf_p, int size)
{

	sem_init(&Buf_p->SemSig,1,0);

	Buf_p->RingBuf.buf_size=size;
}


void ringBuffWrite(SharedBMem_t * SharedBMem_t, uint32_t data)
{
	RingBuff_t * RinBuff= &SharedBMem_t->RingBuf;

	if(RinBuff->data_count == RinBuff->buf_size)
	{
		printf("buffer full");

		return;
	}
	RinBuff->buffer[RinBuff->wr_count]=data;

	RinBuff->wr_count++;

	RinBuff->data_count++;
	RinBuff->wr_count= RinBuff->wr_count % RinBuff->buf_size;

	sem_post(&SharedBMem_t->SemSig);

}

void ringBuffRead(SharedBMem_t * SharedBMem_t, uint32_t *data)
{
	RingBuff_t * RinBuff= &SharedBMem_t->RingBuf;

	printf(" wr %d rd %d \n",RinBuff->wr_count,RinBuff->read_count);
    if(RinBuff->read_count== RinBuff->wr_count)
    {
    	printf("no data in buf");

    	return;
    }


    *data=RinBuff->buffer[RinBuff->read_count];

    RinBuff->data_count--;

    RinBuff->read_count++;
    RinBuff->read_count= RinBuff->read_count % RinBuff->buf_size;


}
