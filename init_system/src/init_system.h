

#ifndef INIT_SYSTEM_H
#define INIT_SYSTEM_H


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

typedef struct
{
	char *bin_path;
	char **argument_list;


}Process_t;

typedef struct
{
	Process_t process_data;
	pid_t  pid;
}Process_run_t;

#endif
