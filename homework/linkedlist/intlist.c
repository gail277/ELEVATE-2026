#include <stdlib.h>
#include "intlist.h"

typedef struct Node
{
    int data;           /*Stores value of the integer*/
    struct Node *next;  /*Stores the location of the integer*/
} Node;

/*Pointer to the address of the head*/
static Node *head = NULL;

void intlist_add(int value)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = head;
    head = new_node;
}

int intlist_remove(void)
{
    if (head == NULL)
    {
        return 0;
    }

    Node *temp = head;
    int value = temp->data;
    head = temp->next;
    free(temp);
    return value;

}