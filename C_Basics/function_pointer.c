#include <stdio.h>


int add(int a, int b)
{
    return a+b;
}

//extern char __bss_start; 


typedef int (*callback) (int,int);

int main() {
    
    int (*func_ptr) (int,int);

    int (*operations[4])(int, int) ={add,add,add,add};

    callback cb;

    func_ptr = add;

    cb = add;

    

    printf("ADD %d %d %d\n",cb(1,1),func_ptr(1,1),operations[1](2,2));

    ////printf("BSS start address: %p ,%p\n", (void *)&__bss_start, __builtin_frame_address (0));

    return 0;
}