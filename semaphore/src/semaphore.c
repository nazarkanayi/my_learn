/*
 ============================================================================
 Name        : semaphore.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
static sem_t x,y,z;


void * thread1(void * arg)
{
	while(1)
	{
	printf("A ");
	fflush(stdout);
	sleep(1);
	sem_post(&x);
	sem_wait(&z);
	}

	return NULL;
}

void * thread2(void * arg)
{
	while(1)
		{

		sem_wait(&x);
		printf("B ");
		fflush(stdout);
        sleep(1);
		sem_post(&y);
		}

		return NULL;
}

void * thread3(void * arg)
{
	while(1)
			{

			sem_wait(&y);
			printf("C \n");
			fflush(stdout);
            sleep(1);
			sem_post(&z);
			}

			return NULL;
}

int main(void) {

;
	pthread_t t1,t2,t3;


	sem_init(&x,0,0);
	sem_init(&y,0,0);
	sem_init(&z,0,0);


	pthread_create(&t1,NULL,thread1,NULL);

	pthread_create(&t2,NULL,thread2,NULL);
	pthread_create(&t3,NULL,thread3,NULL);

    pthread_join(t1,NULL);
	return EXIT_SUCCESS;
}
