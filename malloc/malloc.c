
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  void* ptr = malloc(135150); // Allocate 132 KB
  printf("Allocated memory at: %p\n", ptr);

//   void* ptr = malloc(130*1024); // Allocate 130 KB
//   printf("Allocated memory at: %p\n", ptr);

//   ptr = malloc(130*1024); // Allocate 130 KB

// printf("Allocated memory at: %p\n", ptr);
//    ptr = malloc(130*1024); // Allocate 130 KB
//    printf("Allocated memory at: %p\n", ptr);

//     ptr = malloc(130*1024); // Allocate 130 KB
// printf("Allocated memory at: %p\n", ptr);
//      ptr = malloc(130*1024); // Allocate 130 KB
//      printf("Allocated memory at: %p\n", ptr);
//     //free(ptr);
//     printf("Memory freed\n");
    return 0;
}