


#include <stdio.h>
#include <string.h>



void replace_ab_a(char *str)
{
    char * rd_ptr = str;
    char * wr_ptr = str;

    while(*rd_ptr != 0)
    {
        if(*rd_ptr == 'a' && *(rd_ptr+1) == 'b' )
        {
           *wr_ptr = 'a';
            
            wr_ptr++;

            rd_ptr= rd_ptr+2;

        }
        else
        {
           *wr_ptr = *rd_ptr;
           wr_ptr++;
           rd_ptr++;
        }
    }

    *wr_ptr = '\0';
}

int main()
{

     char str[100];
    printf("enter string\n");

    scanf("%s",str);

    replace_ab_a(str);

    printf("%s\n",str);


}