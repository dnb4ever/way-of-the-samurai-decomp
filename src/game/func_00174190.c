/* func_00174190.c -- list: make empty */

#include "game/linkedlist.h"

/*
================
func_00174190

Makes the list empty: the sentinel links to itself and the count in its
element slot is 0.
================
*/
void func_00174190(List *list)
{
    list->head.prev = &list->head;
    list->head.value = 0;
    list->head.next = &list->head;
}
