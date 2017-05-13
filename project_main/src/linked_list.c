#ifndef LINKED_LIST_H
#include "../include/linked_list.h"
#endif

struct SinglyLinkedList_ {
    Item item;                  /**< A pointer to the payload of the node */
    SinglyLinkedList* next;     /**< A pointer to the next node on the list */
};

struct DoublyLinkedList_ {
    Item item;                  /**< A pointer to the payload of the node */
    DoublyLinkedList* next;     /**< A pointer to the next node on the list */
    DoublyLinkedList* prev;     /**< A pointer to the previous node on the list */
};

SinglyLinkedList* SinglyLinkedList_newNode(Item item)
{
    SinglyLinkedList* new_node = (SinglyLinkedList*)malloc(sizeof(SinglyLinkedList));
    new_node->item = item;
    new_node->next = NULL;

    return new_node;
}

void SinglyLinkedList_freeNode(SinglyLinkedList* node, void (*Item_Free)(Item))
{
    (*Item_Free)(node->item);
    free(node);
}

void SinglyLinkedList_freeList(SinglyLinkedList* head, void (*Item_Free)(Item))
{
    SinglyLinkedList* aux = NULL;
    SinglyLinkedList* aux2 = NULL;

        aux = head->next;
        SinglyLinkedList_freeNode(head, Item_Free);

        while(aux != NULL){
            aux2 = aux->next;
            SinglyLinkedList_freeNode(aux, Item_Free);
            aux = aux2;
        }

    return;
}

void SinglyLinkedList_insertNext(SinglyLinkedList* node1, SinglyLinkedList* node2)
{
    node1->next = node2->next;
    node2->next = node1;

    return;
}

void SinglyLinkedList_insertAtEnd(SinglyLinkedList* head, SinglyLinkedList* node)
{
    SinglyLinkedList* aux;

        for(aux = head; aux->next != NULL; aux = aux->next){}

        aux->next = node;

    return;
}

void SinglyLinkedList_deleteNextNode(SinglyLinkedList* node, void (*Item_Free)(Item))
{
    SinglyLinkedList* aux;

        aux = node->next->next;
        SinglyLinkedList_freeNode(node->next, Item_Free);
        node->next = aux;

    return;
}

void SinglyLinkedList_deleteNode(SinglyLinkedList* node, void(*Item_Free)(Item))
{
    SinglyLinkedList* aux = NULL;
    Item itemToDelete = NULL;

        aux = node->next;
        itemToDelete = node->item;

        if(aux != NULL){
            node->item = node->next->item;
            node->next = node->next->next;
            if(Item_Free != NULL){
                (*Item_Free)(itemToDelete);
            }else{
                free(itemToDelete);
                aux->item = NULL;
            }
            free(aux);
        }else{
            if(Item_Free != NULL){
                (*Item_Free)(itemToDelete);
                node->item = NULL;
            }else{
                free(itemToDelete);
                node->item = NULL;
            }
        }

    return;
}

SinglyLinkedList* SinglyLinkedList_getNextNode(SinglyLinkedList* node)
{
    SinglyLinkedList* next_node = NULL;

        next_node = node->next;

    return next_node;
}

Item SinglyLinkedList_getItem(SinglyLinkedList* node)
{
    Item item = NULL;

        item = node->item;

    return item;
}

void SinglyLinkedList_setItem(SinglyLinkedList* node, Item item)
{
    node->item = item;

    return;
}

void SinglyLinkedList_printListItems(SinglyLinkedList* head, void(*Item_Print)(Item))
{
    SinglyLinkedList* aux = NULL;

    for (aux = head; aux != NULL; aux = aux->next){
        (*Item_Print)(aux->item);
    }

    return;
}

bool SinglyLinkedList_compareItems(SinglyLinkedList* node1, SinglyLinkedList* node2, bool(*Item_Compare)(Item, Item))
{
    bool result = false;

        result = (*Item_Compare)(node1->item, node2->item);

    return result;
}

bool SinglyLinkedList_nodeExistsWithItem(SinglyLinkedList* head, Item item, bool(*Item_Compare)(Item, Item))
{
    SinglyLinkedList* aux = NULL;

        for(aux = head; aux != NULL; aux = aux->next){
            if(true == (*Item_Compare)(item, aux->item)){
                return true;
            }
        }

    return false;
}

SinglyLinkedList* SinglyLinkedList_findAndRetrieveNodeWithItem(SinglyLinkedList* head, Item item, bool(*Item_Compare)(Item, Item))
{
    SinglyLinkedList* aux = NULL;

        for(aux = head; aux != NULL; aux = aux->next){
            if(true == (*Item_Compare)(item, aux->item)){
                return aux;
            }
        }

    return NULL;
}

Item SinglyLinkedList_findAndRetrieveItem(SinglyLinkedList* head, Item item, bool(*Item_Compare)(Item, Item))
{
    SinglyLinkedList* aux = NULL;

        for(aux = head; aux != NULL; aux = aux->next){
            if(true == (*Item_Compare)(item, aux->item)){
                return aux->item;
            }
        }

    return NULL;
}

SinglyLinkedList* SinglyLinkedList_reverseList(SinglyLinkedList* head)
{
    SinglyLinkedList* next = head;
    SinglyLinkedList* cur = head;
    SinglyLinkedList* res = NULL;

        while(cur != NULL){
            next = cur->next;
            cur->next = res;
            res = cur;
            cur = next;
        }

    return res;
}
