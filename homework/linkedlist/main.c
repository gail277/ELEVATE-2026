#include <stdio.h>
#include <stdlib.h>
#include "intlist.h"

void main(void)
{
    int i;      /*This is an integer variable to store the current loop number*/
    int value;  /*This is an integer variable to store the value of the integer added to/removed from the linked list*/

    /*For Loop which adds random numbers to the head of the linked list each iteration*/
    printf("ADDING NUMBERS TO THE LIST:\n");
    for (i = 0; i < 20; i++)
    {
        value = rand() % 100;
        intlist_add(value);
        printf("Added:%d\n", value);
    }

    /*For Loop which removes the head of the linked list each iteration*/
    printf("\nREMOVING NUMBERS FROM THE LIST:\n");
    for (i = 0; i < 20; i++)
    {
        value = intlist_remove();
        printf("Removed: %d\n", value);
    }
}