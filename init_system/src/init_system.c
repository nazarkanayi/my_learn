/*
 ============================================================================
 Name        : init_system.c
 Author      : nazar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "init_system.h"

#include "init_config.h"


Process_run_t ps_data[MAX_PROCESS];


int spawn_process(char *path, char * argv_list[])
{
	pid_t  pid;
	int ret;
	 pid = fork();

	   if (pid < 0){


	      printf("can't fork, error occured\n");
	      return -1;
	   }
	   else if (pid == 0)
	   {
		   ret= execv(path,argv_list);

		   exit(-1);

	   }
	   else
	   {
          return pid;
	   }
}

void init_list(void)
{
int i;

for(i=0;i<MAX_PROCESS;i++)
{
	ps_data[i].process_data.bin_path=list_p[i][0];
	ps_data[i].process_data.argument_list=&list_p[i][1];
}


for(i=0;i<MAX_PROCESS;i++)
{
	printf("%s , %s \n",ps_data[i].process_data.bin_path,*(ps_data[i].process_data.argument_list));
}
}

int main(void)
{
	int ret;
	int i;

	init_list();
	for(i=0;i<MAX_PROCESS;i++)
	{
		ret=spawn_process(ps_data[i].process_data.bin_path,ps_data[i].process_data.argument_list);
		printf("id %d\n",ret);
	}


	return EXIT_SUCCESS;
}
