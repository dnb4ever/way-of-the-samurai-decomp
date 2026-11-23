/* linkedlist.h -- list template instances of the game code above 0x168000 */
#ifndef GAME_LINKEDLIST_H
#define GAME_LINKEDLIST_H

#include "common.h"
#include "game/listiter.h"

/* The list, node and iterator types are listiter.h's. */

/* A node whose element needs 16-byte alignment: the element is at 0x10. */
typedef struct ListNodeQ {
    ListNode link;
    u128     unk_10;
} ListNodeQ;

/* iterator vtables of the instances above 0x168000 (the others are in
   listiter.h) */
extern const ListIterVtbl D_00223C40;
extern const ListIterVtbl D_00223C80;
extern const ListIterVtbl D_00223CB0;
extern const ListIterVtbl D_00223CE0;
extern const ListIterVtbl D_00223D10;
extern const ListIterVtbl D_00223D40;
extern const ListIterVtbl D_00223D70;
extern const ListIterVtbl D_00223E10;
extern const ListIterVtbl D_00223FE0;
extern const ListIterVtbl D_00224010;

/* functions, by address */
void func_00168A80(List *list);
List *func_0016D650(List *list);
ListIter *func_00174150(ListIter *it, ListNode *node);
ListIter *func_00174170(ListIter *it, List *list);
void func_00174190(List *list);
ListIter *func_00176080(ListIter *it);
ListIter *func_001760A0(ListIter *it);
ListIter *func_001760D0(ListIter *it);
ListIter *func_00179400(ListIter *it, ListNode *node);
void *func_00179420(ListIter *it);
ListIter *func_00179480(ListIter *it, List *list);
ListIter *func_0017A860(ListIter *it, ListNode *node);
void *func_0017A880(ListIter *it);
ListIter *func_0017A8E0(ListIter *it, List *list);
void *func_0017AF40(ListIter *it);
ListIter *func_0017AF50(ListIter *it, List *list);
ListIter *func_0017AF80(ListIter *it, ListNode *node);
ListIter *func_0017B0E0(ListIter *it, List *list);
ListIter *func_0017B110(ListIter *it, List *list);
ListIter *func_0017D2A0(ListIter *it, ListNode *node);
void *func_0017D2C0(ListIter *it);
ListIter *func_0017D2D0(ListIter *it, List *list);
ListIter *func_0017F500(ListIter *it, ListNode *node);
ListIter *func_0017F630(ListIter *it, List *list);
ListIter *func_001803C0(ListIter *it, ListNode *node);
ListIter *func_00180510(ListIter *it, List *list);
void func_001811E0(List *list);
ListIter *func_00181D40(ListIter *it);
ListIter *func_00181D80(ListIter *it);
ListIter *func_00181DD0(ListIter *it);
ListIter *func_00181E00(ListIter *it);
ListIter *func_001822A0(ListIter *it);
ListIter *func_001822C0(ListIter *it);
ListIter *func_001822E0(ListIter *it);
void *func_00182EC0(ListIter *it);
ListIter *func_00182F10(ListIter *it, List *list);
ListIter *func_00183030(ListIter *it, List *list);
ListIter *func_001858A0(ListIter *it);
void *func_00185A60(ListIter *it);
ListIter *func_00186040(ListIter *it);
ListIter *func_00186060(ListIter *it);
ListIter *func_0018E1F0(ListIter *it);
void *func_0018E210(ListIter *it);
ListIter *func_0018E220(ListIter *it, ListNode *node);
ListIter *func_0018E290(ListIter *it, List *list);
ListIter *func_0018E320(ListIter *it);
void *func_0018E340(ListIter *it);
ListIter *func_0018E350(ListIter *it, ListNode *node);
ListIter *func_0018E3C0(ListIter *it, List *list);
ListIter *func_00190950(ListIter *it);
ListIter *func_0019BB10(ListIter *it);
void *func_0019BB30(ListIter *it);
ListIter *func_0019BB40(ListIter *it, List *list);
ListIter *func_0019BC50(ListIter *it, ListNode *node);
void *func_001B7EE0(ListIter *it);
ListIter *func_001B9470(ListIter *it, ListNode *node);
ListIter *func_001B9490(ListIter *it, List *list);
void func_001B94B0(List *list);
ListIter *func_001B9B10(ListIter *it);
void *func_001BBA90(ListIter *it);
ListIter *func_001BD2E0(ListIter *it);
ListIter *func_001EF350(ListIter *it, ListNode *node);
void *func_001EF370(ListIter *it);
ListIter *func_001EF3D0(ListIter *it, List *list);
ListIter *func_001F02C0(ListIter *it);
ListIter *func_001F0880(ListIter *it, ListNode *node);
ListIter *func_001F09B0(ListIter *it, List *list);
void *func_001F0DA0(ListIter *it);
ListIter *func_001F2CC0(ListIter *it);

#endif /* GAME_LINKEDLIST_H */
