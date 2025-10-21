#include <stdio.h>
#include <unistd.h> // For getopt()

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "hv:o:")) != -1) {
        switch (opt) {
            case 'h':
                printf("Help message requested.\n");
                break;
            case 'v':
                printf("Verbose level: %s\n", optarg); // optarg holds the argument for the option
                break;
            case 'o':
                printf("Output file: %s\n", optarg);
                break;
            case '?': // Unrecognized option or missing argument
                fprintf(stderr, "Usage: %s [-h] [-v value] [-o file]\n", argv[0]);
                return 1;
        }
    }

    // Process remaining non-option arguments
    for (int i = optind; i < argc; i++) {
        printf("Non-option argument: %s\n", argv[i]);
    }

    return 0;
}