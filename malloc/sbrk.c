#include <unistd.h>
#include <stdio.h>

int main() {
    void* a = sbrk(0);
    printf("Initial brk: %p\n", a);

    sbrk(100);  // request 10 bytes

    sbrk(100);

    // sbrk(1);

    void* b = sbrk(0);
    printf("After sbrk(1): %p\n", b);

    printf("Increase: %ld bytes\n", (char*)b - (char*)a);
    return 0;
}

