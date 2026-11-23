/*
 * extract_elf.c -- pull the boot executable out of a PS2 disc image.
 *
 * Usage: extract_elf <image.bin|image.iso> <out.elf>
 *
 * Accepts a raw MODE2/2352 track (Redump .bin) or a plain 2048-byte-sector
 * ISO. Reads SYSTEM.CNF, follows its BOOT2 line and writes the executable it
 * names. Only the root directory is searched, which is where PS2 discs keep
 * both files.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ISO_SECTOR      2048
#define RAW_SECTOR      2352
#define RAW_DATA_OFFSET 24      /* sync(12) + header(4) + subheader(8) */

static FILE *g_image;
static long  g_sectorSize;
static long  g_dataOffset;

static int ReadSector(long lba, unsigned char *out)
{
    if (fseek(g_image, lba * g_sectorSize + g_dataOffset, SEEK_SET) != 0)
        return 0;
    return fread(out, 1, ISO_SECTOR, g_image) == ISO_SECTOR;
}

static unsigned long ReadLE32(const unsigned char *p)
{
    return p[0] | (p[1] << 8) | ((unsigned long)p[2] << 16) | ((unsigned long)p[3] << 24);
}

/* Find a root-directory entry by name, ignoring case and the ";1" suffix. */
static int FindRootFile(const char *name, unsigned long *lba, unsigned long *size)
{
    unsigned char pvd[ISO_SECTOR], dir[ISO_SECTOR];
    unsigned long rootLba, rootSize, offset;
    size_t nameLen = strlen(name);

    if (!ReadSector(16, pvd) || memcmp(pvd + 1, "CD001", 5) != 0)
        return 0;

    rootLba  = ReadLE32(pvd + 156 + 2);
    rootSize = ReadLE32(pvd + 156 + 10);

    for (offset = 0; offset < rootSize; offset += ISO_SECTOR) {
        unsigned int pos = 0;

        if (!ReadSector(rootLba + offset / ISO_SECTOR, dir))
            return 0;

        while (pos < ISO_SECTOR && dir[pos] != 0) {
            unsigned int recLen = dir[pos];
            unsigned int idLen  = dir[pos + 32];
            const char  *id     = (const char *)dir + pos + 33;
            size_t       i;

            if (idLen >= nameLen) {
                for (i = 0; i < nameLen; i++) {
                    if (toupper((unsigned char)id[i]) != toupper((unsigned char)name[i]))
                        break;
                }
                if (i == nameLen && (idLen == nameLen || id[nameLen] == ';')) {
                    *lba  = ReadLE32(dir + pos + 2);
                    *size = ReadLE32(dir + pos + 10);
                    return 1;
                }
            }
            pos += recLen;
        }
    }
    return 0;
}

static unsigned char *ReadFile(unsigned long lba, unsigned long size)
{
    unsigned char *buf = malloc(size + ISO_SECTOR);
    unsigned long  done;

    if (buf == NULL)
        return NULL;
    for (done = 0; done < size; done += ISO_SECTOR) {
        if (!ReadSector(lba + done / ISO_SECTOR, buf + done)) {
            free(buf);
            return NULL;
        }
    }
    return buf;
}

int main(int argc, char **argv)
{
    unsigned char  probe[16];
    unsigned long  lba, size;
    unsigned char *cnf, *exe;
    char           bootName[64];
    const char    *boot, *p;
    size_t         n = 0;
    FILE          *out;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <image.bin|image.iso> <out.elf>\n", argv[0]);
        return 2;
    }
    g_image = fopen(argv[1], "rb");
    if (g_image == NULL) {
        perror(argv[1]);
        return 1;
    }

    /* raw tracks start every sector with the 00 FF*10 00 sync pattern */
    if (fread(probe, 1, 12, g_image) == 12 && probe[0] == 0 && probe[1] == 0xFF && probe[11] == 0) {
        g_sectorSize = RAW_SECTOR;
        g_dataOffset = RAW_DATA_OFFSET;
    } else {
        g_sectorSize = ISO_SECTOR;
        g_dataOffset = 0;
    }

    if (!FindRootFile("SYSTEM.CNF", &lba, &size) || (cnf = ReadFile(lba, size)) == NULL) {
        fprintf(stderr, "SYSTEM.CNF not found\n");
        return 1;
    }
    cnf[size] = '\0';

    boot = strstr((char *)cnf, "cdrom0:");
    if (boot == NULL) {
        fprintf(stderr, "no BOOT2 line in SYSTEM.CNF\n");
        return 1;
    }
    for (p = boot + 7; *p == '\\' || *p == '/'; p++)
        ;
    while (*p != ';' && !isspace((unsigned char)*p) && *p != '\0' && n < sizeof(bootName) - 1)
        bootName[n++] = *p++;
    bootName[n] = '\0';

    if (!FindRootFile(bootName, &lba, &size) || (exe = ReadFile(lba, size)) == NULL) {
        fprintf(stderr, "%s not found\n", bootName);
        return 1;
    }

    out = fopen(argv[2], "wb");
    if (out == NULL || fwrite(exe, 1, size, out) != size) {
        perror(argv[2]);
        return 1;
    }
    fclose(out);
    printf("%s: %lu bytes -> %s\n", bootName, size, argv[2]);
    return 0;
}
