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


	uint32_t data=0;

	/* create the shared memory object */
	shm_fd = shm_open(SHARED_MEMORY, O_RDWR, 0666);
     if(shm_fd < 0)
     {
    	 printf("shm error\n");
     }
	 printf("shm_fd %d\n",shm_fd);
	/* configure the size of the shared memory object */


	/* memory map the shared memory object */
	ptr = (SharedBMem_t * )mmap(0, sizeof(SharedBMem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);




	while(1)
	{
		sem_wait(&ptr->SemSig);

		ringBuffRead(ptr,&data);

		printf("data %d\n",data);

		fflush(stdout);
		sleep(1);

	}

	return EXIT_SUCCESS;
}
