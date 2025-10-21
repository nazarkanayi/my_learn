#include <stdio.h>


int main() {
    

    // Using macro to calculate square
    #define TEST(a) (#a)


    #define TEST2(a) (a##a)

    printf("Square of 5: %s , %d\n", TEST(5),TEST2(5));

    return 0;
}