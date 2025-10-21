/*
 ============================================================================
 Name        : shared_memory_buf_sender.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */



#include "ringBuff.h"


int main(void) {

	int shm_fd;
	SharedBMem_t * ptr;
	int i=0;
	/* create the shared memory object */
	shm_fd = shm_open(SHARED_MEMORY, O_CREAT | O_RDWR, 0666);

	printf("shm_fd %d",shm_fd);
	/* configure the size of the shared memory object */
	ftruncate(shm_fd, sizeof(SharedBMem_t));

	/* memory map the shared memory object */
	ptr = (SharedBMem_t * )mmap(0, sizeof(SharedBMem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	ring_buff_init(ptr,SIZE);

	while(1)
	{
		printf("write\n");
		ringBuffWrite(ptr,i);
		i++;
		fflush(stdout);
		sleep(1);
	}

	return EXIT_SUCCESS;
}
