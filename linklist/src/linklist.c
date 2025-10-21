/*
 ============================================================================
 Name        : linklist.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>


struct node
{
	int data;
	struct node *next;
};


static void display(struct node *p)
{
	while(p!=NULL)
	{
		printf("%d",p->data);
		p=p->next;
	}
}

static reverse(struct node **x)
		{

	       struct node *q,*r,*s;

	       q=*x;

	       r=NULL;

	       while(q!=NULL)
	       {
              s=r;
              r=q;
              q=q->next;
              r->next=s;
	       }
	       *x=r;
		}

int main(void) {





	return EXIT_SUCCESS;
}
