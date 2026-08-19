/**
 * @file intlist.h
 * @author Abigail Wong
 * @brief Header file for the integer Linked List
 * @version 0.1
 * @date 2026-08-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#pragma once

/*Functions for adding and removing integers from a linked list*/

/**
 * Allocates a new node, stores 'value' in it, and links it in
 * front of the current head so it becomes the new first element.
 *
 * @param value integer to add to the head of the list
 * @returns 1 if successful, 0 if memory allocation failed
 */
int intlist_add(int value);


/**
 * Removes the head node and writes its value into 'out'
 * @param out pointer to an int where the removed value will be stored. 
 *            Only written to if the function returns 1; left untouched
 *            if the list was empty.
 * @returns 1 if successful, 0 if the list was already empty
 */
int intlist_remove(int *out);