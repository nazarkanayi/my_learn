#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

struct Node* intersectPoint(struct Node* head1, struct Node* head2) {

    struct Node* ptr1 = head1;
    struct Node* ptr2 = head2;

    if (ptr1 == NULL || ptr2 == NULL)
        return NULL;

    int i = 0;
    // Traverse through the lists until both pointers meet
    while (ptr1 != ptr2) {
      
        ptr1 = ptr1 ? ptr1->next : head2;
        ptr2 = ptr2 ? ptr2->next : head1;

        printf("loop %d \n",++i);
    }

    return ptr1;
}

int main() {
    
    // creation of first list: 10 -> 15 -> 30
    struct Node* head1 = createNode(10);
    head1->next = createNode(15);
    head1->next->next = createNode(30);

    // creation of second list: 3 -> 6 -> 9 -> 15 -> 30
    struct Node* head2 = createNode(3);
    head2->next = createNode(6);
    head2->next->next = createNode(9);
  
    head2->next->next->next = createNode(10);
    // 15 is the intersection point
   // head2->next->next->next = head1->next;

    struct Node* interPt = intersectPoint(head1, head2);

    if (interPt == NULL)
        printf("-1\n");
    else
        printf("%d\n", interPt->data);

    return 0;
}