

#include <stdio.h>
int main()
{

int pos1,pos2,tmp,i;

unsigned int value;

printf("fisrt position should be LSB \n");
printf("enter position1 \n");

scanf("%d",&pos1);

printf("enter position2 \n");

scanf("%d",&pos2);

printf("enter value \n");

scanf("%d",&tmp);



value=(unsigned int)tmp;

printf("entered value %x\n",value);

for(i=0;i<sizeof(int)*8;i++)
{

	if((i >= pos1) &&   (i <= pos2))
	{
		value= value ^ (1 <<i);




	}


}

 printf("final value = %x",value);


return 0;

}
