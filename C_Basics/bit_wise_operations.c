#include <stdint.h>
#include <stdio.h>

void printBinary(uint32_t n)
{
    for (int i = 31; i >= 0; i--)
    {
        printf("%d", (n >> i) & 1);
    }
}

uint32_t swapBits(uint32_t n, int p1, int p2)
{
    // Extract the bits to be swapped
    uint32_t set1 = (n >> p1) & 1U;
    uint32_t set2 = (n >> p2) & 1U;

    // XOR the two sets of bits
    uint32_t xorBits = set1 ^ set2;

    // Put the xorBits back to their original positions
    xorBits = (xorBits << p1) | (xorBits << p2);

    // XOR the original number with xorBits to swap the bits
    return n ^ xorBits;
}


uint32_t clear_multiple_bits(uint32_t n, int p1, int p2)
{
    uint32_t mask1 = (1U << p1);
    uint32_t mask2 = (1U << p2);
    return n & ~(mask1 | mask2);
}


uint32_t reverse_bits(uint32_t n)
{
    uint32_t reversed = 0;
    for (int i = 0; i < 32; i++)
    {
        reversed <<= 1;
        reversed |= (n & 1);
        n >>= 1;
    }
    return reversed;
}





int main()
{
    uint32_t n = 0b11110000111100001111000011110000;
    int p1 = 0;      // Starting position of first set of bits
    int p2 = 4 ;    // Starting position of second set of bits
    int numBits = 4; // Number of bits to swap      
    uint32_t result = swapBits(n, p1, p2);
    printf("Original number: 0b");
    printBinary(n);
    printf("\n");
    printf("Number after swapping bits: 0b");
    printBinary(result);
    printf("\n");

    printf("Original number: 0b");
    printBinary(0x20202020);
    printf("\n");
    result =reverse_bits(0x20202020);
    printf("Number after reversing bits: 0b");
    printBinary(result);
    printf("\n");   



    return 0;
}