#include <stdio.h>
#include <stdlib.h>
#include "intlist.h"

void main(void)
{
    int i;
    int value;

    for (i = 0; i < 20; i++)
    {
        value = rand() % 100;
        intlist_add(value);
        printf("Added:%d\n.", value);
    }

    printf("\nRemoving items:\n");

    for (i = 0; i < 20; i++)
    {
        value = intlist_remove();
        printf("Removed: %d\n", value);
    }
}