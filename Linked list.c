#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node* next;
} Node;

void push(Node** head_ref, int new_data)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = new_data;
    new_node->next = *head_ref;
    *head_ref = new_node;
}

void append(Node** head_ref, int new_data)
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = new_data;
    new_node->next = NULL;

    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    Node* last = *head_ref;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = new_node;
}

void printList(Node* node)
{
    while (node != NULL) {
        printf("%d ->", node->data);
        node = node->next;
    }
    printf("NULL\n");
}

void deleteNode(Node** head_ref, int key)
{
    Node* temp = *head_ref;
    Node* prev = NULL;

    if (temp != NULL && temp->data == key) {
    *head_ref = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
}

void freeList(Node** head_ref)
{
    Node* current = *head_ref;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

int main()
{
    Node* head = NULL;

    append(&head, 10);
    append(&head, 20);
    push(&head, 5);
    printList(head);

    deleteNode(&head, 10);
    printList(head);
    freeList(&head);
    return 0;
}
