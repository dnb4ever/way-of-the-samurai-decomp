/* defMain.c -- default thread */

#include "game/ezmpeg.h"

/*
================
defMain

The sample's idle thread: keeps rotating the ready queue of priority 32.
================
*/
void defMain(void *arg)
{
    while (1) {
        RotateThreadReadyQueue(32);
    }
}
