#include <stdio.h>
#include <assert.h>



int main() {
    int arr[5] = {19, 2, 3, 4, 5};
    
    printf("%p \n", arr);

    printf("%p \n", &arr);

    printf("%d \n", *arr);

    printf("%d \n", *(arr + 1));




    printf("Pointer\n");

    int* ptr = arr;
    printf("%p \n", ptr);

    printf("%d \n", ptr[1]);

    printf("%d \n",(*ptr)++);

    printf("%p \n", ptr);

     printf("%d \n", *ptr);


    printf("two dimensional array\n");
    int arr2[2][3] = {{1, 2, 3},
                     {4, 5, 6}};

    printf("%d \n", arr2[0][1]);

    printf("%d \n", *(*(arr2 + 0) + 1));

    printf("%d \n", *(*(arr2 + 1) + 2));

    printf("Pointer to two dimensional array\n");

    int (*ptr_2)[3] = arr2;



    printf("%p \n", ptr_2);

    assert(ptr_2!=arr2);


    return 0;
}