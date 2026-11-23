/* glue.c -- libkernl: the newlib system call layer */

#include <errno.h>
#include <sys/stat.h>
#include "sdk/libkernl.h"

/* set once sceTtyInit has opened the console */
static int D_001FD5D0;

/*
================
func_0010F0E0

Forgets that the console was opened.
================
*/
void func_0010F0E0(void)
{
    D_001FD5D0 = 0;
}

/*
================
write

Writes to stdout and stderr go to the host console; nothing else can be
written.
================
*/
int write(int fd, const void *buf, int nbytes)
{
    if (fd == 1 || fd == 2) {
        if (!D_001FD5D0) {
            if (!sceTtyInit()) {
                return -1;
            }
            D_001FD5D0 = 1;
        }
        return sceTtyWrite(buf, nbytes);
    }
    return -1;
}

/*
================
read

Reads from stdin come from the host console.
================
*/
int read(int fd, void *buf, int nbytes)
{
    if (fd == 0) {
        if (!D_001FD5D0) {
            if (!sceTtyInit()) {
                return -1;
            }
            D_001FD5D0 = 1;
        }
        return sceTtyRead(buf, nbytes);
    }
    return -1;
}

/*
================
open

There is no file system: every open fails with EIO.
================
*/
int open(const char *path, int flags, int mode)
{
    errno = EIO;
    return -1;
}

/*
================
close

Nothing can be opened, so nothing can be closed.
================
*/
int close(int fd)
{
    return -1;
}

/*
================
func_0010F350
================
*/
int func_0010F350(void)
{
    return -1;
}

/*
================
lseek
================
*/
int lseek(int fd, long offset, int whence)
{
    return -1;
}

/*
================
isatty

Every descriptor is the console.
================
*/
int isatty(int fd)
{
    return 1;
}

/*
================
fstat

Every descriptor is a character device (the console), unbuffered.
================
*/
int fstat(int fd, struct stat *st)
{
    st->st_mode = S_IFCHR;
    st->st_blksize = 0;
    return 0;
}

/*
================
getpid

There is one process.
================
*/
int getpid(void)
{
    return 1;
}

/*
================
kill

Only the program itself (process 1) can be signalled: it exits.
================
*/
int kill(int pid, int sig)
{
    if (pid == 1) {
        Exit(sig);
    }
    return 0;
}

/*
================
stat
================
*/
int stat(const char *path, struct stat *st)
{
    errno = EIO;
    return -1;
}

/*
================
unlink
================
*/
int unlink(const char *path)
{
    errno = EIO;
    return -1;
}
