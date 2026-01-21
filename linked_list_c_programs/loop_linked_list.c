

#include <stdio.h>
#include <stdlib.h>

struct Node
{

     int data;

     struct Node* next;
};

struct Node* CreateNOde(int val)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));

    node->data = val;

    node->next = NULL;

    return node;
}

void printNode(struct Node* Head )
{

    printf("List: ");
    while(Head)
    {
        printf("%d ",Head->data);

        Head= Head->next;
    }

    printf("\n");
}


int Detectloop(struct Node* Head)
{
    struct Node* slow = Head;

    struct Node* fast = Head;

    int cycle =0;
    int status= 0;
    while(fast && fast->next)
    {
        printf("cycle %d \n",++cycle);
        slow=  slow->next;

        fast= fast->next->next;

        if(fast == slow)
        {
            printf("loop detected\n");
            status =1;
            break;
        }

        
    }

    return status;
}

void PrintLoopElement(struct Node* Head)
{
    struct Node* slow = Head;

    struct Node* fast = Head;

    int cycle =0;
    int status= 0;
    while(fast && fast->next)
    {
        printf("cycle %d \n",++cycle);
        slow=  slow->next;

        fast= fast->next->next;

        if(fast == slow)
        {
            printf("loop detected\n");
            
            slow = Head;

            while(slow != fast)
            {
                slow= slow->next;
                fast= fast->next;
            }

            printf("loop element %d\n",slow->data);

            break;
        }

        
    }

}

void RemoveLoop(struct Node* Head)
{
    struct Node* slow = Head;

    struct Node* fast = Head;

    int cycle =0;
    int status= 0;
    while(fast && fast->next)
    {
        printf("cycle %d \n",++cycle);
        slow=  slow->next;

        fast= fast->next->next;

        if(fast == slow)
        {
            printf("loop detected\n");
            
            slow = Head;

            if(slow == fast)
            {
                printf("loop detected at head %d\n",slow->data);
                while(slow != fast->next)
                {
                    fast=  fast->next;
                }
            }
            else
            {
                while(slow->next != fast->next)
                {
                    fast=  fast->next;
                    slow=  slow->next;
                }

                printf("loop element %d\n",fast->next->data);
            }


            fast->next=NULL;

            break;
        }

        
    }
}

int main()
{

  struct Node* Head =   CreateNOde(1);

  Head->next= CreateNOde(2);

  Head->next->next= CreateNOde(3);

  Head->next->next->next= CreateNOde(4);

  Head->next->next->next->next = CreateNOde(5);

  Head->next->next->next->next->next = CreateNOde(6);

  Head->next->next->next->next->next->next = Head;

//   //PrintLoopElement(Head);

//   if(!Detectloop(Head))
//   {
//       printNode(Head);
//   }

RemoveLoop(Head);

printNode(Head);

}