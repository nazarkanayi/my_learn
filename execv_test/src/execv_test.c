#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


//extern char **environ;


/*int main(int argc, char *argv[])
{
	int j;
	char **ep;
	for (j = 0; j < argc; j++)
		printf("argv[%d] = %s\n", j, argv[j]);
	for (ep = environ; *ep != NULL; ep++)
		printf("environ: %s\n", *ep);

	execve();
	exit(EXIT_SUCCESS);
}*/

#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char *argv[] = { "/usr/bin/ls", 0 };
    char *envp[] =
    {
        "HOME=/",
        "PATH=/bin:/usr/bin",
        "TZ=UTC0",
        "USER=nazar",
        "LOGNAME=nazar",
        0
    };
    execve(argv[0], &argv[0], envp);
    fprintf(stderr, "Oops!\n");
    return -1;
}
