/**
 * @file defs.h
 * @author Francisco Pombal
 * @date 22 April 2017
 * @brief File containing simple definitions.
 *
 * This header file contains:
 * - definition of a generic pointer type "Item"
 * - a portable "boolean" type definition
 * This code is heavily inspired on material obtained from the Algorithms and Data Structures
 * course at IST Lisboa.
 * @author Francisco Pombal
 * @bug No known bugs
 * @see https://www.tecnico.ulisboa.pt
 */


#ifndef DEFS_H
#define DEFS_H

/**
 * @brief Generic pointer type Item
 *
 * Saves the pain of always writing the *, making the code easier to read
 */
typedef void* Item;

/**
 * @brief Portable boolean type implementation
 *
 * This is useful for some systems that may not have the system stdbool.h header.
 */
typedef enum
{
    TRUE = 1,   /**< The boolean value "true" */
    FALSE = 0   /**< The boolean value "false" */
} Boolean;

#endif
