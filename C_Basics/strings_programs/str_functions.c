






#include <stdio.h>



my_strcpy(char * dest, char * source)
{
    while(*source)
    {
        *(dest++) = *(source++);
    }

    *dest = '\0';
}



int main()
{


}