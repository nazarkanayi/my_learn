

#include <iostream>

template<typename T>
class SinglyLIst
{
public:
    struct Node
    {
       T data;
       Node* Next;

       ~Node()
       {
        std::cout << "Destruct " << data << "\n";
       }
    };


    SinglyLIst(T&& Data): Head{nullptr}, Tail{nullptr}
    {
       Head = new Node();

       Head->data = Data;
       Head->Next = nullptr;

       Tail = Head;

    }

    void InsertToList(T&& Data)
    {
        Tail->Next = new Node();
        Tail= Tail->Next;
        Tail->Next =nullptr;
        Tail->data = Data;
    }

    void showList() const
    {
        Node* current = Head;

        while(current != nullptr)
        {
            std::cout << current->data << " ";
            current = current->Next;
        }

        std::cout << std::endl;
    }

    ~SinglyLIst()
    {
        

        while(Head != nullptr)
        {
             Node* tmp = Head;
             Head = Head->Next;
             delete tmp;

        }
    }


    private:

Node* Head;
Node* Tail;
};


int main()
{
    SinglyLIst<int> list(1);


    list.InsertToList(2);

    list.InsertToList(3);

    list.showList();

}