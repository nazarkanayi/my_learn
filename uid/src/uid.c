/* CELEBS11

   This example changes the effective UID.

 */
#define _POSIX_SOURCE
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
int main(int argc,char* argv[]) {
	int opt, suid = getuid(), /* this is the saved uid */
	        uid = 0;
	    while ((opt = getopt(argc, argv, "i:")) != EOF) {
	        switch (opt) {
	        case 'i': uid = atoi(optarg); break;
	        }
	    }
	    /* execute this program with root privileges, like setuid root, for example */
	    printf("real uid=%d; effective uid=%d\n", getuid(), geteuid());
	    seteuid(uid);  /* change to the non-privileged id configured */
	    printf("real uid=%d; effective uid=%d\n", getuid(), geteuid());
	    seteuid(0); /* return back to saved uid */
	    printf("real uid=%d; effective uid=%d\n", getuid(), geteuid());
}
