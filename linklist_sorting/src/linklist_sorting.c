/*
 ============================================================================
 Name        : linklist_sorting.c
 Author      : nazar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

struct node
{
	int value;
	struct node *next;
};

void remove_duplicate(struct node *list)
{



	while(list->next !=NULL)
	{
		if(list->value == list->next->value)
		{
			if(list->next->next == NULL)
			{
				list->next=NULL;
				break;

			}
			list->next=list->next->next;


		}
		else
		{
			list=list->next;
		}

	}
}
void selection_sort(struct node *list)
{
	int len=0,i,j,tmp;
	struct node *list_head;
	list_head=list;
	struct node *list_1;
    while(list != NULL)
    {
    	len++;

    	list=list->next;
    }

    list_1=list_head;

  for(i=0;i<len-1;i++)
  {
	  list=list_1->next;

	  for(j=i+1;j<len;j++)
	  {
		  if(list_1->value > list->value)
		  {
			  tmp =list->value;
			  list->value=list_1->value;

			  list_1->value=tmp;
		  }

		  list=list->next;

	  }

	  list_1=list_1->next;
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

int main(void)
{
	struct node *list_1=NULL,*list_1_head=NULL;

	struct node *list_2=NULL,*list_2_head=NULL;



	struct node *tmp_list;
	char buffer[20];
	int num;
	printf(" enter first list\n");

	while (1)
	{
		fgets(buffer,sizeof(buffer),stdin);

		if(buffer[0] =='\n')
		{
			break;
		}


		if(1==sscanf(buffer, "%d", &num))
		{

			tmp_list= (struct node *)malloc(sizeof(struct node));

			if(tmp_list == NULL)
			{
				printf("malloc error \n");

			}
			tmp_list->value=num;
			tmp_list->next=NULL;

			if(list_1_head == NULL)
			{
				list_1_head=tmp_list;
				list_1=tmp_list;

			}
			else
			{
				list_1->next=tmp_list;
				list_1=list_1->next;
			}
		}
		else
		{
			printf("sscanf error \n");
		}


	}

	printf(" enter second list\n");

		while (1)
		{
			fgets(buffer,sizeof(buffer),stdin);

			if(buffer[0] =='\n')
			{
				break;
			}

			if(1==sscanf(buffer, "%d", &num))
			{
				tmp_list= (struct node *)malloc(sizeof(struct node));

				tmp_list->value=num;
				tmp_list->next=NULL;

				if(list_2_head == NULL)
				{
					list_2_head=tmp_list;
					list_2=tmp_list;
				}
				else
				{
					list_2->next=tmp_list;
					list_2=list_2->next;
				}
			}

		}


		print_list(list_1_head);
		print_list(list_2_head);

		list_1->next=list_2_head;

		print_list(list_1_head);

		selection_sort(list_1_head);

		print_list(list_1_head);

		remove_duplicate(list_1_head);
		print_list(list_1_head);

	return EXIT_SUCCESS;
}
