#ifndef DEFS_H
#include "./defs.h"
#endif

typedef struct SinglyLinkedList_ SinglyLinkedList;
typedef struct DoublyLinkedList_ DoublyLinkedList;

SinglyLinkedList* SinglyLinkedList_newNode(Item);
void SinglyLinkedList_freeNode(SinglyLinkedList*, void(*)(Item));
void SinglyLinkedList_freeList(SinglyLinkedList*, void (*)(Item));
void SinglyLinkedList_insertNext(SinglyLinkedList*, SinglyLinkedList*);
void SinglyLinkedList_insertAtEnd(SinglyLinkedList*, SinglyLinkedList*);
void SinglyLinkedList_deleteNextNode(SinglyLinkedList*, void(*)(Item));
SinglyLinkedList* SinglyLinkedList_getNextNode(SinglyLinkedList*); /* acessor*/
Item SinglyLinkedList_getItem(SinglyLinkedList*); /* acessor */
void SinglyLinkedList_setItem(SinglyLinkedList*, Item);
void SinglyLinkedList_printListItems(SinglyLinkedList*, void(*)(Item));
void* SinglyLinkedList_compareItems(SinglyLinkedList*, SinglyLinkedList*, void*(*)(Item, Item));
SinglyLinkedList* SinglyLinkedList_reverseList(SinglyLinkedList*);

// TODO: Implement and check if more are needed
DoublyLinkedList* DoublyLinkedList_newNode(Item);
void DoublyLinkedList_freeNode(DoublyLinkedList*, void(*)(Item));
void DoublyLinkedList_insertNext(DoublyLinkedList*, DoublyLinkedList*);
void DoublyLinkedList_deleteNextNode(DoublyLinkedList*, void(*)(Item));
DoublyLinkedList* DoublyLinkedList_getNextNode(DoublyLinkedList*); /* acessor*/
Item DoublyLinkedList_getItem(DoublyLinkedList*); /* acessor */
void DoublyLinkedList_printListItems(DoublyLinkedList*, void(*)(Item));
DoublyLinkedList* DoublyLinkedList_reverseList(DoublyLinkedList*);
