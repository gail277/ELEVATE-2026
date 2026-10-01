#pragma once

#include <stdint.h>

/** @brief Initialise the memory pool and free list */
void myalloc_init(void);

/** @brief Allocate a block of memory */
void *myalloc(uint32_t size);

/** @brief Free a previously allocated block */
void myfree(void *ptr);
