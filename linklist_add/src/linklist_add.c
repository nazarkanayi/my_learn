#include <stdio.h>
#include <stdlib.h>

struct node
{
	int value;

	struct node *next;
};


void insert(struct node **list, int a)
{

	struct node *tmp,*local_list,*prev=NULL;

	local_list= *list;

	tmp= (struct node *)malloc(sizeof(struct node));

	tmp->value=a;
	tmp->next=NULL;


	if(*list ==NULL )
	{


		*list=tmp;

	}
	else
	{
		while((local_list!=NULL) && (a > local_list->value))
		{

			prev=local_list;
			local_list=local_list->next;
		}

		tmp->next =local_list;

		if(prev != NULL)
		{
			prev->next=tmp;
		}
	}
}

void print_list(struct node *list)
{


	while(list !=NULL)
	{
		printf("%d ",list->value);
		list=list->next;
	}

	printf("\n");
}

int main(int argc, char **argv)
{

	char buff[20];
	struct node *head=NULL;
	int tmp;
	printf("enter element\n");

	while(1)
	{
		fgets(buff,sizeof(buff),stdin);

		if(buff[0]=='\n')
		{
			break;
		}

		if(sscanf(buff,"%d",&tmp)==1)
		{
			insert(&head,tmp);

		}

	}

	print_list(head);

	return 0;
}
