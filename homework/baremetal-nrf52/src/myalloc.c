
/**
 * Possible ways to  detect memory leaks:
 * 
 * Have a list that tracks allocated blocks of memory
 *  however this would require memory and the fixed size of the list 
 * varies so this is not optimal
 *  
 * Each block header contains a flag which determines if a block is free or used
 * At program termination any block still marked as allocated would 
 * indicate a possible leak.
 * 
 * */


#include <stdint.h>
#include <stddef.h>
#include "myalloc.h"


#define POOL_SIZE 1024

typedef struct block
{
    uint32_t length;
    struct block *next;
} block_t;


static uint32_t memory_pool[POOL_SIZE];
static block_t *free_list;


void myalloc_init(void)
{
    free_list = (block_t *)memory_pool;

    free_list->length = sizeof(memory_pool) - sizeof(block_t);
    free_list->next = NULL;
}



void *myalloc(uint32_t size)
{
    block_t *current = free_list;
    block_t *previous = NULL;
    block_t *prev_of_best = NULL;
    block_t *best = NULL;
    
    /* Round requested size up to a multiple of 4 */
    size = (size + 3) & ~3U;

    while (current != NULL)
    {
        /*If free block is large enough */
        if (current->length >= size)
        {
            /*If the free block is a tighter fit than the best block*/
            if (best == NULL || current->length < best->length)
            {
                prev_of_best = previous;
                best = current;
            }
        }

        previous = current;
        current = current->next;
    }

    if (best == NULL)
    {
        return NULL;
    }
    
    /*
        * Check whether the block is large enough
        * to split into an allocated block and a new
        * free block with usable space.
        */
    if (best->length >= size + sizeof(block_t) + sizeof(uint32_t))
    {
        block_t *new_block;

        new_block = (block_t *)((uint8_t *)(best + 1) + size);

        new_block->length =
            best->length - size - sizeof(block_t);

        new_block->next = best->next;

        if (prev_of_best == NULL)
        {
            free_list = new_block;
        }
        else
        {
            prev_of_best->next = new_block;
        }

        best->length = size;

        return (void *)(best + 1);
    }

    /*
        * Block is suitable but too small to split,
        * so use the entire block.
        */
    if (prev_of_best == NULL)
    {
        free_list = best->next;
    }
    else
    {
        prev_of_best->next = best->next;
    }

    return (void *)(best + 1);

}


void myfree(void *ptr)
{
    block_t *block;
    block_t *current;
    block_t *previous;

    if (ptr == NULL)
    {
        return;
    }

    /* Get the block header from the user pointer */
    block = ((block_t *)ptr) - 1;

    /* Find where this block belongs in the address-ordered free list */
    current = free_list;
    previous = NULL;

    while (current != NULL && current < block)
    {
        previous = current;
        current = current->next;
    }

    /* Insert the block into the free list */
    block->next = current;

    if (previous == NULL)
    {
        free_list = block;
    }
    else
    {
        previous->next = block;
    }

    /* Merge with the previous block if they are adjacent */
    if (previous != NULL &&
        (uint8_t *)previous + sizeof(block_t) + previous->length
            == (uint8_t *)block)
    {
        previous->length += sizeof(block_t) + block->length;
        previous->next = block->next;
        block = previous;
    }

    /* Merge with the next block if they are adjacent */
    if (current != NULL &&
        (uint8_t *)block + sizeof(block_t) + block->length
            == (uint8_t *)current)
    {
        block->length += sizeof(block_t) + current->length;
        block->next = current->next;
    }
}