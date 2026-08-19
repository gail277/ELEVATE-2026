#include <stdlib.h>
#include "intlist.h"

/**
 * A single node in the linked list.
 * Each node holds one integer value and a pointer to the next
 * node in the list (or NULL if it's the last node).
 */
typedef struct Node_t
{
    int data;               /*Stores value of the integer*/
    struct Node_t *next;    /*Stores the location of the integer*/
} Node_t;

/**
 * Pointer to the head (first node) of the linked list.
 * Initialised to NULL since the list starts out empty.
 * Declared static so it is private to this file.
 * External code can only interact with the list through the functions below.
 */
static Node_t *head = NULL;

/**
 * Allocates a new node, stores 'value' in it, and links it in
 * front of the current head so it becomes the new first element.
 *
 * @param value integer to add to the head of the list
 * @returns 1 if successful, 0 if memory allocation failed
 */
int 
intlist_add(int value)
{
    /*Allocates enough space for a new node*/
    Node_t *new_node = malloc(sizeof(Node_t));

    /*If memory allocation failed (is null) then return 0*/
    if (new_node == NULL)
    {
        return 0;
    }

    /*Set the new node's data to be the passed in value*/
    new_node->data = value;

    /*Make the next node for new node the current head node*/
    new_node->next = head;

    /*Set the head node to the new node*/
    head = new_node;

    /*return 1 for successful node insertion*/
    return 1;
}

/**
 * Removes the head node and writes its value into 'out'
 * @param out pointer to an int where the removed value will be stored. 
 *            Only written to if the function returns 1; left untouched
 *            if the list was empty.
 * @returns 1 if successful, 0 if the list was already empty
 */
int 
intlist_remove(int *out)
{
    /* If the list is empty, 0 is returned*/
    if (head == NULL)
    {
        return 0;
    }

    Node_t *buffer = head;      /*Store head node in a buffer node*/  
    int value = buffer->data;   /*Store integer value in buffer into 'value'*/
    head = buffer->next;        /*Set head node to the next node in the linked list*/
    free(buffer);               /*Release the memory of the removed node*/
    *out = value;               /*Write the removed node back to the caller*/

    return 1;   /*return 1 for successful node removal*/
}
