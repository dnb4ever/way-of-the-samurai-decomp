/* func_001B94B0.c -- list: make empty */

#include "game/linkedlist.h"

/*
================
func_001B94B0

Makes the list empty: the sentinel links to itself.
================
*/
void func_001B94B0(List *list)
{
    list->head.prev = &list->head;
    list->count = 0;
    list->head.next = &list->head;
}
