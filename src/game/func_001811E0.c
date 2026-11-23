/* func_001811E0.c -- list: make empty */

#include "game/linkedlist.h"

/*
================
func_001811E0

Makes the list empty: the sentinel links to itself.
================
*/
void func_001811E0(List *list)
{
    list->head.prev = &list->head;
    list->count = 0;
    list->head.next = &list->head;
}
