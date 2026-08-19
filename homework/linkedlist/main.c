#include <stdio.h>
#include <stdlib.h>
#include "intlist.h"

int main(void)
{
    /* Integer variable to store the current loop number */
    int i;

    /* Integer variable to store the value of the integer that
     * is added to or removed from the linked list */
    int value;

    /* Constant int for the number of random int values added to the list*/
    const int NUM_ADDED = 20;

    /* Constant int for the number of random int values removed from the list*/
    const int NUM_REMOVED = 20;



    /*Loop which adds random numbers to the start of the linked list each iteration*/
    printf("ADDING NUMBERS TO THE LIST:\n");
    for (i = 0; i < NUM_ADDED; i++)
    {
        value = rand() % 100;
        if (intlist_add(value))
        {
            printf("Added:%d\n", value);
        }
        else
        {
            printf("ERROR: Failed to add number: %d\n", value);
        }
    }

    /*Loop which removes the head of the linked list each iteration*/
    printf("\nREMOVING NUMBERS FROM THE LIST:\n");
    for (i = 0; i < NUM_REMOVED; i++)
    {
        if (intlist_remove(&value))
        {
            printf("Removed: %d\n", value);
        }
        else
        {
            printf("ERROR: List is empty");
        }
        
    }

    return 0;
}