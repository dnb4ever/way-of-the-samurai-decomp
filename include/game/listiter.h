/* listiter.h -- iterators over the game's circular doubly linked lists */
#ifndef GAME_LISTITER_H
#define GAME_LISTITER_H

#include "common.h"

/*
 * guess: names in this header (ListNode, ListIter, ListIterVtbl and their
 * members) are descriptive, not recorded.
 *
 * Evidence for the shape: every list keeps a sentinel node at its own
 * offset 0. Its iterators are 8-byte objects { node, vptr } whose vtables
 * share one 8-slot layout; slot 2 follows node->next (offset 4), slot 3
 * follows node->prev (offset 0), slot 1 returns the address just past the
 * two links, and the "begin" constructor starts at the sentinel's next.
 * The game is C++: each list element type has its own iterator vtable and
 * its own copy of each method (one template instance per element type).
 * Here one C type stands for all of them.
 */

typedef struct ListNode ListNode;
typedef struct ListIter ListIter;
typedef struct ListIterVtbl ListIterVtbl;

struct ListNode {
    ListNode *prev;   /* 0x00 */
    ListNode *next;   /* 0x04 */
    s32       value;  /* 0x08 element storage; its type depends on the list */
};

/* a list: its sentinel node, the end of every walk. The sentinel's element
   slot (head.value) holds the element count: the list initialisers clear it
   with the links (func_00168A80, func_0016D650). */
typedef struct List {
    ListNode head;    /* 0x00 */
} List;

struct ListIter {
    ListNode           *node;  /* 0x00 current node */
    const ListIterVtbl *vtbl;  /* 0x04 */
};

/* the iterator vtable; vptr points at the two header words */
struct ListIterVtbl {
    s32 unk_00;                                     /* 0x00 header word, 0 */
    s32 unk_04;                                     /* 0x04 header word, 0 */
    ListIter *(*dtor)(ListIter *it, s16 flags);     /* 0x08 deleting destructor */
    void *(*get)(ListIter *it);                     /* 0x0C element address */
    ListIter *(*next)(ListIter *it);                /* 0x10 ++it */
    ListIter *(*prev)(ListIter *it);                /* 0x14 --it */
    ListIter *(*postNext)(ListIter *out, ListIter *it); /* 0x18 it++ */
    ListIter *(*postPrev)(ListIter *out, ListIter *it); /* 0x1C it-- */
    void *(*unk_20)(ListIter *it);                  /* 0x20 calls get */
    void *(*unk_24)(ListIter *it);                  /* 0x24 calls get */
};

/* iterator vtables, one per element type */
extern const ListIterVtbl D_00223670;
extern const ListIterVtbl D_00223980;
extern const ListIterVtbl D_002239E0;
extern const ListIterVtbl D_00223A10;
extern const ListIterVtbl D_00223A40;
extern const ListIterVtbl D_00223B40;
extern const ListIterVtbl D_00223B80;
extern const ListIterVtbl D_00223BB0;

/* D_00223670 */
void     *func_00134960(ListIter *it);
ListIter *func_00134970(ListIter *it, ListNode *node);
ListIter *func_00136170(ListIter *it);
ListIter *func_00136210(ListIter *it);

/* D_00223980 */
ListIter *func_001525A0(ListIter *it, ListNode *node);
ListIter *func_00153170(ListIter *it);

/* D_002239E0 */
ListIter *func_0015F3B0(ListIter *it, ListNode *node);
ListIter *func_0015FA80(ListIter *it);

/* D_00223A10 */
ListIter *func_0015F260(ListIter *it, ListNode *node);
void     *func_0015F790(ListIter *it);
ListIter *func_0015F7B0(ListIter *it);
void     *func_0015FA40(ListIter *it);
ListIter *func_0015FA50(ListIter *it);

/* D_00223A40 */
ListIter *func_0015E2C0(ListIter *it, ListNode *node);
void     *func_0015EF50(ListIter *it);
ListIter *func_0015F7E0(ListIter *it);
ListIter *func_0015FAA0(ListIter *it);

/* D_00223B40 */
ListIter *func_00165B00(ListIter *it, List *list);
ListIter *func_00166440(ListIter *it, ListNode *node);

/* D_00223B80 */
ListIter *func_00165170(ListIter *it, ListNode *node);
void     *func_00165190(ListIter *it);

/* D_00223BB0 */
void     *func_00165230(ListIter *it);
ListIter *func_00165240(ListIter *it, ListNode *node);
ListIter *func_00166A30(ListIter *it, ListNode *node);

#endif /* GAME_LISTITER_H */
