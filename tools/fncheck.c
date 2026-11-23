/*
 * fncheck.c -- per-function byte check and image byte gate for SLUS_204.07.
 *
 * Usage:
 *   fncheck [-q] [-o patched.elf] [-H sha1] -t retail.elf -s symbols.txt [-s more.txt] obj.o...
 *
 * For every function symbol defined in the given objects:
 *   1. find its retail address: the symbol map, or the address in a
 *      func_XXXXXXXX / D_XXXXXXXX style name;
 *   2. relocate its bytes as if linked at that address, resolving external
 *      symbols through the map and $gp-relative ones through the retail $gp;
 *   3. references into the object's own data sections (literal pools, string
 *      tables, statics) cannot be placed by us, so the retail instruction is
 *      decoded to find the address it uses, and the object's bytes there must
 *      equal the retail bytes there. Every reference into one section must
 *      imply the same section base, or the check fails;
 *   4. compare with the retail bytes and print MATCH or DIFF.
 *
 * With -F, nothing is checked: each function's bytes are searched for in the
 * retail image instead, relocation fields masked, to place unnamed copies.
 *
 * With -o, every function of every object is written over a copy of the
 * retail image at its address; with -H, the SHA-1 of that copy must equal the
 * given hash. A function that does not match changes the hash, so the gate
 * fails unless every function matches. Exit status: 0 when every function
 * matches (and the hash holds), 1 otherwise, 2 on usage or I/O errors.
 */
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* ELF32 little-endian definitions                                     */
/* ------------------------------------------------------------------ */

#define SHT_SYMTAB   2
#define SHT_RELA     4
#define SHT_NOBITS   8
#define SHT_REL      9
#define STT_SECTION  3
#define STT_FUNC     2
#define SHN_UNDEF    0
#define SHN_ABS      0xFFF1
#define SHN_COMMON   0xFFF2
#define PT_LOAD      1

#define R_MIPS_NONE     0
#define R_MIPS_32       2
#define R_MIPS_26       4
#define R_MIPS_HI16     5
#define R_MIPS_LO16     6
#define R_MIPS_GPREL16  7
#define R_MIPS_LITERAL  8
#define R_MIPS_PC16     10
#define R_MIPS_GPREL32  12

typedef struct {
    uint32_t name, type, flags, addr, offset, size, link, info, addralign, entsize;
} Shdr;

typedef struct {
    const char *name;
    uint32_t    value, size;
    uint8_t     type, bind;
    uint16_t    shndx;
} Sym;

typedef struct {
    uint32_t offset, sym, type;
    int32_t  addend;
    int      hasAddend;
} Rel;

static uint32_t Rd32(const uint8_t *p) { return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24); }
static uint16_t Rd16(const uint8_t *p) { return (uint16_t)(p[0] | (p[1] << 8)); }
static void Wr32(uint8_t *p, uint32_t v) { p[0] = v; p[1] = v >> 8; p[2] = v >> 16; p[3] = v >> 24; }
static int32_t Sext16(uint32_t v) { return (int32_t)(int16_t)(v & 0xFFFF); }

static uint8_t *LoadFile(const char *path, long *size)
{
    FILE    *f = fopen(path, "rb");
    uint8_t *buf;
    long     n;

    if (f == NULL)
        return NULL;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(n ? n : 1);
    if (buf == NULL || fread(buf, 1, n, f) != (size_t)n) {
        fclose(f);
        free(buf);
        return NULL;
    }
    fclose(f);
    *size = n;
    return buf;
}

/* ------------------------------------------------------------------ */
/* retail image                                                        */
/* ------------------------------------------------------------------ */

typedef struct {
    uint32_t vaddr, filesz, fileoff;
} Segment;

static uint8_t *g_image;
static long     g_imageSize;
static Segment  g_segs[8];
static int      g_numSegs;
static uint32_t g_gp;

static int LoadRetail(const char *path)
{
    uint32_t phoff, shoff;
    int      i, phnum, shnum;

    g_image = LoadFile(path, &g_imageSize);
    if (g_image == NULL || memcmp(g_image, "\177ELF", 4) != 0)
        return 0;
    phoff = Rd32(g_image + 28);
    phnum = Rd16(g_image + 44);
    for (i = 0; i < phnum && g_numSegs < 8; i++) {
        const uint8_t *ph = g_image + phoff + i * 32;
        if (Rd32(ph) == PT_LOAD && Rd32(ph + 16) != 0) {
            g_segs[g_numSegs].fileoff = Rd32(ph + 4);
            g_segs[g_numSegs].vaddr   = Rd32(ph + 8);
            g_segs[g_numSegs].filesz  = Rd32(ph + 16);
            g_numSegs++;
        }
    }
    /* $gp comes from .reginfo (ri_gp_value is its last word) */
    shoff = Rd32(g_image + 32);
    shnum = Rd16(g_image + 48);
    for (i = 0; i < shnum; i++) {
        const uint8_t *sh = g_image + shoff + i * 40;
        if (Rd32(sh + 4) == 0x70000006)
            g_gp = Rd32(g_image + Rd32(sh + 16) + 20);
    }
    return g_numSegs > 0;
}

/* file offset of a retail address range, or -1 */
static long RetailOffset(uint32_t addr, uint32_t len)
{
    int i;
    for (i = 0; i < g_numSegs; i++) {
        if (addr >= g_segs[i].vaddr && addr + len <= g_segs[i].vaddr + g_segs[i].filesz)
            return g_segs[i].fileoff + (addr - g_segs[i].vaddr);
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/* symbol map                                                          */
/* ------------------------------------------------------------------ */

typedef struct {
    char    *name;
    uint32_t addr, size;
} MapEntry;

static MapEntry *g_map;
static int       g_mapCount, g_mapCap;

static unsigned HashName(const char *s)
{
    unsigned h = 2166136261u;
    while (*s)
        h = (h ^ (uint8_t)*s++) * 16777619u;
    return h;
}

static int *g_mapHash;
static int  g_mapHashSize;

static void MapAdd(const char *name, size_t len, uint32_t addr, uint32_t size)
{
    if (g_mapCount == g_mapCap) {
        g_mapCap = g_mapCap ? g_mapCap * 2 : 4096;
        g_map = realloc(g_map, g_mapCap * sizeof(*g_map));
    }
    g_map[g_mapCount].name = malloc(len + 1);
    memcpy(g_map[g_mapCount].name, name, len);
    g_map[g_mapCount].name[len] = '\0';
    g_map[g_mapCount].addr = addr;
    g_map[g_mapCount].size = size;
    g_mapCount++;
}

static void MapIndex(void)
{
    int i;
    g_mapHashSize = 1;
    while (g_mapHashSize < g_mapCount * 2)
        g_mapHashSize <<= 1;
    g_mapHash = malloc(g_mapHashSize * sizeof(int));
    memset(g_mapHash, -1, g_mapHashSize * sizeof(int));
    for (i = 0; i < g_mapCount; i++) {
        unsigned h = HashName(g_map[i].name) & (g_mapHashSize - 1);
        while (g_mapHash[h] >= 0)
            h = (h + 1) & (g_mapHashSize - 1);
        g_mapHash[h] = i;
    }
}

/* every entry with this name (library copies can share a name) */
static int MapFindAll(const char *name, const MapEntry **out, int max)
{
    unsigned h;
    int      n = 0;
    if (g_mapHashSize == 0)
        return 0;
    h = HashName(name) & (g_mapHashSize - 1);
    while (g_mapHash[h] >= 0) {
        if (strcmp(g_map[g_mapHash[h]].name, name) == 0 && n < max)
            out[n++] = &g_map[g_mapHash[h]];
        h = (h + 1) & (g_mapHashSize - 1);
    }
    return n;
}

/* "name = 0xADDR; // ... size:0xN" (splat symbol_addrs format) */
static int LoadMap(const char *path)
{
    FILE *f = fopen(path, "r");
    char  line[4096];

    if (f == NULL)
        return 0;
    while (fgets(line, sizeof(line), f)) {
        char    *p = line, *nameEnd, *sz;
        uint32_t addr, size = 0;

        while (isspace((unsigned char)*p))
            p++;
        if (*p == '\0' || *p == '/' || *p == '#')
            continue;
        nameEnd = p;
        while (*nameEnd && !isspace((unsigned char)*nameEnd) && *nameEnd != '=')
            nameEnd++;
        sz = strchr(nameEnd, '=');
        if (sz == NULL)
            continue;
        addr = strtoul(sz + 1, NULL, 0);
        sz = strstr(sz, "size:");
        if (sz != NULL)
            size = strtoul(sz + 5, NULL, 0);
        MapAdd(p, nameEnd - p, addr, size);
    }
    fclose(f);
    return 1;
}

/*
 * Address of a symbol: the map, then an address-derived placeholder name.
 * When several library copies share the name, `prefer` (the address the
 * retail code uses, or 0) picks among them.
 */
static int ResolveNameAt(const char *name, uint32_t prefer, uint32_t *addr)
{
    const MapEntry *all[16];
    int             n = MapFindAll(name, all, 16), i;
    const char     *u;

    if (n > 0) {
        *addr = all[0]->addr;
        for (i = 0; i < n; i++) {
            if (all[i]->addr == prefer)
                *addr = prefer;
        }
        return 1;
    }
    u = strrchr(name, '_');
    if (u != NULL && strlen(u + 1) >= 6 && strlen(u + 1) <= 8) {
        char *end;
        unsigned long v = strtoul(u + 1, &end, 16);
        if (*end == '\0' && v >= 0x100000) {
            *addr = (uint32_t)v;
            return 1;
        }
    }
    return 0;
}

static int ResolveName(const char *name, uint32_t *addr)
{
    return ResolveNameAt(name, 0, addr);
}

/* ------------------------------------------------------------------ */
/* object files                                                        */
/* ------------------------------------------------------------------ */

typedef struct {
    const char *path;
    uint8_t    *data;
    long        size;
    Shdr       *sh;
    int         shnum;
    const char *shstr;
    Sym        *syms;
    int         numSyms;
    uint32_t   *secBase;    /* inferred retail address of each section */
    uint8_t    *secBaseSet; /* 0 unset, 1 from a function, 2 from a referent */
} Object;

static int LoadObject(Object *o, const char *path)
{
    uint32_t shoff;
    int      i, shstrndx;

    memset(o, 0, sizeof(*o));
    o->path = path;
    o->data = LoadFile(path, &o->size);
    if (o->data == NULL || memcmp(o->data, "\177ELF", 4) != 0 || o->data[4] != 1 || o->data[5] != 1)
        return 0;
    shoff    = Rd32(o->data + 32);
    o->shnum = Rd16(o->data + 48);
    shstrndx = Rd16(o->data + 50);
    o->sh    = calloc(o->shnum, sizeof(Shdr));
    for (i = 0; i < o->shnum; i++) {
        const uint8_t *s = o->data + shoff + i * 40;
        o->sh[i].name = Rd32(s);       o->sh[i].type = Rd32(s + 4);
        o->sh[i].flags = Rd32(s + 8);  o->sh[i].addr = Rd32(s + 12);
        o->sh[i].offset = Rd32(s + 16); o->sh[i].size = Rd32(s + 20);
        o->sh[i].link = Rd32(s + 24);  o->sh[i].info = Rd32(s + 28);
        o->sh[i].addralign = Rd32(s + 32); o->sh[i].entsize = Rd32(s + 36);
    }
    o->shstr = (const char *)o->data + o->sh[shstrndx].offset;
    for (i = 0; i < o->shnum; i++) {
        if (o->sh[i].type == SHT_SYMTAB) {
            const char *str = (const char *)o->data + o->sh[o->sh[i].link].offset;
            int         n = o->sh[i].size / 16, j;
            o->syms = calloc(n, sizeof(Sym));
            o->numSyms = n;
            for (j = 0; j < n; j++) {
                const uint8_t *s = o->data + o->sh[i].offset + j * 16;
                o->syms[j].name  = str + Rd32(s);
                o->syms[j].value = Rd32(s + 4);
                o->syms[j].size  = Rd32(s + 8);
                o->syms[j].type  = s[12] & 0xF;
                o->syms[j].bind  = s[12] >> 4;
                o->syms[j].shndx = Rd16(s + 14);
            }
        }
    }
    o->secBase    = calloc(o->shnum, sizeof(uint32_t));
    o->secBaseSet = calloc(o->shnum, 1);
    return o->syms != NULL;
}

static const char *SecName(const Object *o, int i) { return o->shstr + o->sh[i].name; }
static int IsExecSection(const Object *o, int i) { return i > 0 && i < o->shnum && (o->sh[i].flags & 4); }

/* relocations that apply to section `target` */
static Rel *ObjectRelocs(const Object *o, int target, int *count)
{
    Rel *rels = NULL;
    int  n = 0, i;
    for (i = 0; i < o->shnum; i++) {
        int      rela = o->sh[i].type == SHT_RELA, ent = rela ? 12 : 8, j, m;
        if ((o->sh[i].type != SHT_REL && !rela) || (int)o->sh[i].info != target)
            continue;
        m = o->sh[i].size / ent;
        rels = realloc(rels, (n + m) * sizeof(Rel));
        for (j = 0; j < m; j++) {
            const uint8_t *r = o->data + o->sh[i].offset + j * ent;
            rels[n].offset    = Rd32(r);
            rels[n].sym       = Rd32(r + 4) >> 8;
            rels[n].type      = Rd32(r + 4) & 0xFF;
            rels[n].addend    = rela ? (int32_t)Rd32(r + 8) : 0;
            rels[n].hasAddend = rela;
            n++;
        }
    }
    *count = n;
    return rels;
}

/* ------------------------------------------------------------------ */
/* checking                                                            */
/* ------------------------------------------------------------------ */

static int g_quiet;
static int g_find;
static int g_failures, g_matches;
static uint32_t g_matchedBytes;

static void Report(const char *status, const Object *o, const char *name, uint32_t addr, uint32_t size, const char *why)
{
    if (g_quiet && strcmp(status, "MATCH") == 0)
        return;
    printf("%-8s %08X %5u %s", status, addr, size, name);
    if (why && *why)
        printf("  (%s; %s)", why, o->path);
    printf("\n");
}

/*
 * Address that the retail instruction(s) at a reloc site refer to.
 * HI16 needs its LO16 partner: `loWord` is the retail word at the partner.
 */
static int RetailReferent(uint32_t type, uint32_t word, uint32_t loWord, uint32_t pc, uint32_t *out)
{
    switch (type) {
    case R_MIPS_32:      *out = word; return 1;
    case R_MIPS_26:      *out = ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2); return 1;
    case R_MIPS_HI16:    *out = ((word & 0xFFFF) << 16) + Sext16(loWord); return 1;
    case R_MIPS_GPREL16:
    case R_MIPS_LITERAL: *out = g_gp + Sext16(word); return 1;
    default:             return 0;
    }
}

/* the address a retail call, word or $gp access names (0 when it can't tell) */
static uint32_t RetailWant(uint32_t type, uint32_t word, uint32_t pc)
{
    uint32_t want = 0;
    if (type == R_MIPS_26 || type == R_MIPS_32 || type == R_MIPS_GPREL16)
        RetailReferent(type, word, 0, pc, &want);
    return want;
}

/* Check one function. `work` holds a writable copy of its section. */
static int CheckFunction(Object *o, const Sym *fn, uint32_t addr, uint8_t *patch, char *why, size_t whyLen)
{
    int       sec = fn->shndx, nrel, i;
    Rel      *rels = ObjectRelocs(o, sec, &nrel);
    uint8_t  *buf;
    long      roff = RetailOffset(addr, fn->size);
    const uint8_t *retail;
    uint32_t  lo = fn->value, hi = fn->value + fn->size;
    int       diffs = 0, firstDiff = -1;

    why[0] = '\0';
    if (roff < 0) {
        snprintf(why, whyLen, "address outside the image");
        free(rels);
        return 0;
    }
    retail = g_image + roff;
    buf = malloc(fn->size);
    memcpy(buf, o->data + o->sh[sec].offset + fn->value, fn->size);

    /* pass 0 places every referent; pass 1 then fills the LO16 halves */
    for (i = 0; i < 2 * nrel; i++) {
        const Rel *r = &rels[i % nrel];
        const Sym *s;
        uint32_t   P, S, A, word, field;
        int        local, pos, pass = i >= nrel;

        if (r->offset < lo || r->offset + 4 > hi || r->type == R_MIPS_NONE)
            continue;
        if ((r->type == R_MIPS_LO16) != pass)
            continue;
        pos  = r->offset - lo;
        P    = addr + pos;
        s    = &o->syms[r->sym];
        word = Rd32(buf + pos);

        /* addend: REL keeps it in the field */
        switch (r->type) {
        case R_MIPS_32:      A = word; break;
        case R_MIPS_26:      A = (word & 0x03FFFFFF) << 2; break;
        case R_MIPS_HI16: {
            int j;
            A = (word & 0xFFFF) << 16;
            for (j = i + 1; j < nrel; j++) {
                if (rels[j].type == R_MIPS_LO16 && rels[j].sym == r->sym) {
                    A += Sext16(Rd32(o->data + o->sh[sec].offset + rels[j].offset));
                    break;
                }
            }
            if (j == nrel) {
                snprintf(why, whyLen, "HI16 without LO16");
                goto fail;
            }
            break;
        }
        case R_MIPS_LO16:
        case R_MIPS_GPREL16:
        case R_MIPS_LITERAL: A = Sext16(word); break;
        case R_MIPS_PC16:    A = Sext16(word) << 2; break;
        default:
            snprintf(why, whyLen, "unsupported relocation type %u", r->type);
            goto fail;
        }
        if (r->hasAddend)
            A = r->addend;

        local = s->shndx != SHN_UNDEF && s->shndx != SHN_COMMON && s->shndx != SHN_ABS;
        if (s->shndx == SHN_ABS) {
            S = s->value;
        } else if (local && IsExecSection(o, s->shndx) && o->secBaseSet[s->shndx] == 1) {
            S = o->secBase[s->shndx] + s->value;
        } else if (local && s->type != STT_SECTION && ResolveName(s->name, &S) && s->bind != 0) {
            /* a global defined here that the map also places */
        } else if (local) {
            /*
             * Data the object owns: locate it through the retail instruction,
             * then require identical contents and one base per section.
             */
            uint32_t ref, loWord = 0, base;
            long     doff;
            const Shdr *d = &o->sh[s->shndx];

            if (r->type == R_MIPS_LO16) {
                if (!o->secBaseSet[s->shndx]) {
                    snprintf(why, whyLen, "LO16 into %s without a placed HI16", SecName(o, s->shndx));
                    goto fail;
                }
                S = o->secBase[s->shndx] + s->value;
                goto apply;
            }
            if (r->type == R_MIPS_HI16) {
                int j;
                for (j = i + 1; j < nrel; j++) {
                    if (rels[j].type == R_MIPS_LO16 && rels[j].sym == r->sym) {
                        loWord = Rd32(retail + (rels[j].offset - lo));
                        break;
                    }
                }
                if (j == nrel) {
                    snprintf(why, whyLen, "HI16 without LO16");
                    goto fail;
                }
            }
            if (!RetailReferent(r->type, Rd32(retail + pos), loWord, P, &ref)) {
                snprintf(why, whyLen, "cannot place local referent (reloc %u)", r->type);
                goto fail;
            }
            /* ref = base + s->value + A */
            base = ref - s->value - A;
            if (o->secBaseSet[s->shndx] && o->secBase[s->shndx] != base) {
                snprintf(why, whyLen, "%s referents imply two section bases", SecName(o, s->shndx));
                goto fail;
            }
            o->secBase[s->shndx] = base;
            o->secBaseSet[s->shndx] = 2;
            if (d->type != SHT_NOBITS) {
                uint32_t off = s->value + A, n = d->size > off ? d->size - off : 0;
                if (n > 16)
                    n = 16;
                doff = RetailOffset(ref, n);
                if (n && (doff < 0 || memcmp(g_image + doff, o->data + d->offset + off, n) != 0)) {
                    snprintf(why, whyLen, "%s+0x%X differs from retail %08X", SecName(o, s->shndx), off, ref);
                    goto fail;
                }
            }
            S = base + s->value;
        } else if (!ResolveNameAt(s->name, RetailWant(r->type, Rd32(retail + pos), P), &S)) {
            snprintf(why, whyLen, "unresolved symbol %s", s->name);
            goto fail;
        }

    apply:
        switch (r->type) {
        case R_MIPS_32:
            Wr32(buf + pos, S + A);
            break;
        case R_MIPS_26:
            field = ((S + A) >> 2) & 0x03FFFFFF;
            Wr32(buf + pos, (word & 0xFC000000) | field);
            break;
        case R_MIPS_HI16:
            Wr32(buf + pos, (word & 0xFFFF0000) | (((S + A + 0x8000) >> 16) & 0xFFFF));
            break;
        case R_MIPS_LO16:
            Wr32(buf + pos, (word & 0xFFFF0000) | ((S + A) & 0xFFFF));
            break;
        case R_MIPS_GPREL16:
        case R_MIPS_LITERAL: {
            int32_t v = (int32_t)(S + A - g_gp);
            if (v < -32768 || v > 32767) {
                snprintf(why, whyLen, "%s out of $gp range", s->name);
                goto fail;
            }
            Wr32(buf + pos, (word & 0xFFFF0000) | (v & 0xFFFF));
            break;
        }
        case R_MIPS_PC16: {
            int32_t v = (int32_t)(S + A - (P + 4)) >> 2;
            Wr32(buf + pos, (word & 0xFFFF0000) | (v & 0xFFFF));
            break;
        }
        }
    }

    for (i = 0; i + 4 <= (int)fn->size; i += 4) {
        if (memcmp(buf + i, retail + i, 4) != 0) {
            if (firstDiff < 0)
                firstDiff = i;
            diffs++;
        }
    }
    if (patch != NULL)
        memcpy(patch + roff, buf, fn->size);
    free(buf);
    free(rels);
    if (diffs) {
        snprintf(why, whyLen, "%d word(s) differ, first at +0x%X", diffs, firstDiff);
        return 0;
    }
    return 1;

fail:
    if (patch != NULL)
        memcpy(patch + roff, buf, fn->size);
    free(buf);
    free(rels);
    return 0;
}

/* -F: list retail addresses whose bytes equal the function's, relocation fields masked */
static void FindFunction(const Object *o, const Sym *fn)
{
    int       nrel, i, seg, hits = 0;
    Rel      *rels = ObjectRelocs(o, fn->shndx, &nrel);
    uint32_t  words = fn->size / 4, w;
    uint32_t *mask = malloc(words * sizeof(uint32_t));
    const uint8_t *code = o->data + o->sh[fn->shndx].offset + fn->value;

    for (w = 0; w < words; w++)
        mask[w] = 0xFFFFFFFF;
    for (i = 0; i < nrel; i++) {
        uint32_t at = rels[i].offset - fn->value;
        if (rels[i].offset < fn->value || at + 4 > fn->size)
            continue;
        mask[at / 4] &= rels[i].type == R_MIPS_26 ? 0xFC000000 : rels[i].type == R_MIPS_32 ? 0 : 0xFFFF0000;
    }
    for (seg = 0; seg < g_numSegs; seg++) {
        const uint8_t *base = g_image + g_segs[seg].fileoff;
        uint32_t       off;
        for (off = 0; off + fn->size <= g_segs[seg].filesz; off += 4) {
            for (w = 0; w < words; w++) {
                if (((Rd32(base + off + 4 * w) ^ Rd32(code + 4 * w)) & mask[w]) != 0)
                    break;
            }
            if (w == words) {
                printf("FOUND    %08X %5u %s  (%s)\n", g_segs[seg].vaddr + off, fn->size, fn->name, o->path);
                if (++hits == 8)
                    goto done;
            }
        }
    }
    if (hits == 0)
        printf("NOTFOUND %08X %5u %s  (%s)\n", 0, fn->size, fn->name, o->path);
done:
    free(mask);
    free(rels);
}

static void CheckObject(Object *o, uint8_t *patch)
{
    int  i, pass;
    char why[256];

    /* pass 0: place executable sections from their function symbols */
    for (i = 0; i < o->numSyms; i++) {
        const Sym *s = &o->syms[i];
        uint32_t   addr;
        const MapEntry *all[2];
        if (s->type != STT_FUNC || !IsExecSection(o, s->shndx) || !ResolveName(s->name, &addr))
            continue;
        if (MapFindAll(s->name, all, 2) > 1)
            continue;   /* placed by its bytes later */
        if (o->secBaseSet[s->shndx] == 1 && o->secBase[s->shndx] != addr - s->value) {
            printf("ORDER    %08X %5u %s  (function order differs from the retail layout; %s)\n",
                   addr, s->size, s->name, o->path);
            g_failures++;
            continue;
        }
        o->secBase[s->shndx] = addr - s->value;
        o->secBaseSet[s->shndx] = 1;
    }

    for (pass = 0; pass < 1; pass++) {
        for (i = 0; i < o->numSyms; i++) {
            const Sym *s = &o->syms[i];
            uint32_t   addr;
        
            if (s->type != STT_FUNC || !IsExecSection(o, s->shndx) || s->size == 0)
                continue;
            if (g_find) {
                FindFunction(o, s);
                continue;
            }
            if (!ResolveName(s->name, &addr)) {
                Report("NOADDR", o, s->name, 0, s->size, "not in the symbol map");
                g_failures++;
                continue;
            }
            {
                /* library copies may share a name: use the copy whose bytes match */
                const MapEntry *all[16];
                int             n = MapFindAll(s->name, all, 16), k, sized = 0;
                for (k = 0; k < n; k++) {
                    if (all[k]->size != 0 && all[k]->size != s->size)
                        continue;
                    if (!sized++)
                        addr = all[k]->addr;
                    if (n > 1 && CheckFunction(o, s, all[k]->addr, NULL, why, sizeof(why))) {
                        addr = all[k]->addr;
                        break;
                    }
                }
                if (n > 0 && sized == 0) {
                    snprintf(why, sizeof(why), "retail size %u", all[0]->size);
                    Report("SIZE", o, s->name, all[0]->addr, s->size, why);
                    g_failures++;
                    continue;
                }
            }
            if (CheckFunction(o, s, addr, patch, why, sizeof(why))) {
                Report("MATCH", o, s->name, addr, s->size, "");
                g_matches++;
                g_matchedBytes += s->size;
            } else {
                Report("DIFF", o, s->name, addr, s->size, why);
                g_failures++;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* SHA-1                                                               */
/* ------------------------------------------------------------------ */

static void Sha1(const uint8_t *data, size_t len, char out[41])
{
    uint32_t h[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
    uint64_t bits = (uint64_t)len * 8;
    size_t   total = ((len + 8) / 64 + 1) * 64, off;
    uint8_t *msg = calloc(total, 1);
    int      i;

    memcpy(msg, data, len);
    msg[len] = 0x80;
    for (i = 0; i < 8; i++)
        msg[total - 1 - i] = (uint8_t)(bits >> (8 * i));

    for (off = 0; off < total; off += 64) {
        uint32_t w[80], a, b, c, d, e, f, k, t;
        for (i = 0; i < 16; i++)
            w[i] = (msg[off + 4 * i] << 24) | (msg[off + 4 * i + 1] << 16) | (msg[off + 4 * i + 2] << 8) | msg[off + 4 * i + 3];
        for (i = 16; i < 80; i++) {
            t = w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16];
            w[i] = (t << 1) | (t >> 31);
        }
        a = h[0]; b = h[1]; c = h[2]; d = h[3]; e = h[4];
        for (i = 0; i < 80; i++) {
            if (i < 20)      { f = (b & c) | (~b & d);           k = 0x5A827999; }
            else if (i < 40) { f = b ^ c ^ d;                    k = 0x6ED9EBA1; }
            else if (i < 60) { f = (b & c) | (b & d) | (c & d);  k = 0x8F1BBCDC; }
            else             { f = b ^ c ^ d;                    k = 0xCA62C1D6; }
            t = ((a << 5) | (a >> 27)) + f + e + k + w[i];
            e = d; d = c; c = (b << 30) | (b >> 2); b = a; a = t;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
    }
    free(msg);
    for (i = 0; i < 5; i++)
        sprintf(out + 8 * i, "%08x", h[i]);
}

/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
    const char *retail = NULL, *maps[8], *map = NULL;
    int         numMaps = 0;
    const char *outPath = NULL, *expect = NULL;
    uint8_t    *patch = NULL;
    int         i, first = 0;
    char        digest[41];

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-t") == 0 && i + 1 < argc)      retail = argv[++i];
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc && numMaps < 8) map = maps[numMaps++] = argv[++i];
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) outPath = argv[++i];
        else if (strcmp(argv[i], "-H") == 0 && i + 1 < argc) expect = argv[++i];
        else if (strcmp(argv[i], "-q") == 0)                 g_quiet = 1;
        else if (strcmp(argv[i], "-F") == 0)                 g_find = 1;
        else if (argv[i][0] == '-') {
            fprintf(stderr, "unknown option %s\n", argv[i]);
            return 2;
        } else {
            first = i;
            break;
        }
    }
    if (retail == NULL || map == NULL) {
        fprintf(stderr, "usage: fncheck [-q] [-F] [-o out.elf] [-H sha1] -t retail.elf -s symbols.txt obj.o...\n");
        return 2;
    }
    if (!LoadRetail(retail)) {
        fprintf(stderr, "cannot load %s\n", retail);
        return 2;
    }
    for (i = 0; i < numMaps; i++) {
        if (!LoadMap(maps[i])) {
            fprintf(stderr, "cannot load %s\n", maps[i]);
            return 2;
        }
    }
    MapIndex();
    if (expect != NULL) {
        Sha1(g_image, g_imageSize, digest);
        if (strcmp(digest, expect) != 0) {
            fprintf(stderr, "retail %s has SHA-1 %s, expected %s\n", retail, digest, expect);
            return 2;
        }
    }
    if (outPath != NULL || expect != NULL) {
        patch = malloc(g_imageSize);
        memcpy(patch, g_image, g_imageSize);
    }

    if (first == 0 && expect != NULL) {
        fprintf(stderr, "refusing to gate an empty object list\n");
        return 2;
    }
    for (i = first; first && i < argc; i++) {
        Object o;
        if (!LoadObject(&o, argv[i])) {
            fprintf(stderr, "cannot load object %s\n", argv[i]);
            return 2;
        }
        CheckObject(&o, patch);
    }

    if (outPath != NULL) {
        FILE *f = fopen(outPath, "wb");
        if (f == NULL || fwrite(patch, 1, g_imageSize, f) != (size_t)g_imageSize) {
            perror(outPath);
            return 2;
        }
        fclose(f);
    }
    printf("%d function(s) match (%u bytes), %d do not\n", g_matches, g_matchedBytes, g_failures);
    if (expect != NULL) {
        Sha1(patch, g_imageSize, digest);
        if (strcmp(digest, expect) != 0 || g_failures != 0) {
            printf("GATE FAILED: patched image SHA-1 %s, %d function(s) not matching\n", digest, g_failures);
            return 1;
        }
        printf("GATE OK: patched image SHA-1 %s (%d functions from source)\n", digest, g_matches);
    }
    return g_failures ? 1 : 0;
}
