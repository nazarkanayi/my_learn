/*
 ============================================================================
 Name        : pthread_new.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

void *threadfun(void *arg)
{
	printf("msg:%s\n",(char *)arg);

	return NULL;
}
/// @brief 
/// @param arg 
/// @return 
void *signal_hdl(void *arg)
{
	sigset_t set;
	    sigemptyset(&set);
	    sigfillset(&set);

	    siginfo_t info;
	    printf("signal handler\n");
	    while(1) {
	      sigwaitinfo(&set,&info);

	        printf("\nSIGINT received %d \n",info.si_signo);



	    }

	return NULL;
}

int main(void) {

	pthread_t thread1, thread2,thread3;
	char *message1 = "Thread 1";
	char *message2 = "Thread 2";

	sigset_t set;

	sigfillset(&set);

	pthread_sigmask(SIG_BLOCK, &set, NULL);


	pthread_create(&thread1,NULL,signal_hdl,NULL);

	pthread_create(&thread2,NULL,threadfun,message1);

	pthread_create(&thread3,NULL,threadfun,message2);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	pthread_join(thread3,NULL);


	return EXIT_SUCCESS;
}
