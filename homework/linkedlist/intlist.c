#include <stdlib.h>
#include "intlist.h"

typedef struct Node
{
    int data;           /*Stores value of the integer*/
    struct Node *next;  /*Stores the location of the integer*/
} Node;

/* Pointer to the address of the head of the linked list, 
 * initialised to NULL as there is nothing in the list
 */
static Node *head = NULL;

/**
 * Function that adds an integer value to the front of the list
 * @param value integer that is added to the front of the list
 */
void 
intlist_add(int value)
{
    
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = head;
    head = new_node;
}

/**
 * Function that the integer value at the front of the list
 * @returns value of the integer that has been removed from the front of the list
 */
int 
intlist_remove(void)
{
    /* If the list is empty, 0 is returned*/
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