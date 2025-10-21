/*
 * init_config.h
 *
 *  Created on: 28-Jun-2020
 *      Author: nazar
 */

#ifndef INIT_CONFIG_H_
#define INIT_CONFIG_H_


#include  "init_system.h"

#define MAX_PROCESS 2

char *list_p[MAX_PROCESS][4]=
{
		{"/bin/ls","ls","/home/nazar",NULL},
		{"/bin/ls","ls","/home/nazar/my_study",NULL}
};



#endif /* INIT_CONFIG_H_ */
