/* func_0016D650.c -- list: construct empty */

#include "game/linkedlist.h"

/*
================
func_0016D650

Constructs an empty list: the sentinel links to itself. Returns list.
================
*/
List *func_0016D650(List *list)
{
    list->head.next = &list->head;
    list->head.prev = &list->head;
    list->count = 0;
    return list;
}
