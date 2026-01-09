

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <stddef.h>

struct Node_t
{
    int data;
    struct Node_t* Next;
};


typedef struct Node_t Node;


void AddToFront(Node ** Head, int data, Node ** Tail)
{
    Node * tmp = malloc(sizeof(Node));
    tmp->data = data;
    tmp->Next = *Head;
    *Head = tmp;
    
    if ((*Head)->Next == NULL)
        *Tail = *Head;
}

void AddToBack(Node ** Head, int data, Node ** Tail)
{
    Node * tmp = malloc(sizeof(Node));
    tmp->data = data;
    tmp->Next = NULL;
    
    if (*Tail != NULL)
        (*Tail)->Next = tmp;
    else
        *Head = tmp;
    
    *Tail = tmp;
}


bool remove_entry(Node ** Head, Node ** Tail, int num)
{
    Node ** indirect = Head;

    while(*indirect && (*indirect)->data != num)
        indirect = &(*indirect)->Next;

    if(*indirect == NULL)
        return false;

    Node * tmp = *indirect;
    *indirect = (*indirect)->Next;

    if(*Tail == tmp)
        *Tail = indirect == Head ? NULL : (Node *)((char *)indirect - offsetof(Node, Next));

    free(tmp);
    return true;
}


/**
 * remove_entry_noref - Remove a node with specified data from the linked list
 * @Head: Pointer to the head of the linked list
 * @Tail: Pointer to the tail of the linked list
 * @num: The data value to search for and remove
 *
 * Return: true if entry was found and removed, false otherwise
 */
bool remove_entry_noref(Node ** Head, Node ** Tail, int num)
{
    Node * current = *Head;
    Node * previous = NULL;

    while(current && current->data != num)
    {
        previous = current;
        current = current->Next;
    }

    if(current == NULL)
        return false;

    if(previous == NULL)
        *Head = current->Next;
    else
        previous->Next = current->Next;

    if(*Tail == current)
        *Tail = previous;

    free(current);
    return true;
}

void PrintLst(Node * Head)
{
    for(Node * current = Head; current != NULL; current = current->Next)
        printf("%d ", current->data);
    printf("\n");
}

int main()
{
Node* HeadOfList=NULL;

Node* TailOfList=NULL;

AddToBack(&HeadOfList,20,&TailOfList);
AddToFront(&HeadOfList,10,&TailOfList);
AddToFront(&HeadOfList,11,&TailOfList);
AddToFront(&HeadOfList,12,&TailOfList);
AddToFront(&HeadOfList,13,&TailOfList);
AddToFront(&HeadOfList,14,&TailOfList);
AddToFront(&HeadOfList,15,&TailOfList);


AddToBack(&HeadOfList,21,&TailOfList);
AddToBack(&HeadOfList,22,&TailOfList);
AddToBack(&HeadOfList,23,&TailOfList);
AddToBack(&HeadOfList,24,&TailOfList);
AddToBack(&HeadOfList,25,&TailOfList);


PrintLst(HeadOfList);

remove_entry(&HeadOfList,&TailOfList,25);

remove_entry(&HeadOfList,&TailOfList,15);


PrintLst(HeadOfList);
    
    return 0;
}