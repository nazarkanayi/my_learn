/*
 * ringBuff.h
 *
 *  Created on: 08-Oct-2022
 *      Author: nazar
 */

#ifndef RINGBUFF_H_
#define RINGBUFF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>

#define SHARED_MEMORY  "shared_memory"

#define SIZE  200

typedef struct RingBuff_s
{

	int read_count;
	int wr_count;
	int data_count;
	uint32_t buffer[SIZE];
	int buf_size;


}RingBuff_t;


typedef struct SharedBMem_s
{
 sem_t SemSig;
 RingBuff_t RingBuf;
}SharedBMem_t;

void ring_buff_init(SharedBMem_t  * Buf_p, int size);

void ringBuffWrite(SharedBMem_t * SharedBMem_t, uint32_t data);

void ringBuffRead(SharedBMem_t * SharedBMem_t, uint32_t *data);

#endif /* RINGBUFF_H_ */
