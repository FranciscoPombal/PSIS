#include <stdlib.h>
#include <stdio.h>
#ifndef LINKED_LIST_H
#include "linked_list.h"
#endif

struct SinglyLinkedList_ {
    Item item;
    SinglyLinkedList* next;
};

struct DoublyLinkedList_ {
    Item item;
    DoublyLinkedList* next;
    DoublyLinkedList* prev;
};

SinglyLinkedList* SinglyLinkedList_newNode(Item item)
{
    SinglyLinkedList* new_node = (SinglyLinkedList*)malloc(sizeof(SinglyLinkedList));
    new_node -> item = item;
    new_node -> next = NULL;

    return new_node;
}

void SinglyLinkedList_freeNode(SinglyLinkedList* node, void (*Item_Free)(Item))
{
    (*Item_Free)(node -> item);
    free(node);
}

void SinglyLinkedList_freeList(SinglyLinkedList* head, void (*Item_Free)(Item))
{
    SinglyLinkedList* aux = NULL;
    SinglyLinkedList* aux2 = NULL;

        aux = head -> next;
        SinglyLinkedList_freeNode(head, Item_Free);

        while(aux != NULL){
            aux2 = aux -> next;
            SinglyLinkedList_freeNode(aux, Item_Free);
            aux = aux2;
        }

    return;
}

void SinglyLinkedList_insertNext(SinglyLinkedList* node1, SinglyLinkedList* node2)
{
    node1 -> next = node2 -> next;
    node2 -> next = node1;

    return;
}

void SinglyLinkedList_deleteNextNode(SinglyLinkedList* node, void (*Item_Free)(Item))
{
    SinglyLinkedList* aux;

        aux = node -> next -> next;
        SinglyLinkedList_freeNode(node -> next, Item_Free);
        node -> next = aux;

    return;
}

SinglyLinkedList* SinglyLinkedList_getNextNode(SinglyLinkedList* node)
{
    return node -> next;
}

Item SinglyLinkedList_getItem(SinglyLinkedList* node)
{
    return node -> item;
}

void SinglyLinkedList_setItem(SinglyLinkedList* node, Item it)
{
    node -> item = it;
}

void SinglyLinkedList_printListItems(SinglyLinkedList* head, void(*Item_Print)(Item))
{
    SinglyLinkedList* aux = NULL;

    for (aux = head; aux != NULL; aux = aux -> next){
        (*Item_Print)(aux -> item);
    }

    return;
}

void* SinglyLinkedList_compareItems(SinglyLinkedList* node1, SinglyLinkedList* node2, void*(*Item_Compare)(Item, Item)){
    void* result = (*Item_Compare)(node1 -> item, node2 -> item);

    return result;
}

SinglyLinkedList* SinglyLinkedList_reverseList(SinglyLinkedList* pL)
{
    SinglyLinkedList* Next = pL;
    SinglyLinkedList* Cur = pL;
    SinglyLinkedList* Res = NULL;

        while (Cur != NULL) {
            Next = Cur -> next;
            Cur -> next = Res;
            Res = Cur;
            Cur = Next;
        }

    return Res;
}
