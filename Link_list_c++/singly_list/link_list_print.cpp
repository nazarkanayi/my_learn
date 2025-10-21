#include <iostream>



class Node 
{

    public:

    int data;

    Node *next;

    Node(int a)
    {
        data = a;
        next = nullptr;
    }
};


void PrintList(Node *Head)
{
    while(Head != nullptr)
    {
        std::cout << Head->data << " ";
        Head = Head->next;
    }

    std::cout << std::endl;
}

void ReverseList(Node **Head)
{

    Node * prev = nullptr;

    Node * current = *Head;

    Node* next = nullptr;

    while(current != nullptr)
    {
        next = current->next;

        current->next = prev;

        prev = current;

        current = next;

    }

    *Head = prev;

}


int main()
{
    Node * Head = new Node(1);

    Head->next = new Node(2);

    Head->next->next = new Node(3);

   PrintList(Head);

   ReverseList(&Head);


   PrintList(Head);

}