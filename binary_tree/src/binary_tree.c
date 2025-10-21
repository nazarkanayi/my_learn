/*
 ============================================================================
 Name        : binary_tree.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>


struct tree
{
	struct tree * left;
	int a;
	struct tree *right;
};


struct node
{
	int data;

	struct node *next;
};

void printree(struct tree * head);
void buildtree(struct tree ** head,int num);


void insert_node(struct node ** head,int num)
{

	struct node *tmp1= (struct node *)malloc(sizeof(struct node));

	tmp1->data=num;
	tmp1->next=NULL;

	if(*head==NULL)
	{
		*head= tmp1;



	}
	else
	{
		struct node *tmp = *head;

		while(tmp->next !=NULL)
		{
			tmp=tmp->next;
		}

		tmp->next=tmp1;

	}
}

void printnode(struct node *tmp)
{
	while(tmp !=NULL)
	{
		printf("%d ",tmp->data);

		tmp=tmp->next;
	}
}
void buildtree(struct tree ** head,int num)
{


	if(*head ==NULL)
	{
		struct tree * tmp= (struct tree *)malloc(sizeof(struct tree));

		tmp->a=num;
		tmp->left=NULL;
		tmp->right=NULL;


		*head=tmp;
	}
	else
	{
		if((*head)->a > num)
		{
			buildtree(&(*head)->left,num);
		}
		else
		{
			buildtree(&(*head)->right,num);
		}
	}


}

void printree(struct tree * head)
{
	if(head!=NULL)
	{
		printree(head->left);

		printf("%d ",head->a);

		printree(head->right);
	}

}


void searchtree(struct tree * head,int num,struct tree ** parent,struct tree ** node,int *found)
{
	struct tree *tmp=head;
	struct tree * ptmp=NULL;

	while(tmp!=NULL)
	{
		if(tmp->a==num)
		{
			*found=1;

			*node=tmp;
			*parent=ptmp;

			return;
		}

		ptmp=tmp;

		if(num < tmp->a)
		{
			tmp=tmp->left;
		}
		else
		{
			tmp=tmp->right;
		}
	}


}

void deletetree(struct tree * head,int num)
{
	struct tree *dnode=NULL;
	struct tree * parent=NULL,*xsucc=NULL;

	int found=0;

	searchtree(head,num,&parent,&dnode,&found);


	if(!found)
	{
		printf("no data");
	}

	if(dnode->left!=NULL&& dnode->right!=NULL )
	{
		parent=dnode;
		xsucc=dnode->right;

		while(xsucc->left !=NULL)
		{
			parent=xsucc;
			xsucc=xsucc->left;
		}


		dnode->a=xsucc->a;
		dnode=xsucc;


	}

	if(dnode->left==NULL&& dnode->right==NULL)
	{
		if(parent->left==dnode)
		{
			parent->left=NULL;
		}
		else
		{
			parent->right=NULL;
		}

		free(dnode);

		return;
	}

	if(dnode->left==NULL&& dnode->right!=NULL)
	{
		if(parent->left==dnode)
		{
			parent->left=dnode->right;
		}
		else
		{
			parent->right=dnode->right;;
		}

		free(dnode);

		return;
	}

	if(dnode->left!=NULL&& dnode->right==NULL)
	{
		if(parent->left==dnode)
				{
					parent->left=dnode->left;
				}
				else
				{
					parent->right=dnode->left;;
				}

				free(dnode);

				return;
	}
}
int main(void) {

	struct tree * head=NULL;

	struct node * lhead=NULL;

	buildtree(&head,2);

	buildtree(&head,6);
	buildtree(&head,1);
	buildtree(&head,5);

	printree(head);

	printf("\n");

	deletetree(head,1);

	printree(head);

	//insert_node(&lhead,3);

	//insert_node(&lhead,16);
	//insert_node(&lhead,5);
	//insert_node(&lhead,10);

	//printnode(lhead);

	return EXIT_SUCCESS;
}
