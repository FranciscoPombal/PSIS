/**
 * @file linked_list.h
 * @author Francisco Pombal
 * @date 22 April 2017
 * @brief File containing function prototypes for a singly/doubly-linked list implementation.
 *
 * This header contains the public interface for the linked list.
 * The actual implementation is private and hidden from the client.
 * The interface is as generic as possible in order to be very flexible.
 * This code is heavily inspired on material obtained from the Algorithms and Data Structures
 * course at IST Lisboa.
 * @author Francisco Pombal
 * @bug No known bugs
 * @see https://www.tecnico.ulisboa.pt
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/**
 * Include stdlib.h header because of malloc and related functionality
 */
#ifndef STDLIB_H
#include <stdlib.h>
#endif

/**
 * Include stdbool.h header to have a native boolean type for clarity
 */
#ifndef STDBOOL_H
#include <stdbool.h>
#endif

/**
 * Include the defs file
 * @see The documentation for the defs.h header
 */
#ifndef DEFS_H
#include "./defs.h"
#endif

/**
 * @brief Struct for the singly-linked list.
 *
 * The implementation is hidden from the client,
 */
typedef struct SinglyLinkedList_ SinglyLinkedList;

/**
 * @brief Struct for the doubly-linked list.
 *
 * The implementation is hidden from the client,
 */
typedef struct DoublyLinkedList_ DoublyLinkedList;


/** @brief Creates a new node for a singly-linked list (with a payload Item) and returns a pointer to it
 *
 * Heap memory is alocated for a @SinglyLinkedList object.
 * The next field is initialized to NULL.
 * The item field will point to the specified payload
 *
 * @param item The payload to be held by the newly created node.
 * @return new_node A pointer to the newly alocated node
 */
SinglyLinkedList* SinglyLinkedList_newNode(Item item);

/** @brief Frees the memory allocated to a node and corresponding payload.
 *
 * This function effectivelly deletes the node/link
 * First, the memory associated with the payload is freed by calling the appropriated function
 * specified by the function pointer Item_Free.
 * Finally, we simply free the node.
 *
 * @param node A pointer to the node to be freed.
 * @param Item_Free Function pointer to a function that frees the payload of the node.
 * @return void
 */
void SinglyLinkedList_freeNode(SinglyLinkedList* node, void(*Item_Free)(Item));

/** @brief Frees the memory allocated to an entire singly-linked list.
 *
 * This function traverses the whole list, starting from the first node;
 * For each node, the function SinglyLinkedList_freeNode is called.
 *
 * @param head A pointer to the first node of the singly linked list.
 * @param Item_Free Function pointer to a function that frees the payload of the nodes.
 * @return void
 */
void SinglyLinkedList_freeList(SinglyLinkedList* head, void (*Item_Free)(Item));

/** @brief Inserts a node between node2 and the node node2 is pointing to
 *
 * To explain the behaviour of this function, suppose we have pointers to three nodes, head, node1 and node2, where head is a pointer to the first node of the list.
 * After calling SinglyLinkedList_insertNext(node1, head), we will have:
 * head ---> node1
 * After calling SinglyLinkedList_insertNext(node2, head), we will have:
 * head ---> node2 ---> node1
 *
 * @param node1 A pointer to the node to be inserted
 * @param node2 A pointer to the node node1 will be inserted after
 * @return void
 */
void SinglyLinkedList_insertNext(SinglyLinkedList* node1, SinglyLinkedList* node2);

/** @brief Inserts a node at the end of a singly-linked list.
 *
 * To explain the behaviour of this function, suppose we have pointers to three nodes, head, node1 and node2, where head is a pointer to the first node of the singly-linked list.
 * After calling SinglyLinkedList_insertAtEnd(head, node1), we will have:
 * head ---> node1
 * After calling SinglyLinkedList_insertAtEnd(head, node2), we will have:
 * head ---> node1 ---> node2
 *
 * @param head A pointer to the first node of a singly-linked list
 * @param node A pointer to the node to be inserted at the end of a singly-linked list
 * @return void
 */
void SinglyLinkedList_insertAtEnd(SinglyLinkedList* head, SinglyLinkedList* node);

/** @brief Deletes the node after the one this function is called on
 *
 * To explain the behaviour of this function, suppose we have pointers to three nodes, head, node1 and node2, where head is a pointer to the first node of the singly-linked list.
 * Suppose these nodes are arranged in the following way:
 * head ---> node1 ---> node2 ---> NULL
 * After calling this function on node1, we will have:
 * head ---> node1 ---> NULL
 *
 * @param node A pointer to the node before the one we want to free.
 * @param Item_Free Function pointer to a function that frees the payload of the node.
 * @return void
 */
void SinglyLinkedList_deleteNextNode(SinglyLinkedList* node, void(*Item_Free)(Item));


/** @brief Deletes the specified node from the list
 *
 * To explain the behaviour of this function, suppose we have pointers to three nodes, head, node1 and node2, where head is a pointer to the first node of the singly-linked list.
 * Suppose these nodes are arranged in the following way:
 * head ---> node1 ---> node2 ---> NULL
 * After calling this function on node1, we will have:
 * head ---> node2 ---> NULL
 *
 * @param node A pointer to the node we want to free.
 * @param Item_Free Function pointer to a function that frees the payload of the node.
 * @return void
 */
void SinglyLinkedList_deleteNode(SinglyLinkedList* node, void(*Item_Free)(Item));

/** @brief Returns a pointer to the node after the one specified
 *
 * This function is an acessor for the singly-linked list
 *
 * @param node A pointer to the node before the one we want to get the pointer to.
 * @return next_node A pointer to the next node in the list
 */
SinglyLinkedList* SinglyLinkedList_getNextNode(SinglyLinkedList* node); /* acessor*/

/** @brief Returns a pointer to the payload of the specified node
 *
 * This function is an acessor for the singly-linked list
 *
 * @param node A pointer to the node we want to get a pointer to the payload of.
 * @return item A pointer to the payload we want
 */
Item SinglyLinkedList_getItem(SinglyLinkedList* node);

/** @brief Makes a node point to a specified payload
 *
 * This function does the complementary operation of SinglyLinkedList_getItem
 *
 * @param node A pointer to the node we want to set the payload of.
 * @param item A pointer to the payload
 * @return void
 */
void SinglyLinkedList_setItem(SinglyLinkedList* node, Item item);

/** @brief Prints the value of the payloads of every node on screen
 *
 * This function iterates through the whole list, calling the function pointed to by the specified function pointer
 * to print the values of the payload of each node to stdout
 *
 * @param node A pointer to the (first) node of the singly-linked list
 * @param Item_Print A pointer to function that prints the values of the payloads on screen
 * @return void
 */
void SinglyLinkedList_printListItems(SinglyLinkedList*, void(*Item_Print)(Item));

/** @brief Compares the payloads of two nodes of a singly-linked list
 *
 * This function calls an implementation-specific comparison function pointed to by the specified function pointer on the payloads of two nodes.
 * Then, this function returns a boolean value:
 * - True, if the contents of the payloads of the two nodes match
 * - False otherwise
 * The comparison function must return a boolean value
 *
 * @param node1 One of the nodes we wish to compare the payload of
 * @param node2 One of the nodes we wish to compare the payload of
 * @param Item_Compare Function pointer to the comparison function
 * @return result The result of the comparison
 */
bool SinglyLinkedList_compareItems(SinglyLinkedList*, SinglyLinkedList*, bool(*Item_Compare)(Item, Item));

/** @brief Reverses the order if the nodes in a singly-linked list
 *
 * To illustrate the behaviour of this function, suppose we have this list, with n nodes:
 * head ---> node1 ---> node2 ---> ... ---> noden ---> NULL\
 * After calling this function on the head of this list, the result will be:
 * noden ---> node2 ---> node2 ---> head ---> NULL
 * and a pointer to noden will be returned.
 *
 * @param node A pointer to the first node of the singly-linked list
 * @return res A pointer to the new head of the reversed sinlgy-linked list
 */
SinglyLinkedList* SinglyLinkedList_reverseList(SinglyLinkedList*);

bool SinglyLinkedList_nodeExistsWithItem(SinglyLinkedList* head, Item item, bool(*Item_Compare)(Item, Item));
SinglyLinkedList* SinglyLinkedList_findAndRetrieveNodeWithItem(SinglyLinkedList* head, Item item, bool(*Item_Compare)(Item, Item));
Item SinglyLinkedList_findAndRetrieveItem(SinglyLinkedList* head, Item item, bool(*Item_Compare)(Item, Item));

// TODO: Implement and check if more are needed
DoublyLinkedList* DoublyLinkedList_newNode(Item);
void DoublyLinkedList_freeNode(DoublyLinkedList*, void(*)(Item));
void DoublyLinkedList_insertNext(DoublyLinkedList*, DoublyLinkedList*);
void DoublyLinkedList_deleteNextNode(DoublyLinkedList*, void(*)(Item));
DoublyLinkedList* DoublyLinkedList_getNextNode(DoublyLinkedList*); /* acessor*/
Item DoublyLinkedList_getItem(DoublyLinkedList*); /* acessor */
void DoublyLinkedList_printListItems(DoublyLinkedList*, void(*)(Item));
DoublyLinkedList* DoublyLinkedList_reverseList(DoublyLinkedList*);

#endif
