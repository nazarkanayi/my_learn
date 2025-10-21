/*
 * n-bus.h
 *
 *  Created on: 24-Oct-2021
 *      Author: nazar
 */

#ifndef SRC_N_BUS_H_
#define SRC_N_BUS_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>

typedef void (*nbus_callback_hdler)(int cmd,void *data);


int nbus_connect(char *client_name,nbus_callback_hdler hdler);


int nbus_sendmsg(int id,char * msg,int len);


int nbus_get_id(char *name);


int nbus_disconnect(void);


#endif /* SRC_N_BUS_H_ */
