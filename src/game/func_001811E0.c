/* func_001811E0.c -- list: make empty */

#include "game/linkedlist.h"

/*
================
func_001811E0

Makes the list empty: the sentinel links to itself and the count in its
element slot is 0.
================
*/
void func_001811E0(List *list)
{
    list->head.prev = &list->head;
    list->head.value = 0;
    list->head.next = &list->head;
}
