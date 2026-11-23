/* func_00174190.c -- list: make empty */

#include "game/linkedlist.h"

/*
================
func_00174190

Makes the list empty: the sentinel links to itself.
================
*/
void func_00174190(List *list)
{
    list->head.prev = &list->head;
    list->count = 0;
    list->head.next = &list->head;
}
