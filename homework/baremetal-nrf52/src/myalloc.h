#pragma once

#include <stdint.h>

/* Initialise the memory pool and free list */
void myalloc_init(void);

/* Allocate a block of memory */
void *myalloc(uint32_t size);

/* Free a previously allocated block */
void myfree(void *ptr);
