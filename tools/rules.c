/*
 * rules.c -- the decomp-code-rules checker for this project.
 *
 * Usage: rules [-m fncheck.txt [-j report.json -T totals.txt]] [-q] file.c file.h ...
 *
 * Reports one line per finding:
 *     file:line: [match] R4 FunctionName: offset cast on a byte pointer
 *     file:line: advice R7 FunctionName: placeholder name func_00104370
 *
 * [match] findings stop a function from counting as matched (MATCH-PENDING).
 * Advice findings are open work that does not block a match. Code inside
 * #ifdef NON_MATCHING (the parking mechanism) is not checked for [match]
 * rules, since it is not claimed as matched.
 *
 * With -m, the output of fncheck is read too, and the two headline numbers
 * are printed: functions whose bytes match, and the subset that also passes
 * every [match] rule. With -j, those numbers are also written as an objdiff
 * style progress report; -T names the file holding the image's totals
 * ("total_code N" and "total_functions N" lines).
 *
 * The checks are deliberately narrow (see decomp-code-rules, "Making [match]
 * mechanical"): hardware volatile with a "// volatile:" note, struct
 * inheritance and SDK padding names pass.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 4096
#define MAX_FUNCS 65536

typedef struct {
    char *path;
    char *text;      /* original text */
    char *code;      /* comments and literals blanked, newlines kept */
    int   isHeader;
    int   numLines;
    char **lineCode; /* per-line views into code */
    char **lineText; /* per-line views into text */
    unsigned char *parked;  /* line is inside #ifdef NON_MATCHING */
    unsigned char *inMacro; /* line belongs to a #define */
} SourceFile;

typedef struct {
    char name[256];
    int  file, first, last;  /* lines, 1-based, inclusive */
    int  matchFindings;
    int  parked;
} Function;

static SourceFile g_files[MAX_FILES];
static int        g_numFiles;
static Function   g_funcs[MAX_FUNCS];
static int        g_numFuncs;
static int        g_quiet;
static int        g_matchCount, g_adviceCount;

/* ------------------------------------------------------------------ */

static char *ReadText(const char *path)
{
    FILE *f = fopen(path, "rb");
    long  n;
    char *buf;

    if (f == NULL)
        return NULL;
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc(n + 1);
    if (fread(buf, 1, n, f) != (size_t)n) {
        fclose(f);
        free(buf);
        return NULL;
    }
    buf[n] = '\0';
    fclose(f);
    return buf;
}

/* blank comments, string and character literals; keep newlines */
static char *StripCode(const char *s)
{
    size_t n = strlen(s), i;
    char  *c = malloc(n + 1);

    memcpy(c, s, n + 1);
    for (i = 0; i < n; i++) {
        if (c[i] == '/' && c[i + 1] == '*') {
            c[i] = c[i + 1] = ' ';
            for (i += 2; i < n && !(c[i] == '*' && c[i + 1] == '/'); i++) {
                if (c[i] != '\n')
                    c[i] = ' ';
            }
            if (i < n)
                c[i] = c[i + 1] = ' ', i++;
        } else if (c[i] == '/' && c[i + 1] == '/') {
            for (; i < n && c[i] != '\n'; i++)
                c[i] = ' ';
        } else if (c[i] == '"' || c[i] == '\'') {
            char q = c[i];
            for (i++; i < n && c[i] != q && c[i] != '\n'; i++) {
                if (c[i] == '\\' && i + 1 < n)
                    c[i++] = ' ';
                c[i] = ' ';
            }
        }
    }
    return c;
}

static char **SplitLines(char *buf, int *count)
{
    int    n = 1, i = 0;
    char  *p;
    char **lines;

    for (p = buf; *p; p++) {
        if (*p == '\n')
            n++;
    }
    lines = malloc((n + 1) * sizeof(char *));
    lines[i++] = buf;
    for (p = buf; *p; p++) {
        if (*p == '\n') {
            *p = '\0';
            lines[i++] = p + 1;
        }
    }
    *count = n;
    return lines;
}

static int LoadSource(const char *path)
{
    SourceFile *f = &g_files[g_numFiles];
    int         i, n2, depth = 0, parkDepth = -1;
    size_t      len = strlen(path);

    f->text = ReadText(path);
    if (f->text == NULL)
        return 0;
    f->path     = strdup(path);
    f->isHeader = len > 2 && path[len - 1] == 'h' && path[len - 2] == '.';
    f->code     = StripCode(f->text);
    f->lineCode = SplitLines(f->code, &f->numLines);
    f->lineText = SplitLines(f->text, &n2);
    f->parked   = calloc(f->numLines + 1, 1);
    f->inMacro  = calloc(f->numLines + 1, 1);

    /* preprocessor state: NON_MATCHING parking and #define continuation */
    for (i = 0; i < f->numLines; i++) {
        const char *p = f->lineCode[i];
        while (isspace((unsigned char)*p))
            p++;
        if (*p == '#') {
            const char *d = p + 1;
            while (isspace((unsigned char)*d))
                d++;
            if (strncmp(d, "if", 2) == 0) {
                if (parkDepth < 0 && strstr(d, "NON_MATCHING") && strncmp(d, "ifndef", 6) != 0)
                    parkDepth = depth;
                depth++;
            } else if (strncmp(d, "endif", 5) == 0) {
                depth--;
                if (depth == parkDepth)
                    parkDepth = -1;
            } else if (strncmp(d, "else", 4) == 0 || strncmp(d, "elif", 4) == 0) {
                if (depth - 1 == parkDepth)
                    parkDepth = -1;
            } else if (strncmp(d, "define", 6) == 0) {
                int j = i;
                f->inMacro[j] = 1;
                while (j < f->numLines && strlen(f->lineCode[j]) > 0 &&
                       f->lineCode[j][strlen(f->lineCode[j]) - 1] == '\\')
                    f->inMacro[++j] = 1;
            }
        }
        if (parkDepth >= 0)
            f->parked[i] = 1;
    }
    g_numFiles++;
    return 1;
}

/* ------------------------------------------------------------------ */
/* functions                                                           */
/* ------------------------------------------------------------------ */

static int IsIdentChar(int c) { return isalnum(c) || c == '_'; }

/* whole-word search used before the finding helpers are defined */
static int FindWordIn(const char *s, const char *word)
{
    size_t      n = strlen(word);
    const char *p = s;
    while ((p = strstr(p, word)) != NULL) {
        if ((p == s || !IsIdentChar((unsigned char)p[-1])) && !IsIdentChar((unsigned char)p[n]))
            return 1;
        p += n;
    }
    return 0;
}

/* find function definitions: a '{' at depth 0 preceded by "name(...)" */
static void FindFunctions(int fi)
{
    SourceFile *f = &g_files[fi];
    int         line, depth = 0, cur = -1;
    char        prev[2048] = "";

    for (line = 0; line < f->numLines; line++) {
        const char *p = f->lineCode[line];
        const char *t = p;

        while (isspace((unsigned char)*t))
            t++;
        if (*t == '#' || f->inMacro[line]) {
            prev[0] = '\0';
            continue;
        }
        for (; *p; p++) {
            if (*p == '{') {
                if (depth == 0) {
                    /* prev holds the text since the last ';' or '}' at depth 0 */
                    char *paren = strrchr(prev, ')');
                    char *open;
                    /* a definition: the declarator's ')' is the last thing before '{' */
                    const char *tail = prev + strlen(prev);
                    while (tail > prev && isspace((unsigned char)tail[-1]))
                        tail--;
                    if (paren && tail > prev && tail[-1] == ')' && !strchr(prev, '=') &&
                        !FindWordIn(prev, "typedef")) {
                        int level = 0;
                        open = paren;
                        while (open > prev) {
                            if (*open == ')')
                                level++;
                            else if (*open == '(' && --level == 0)
                                break;
                            open--;
                        }
                        while (open > prev && isspace((unsigned char)open[-1]))
                            open--;
                        {
                            char *end = open, *start = open;
                            while (start > prev && (isalnum((unsigned char)start[-1]) || start[-1] == '_' || start[-1] == ':' || start[-1] == '~'))
                                start--;
                            if (end > start && g_numFuncs < MAX_FUNCS) {
                                Function *fn = &g_funcs[g_numFuncs];
                                size_t    n = end - start;
                                if (n >= sizeof(fn->name))
                                    n = sizeof(fn->name) - 1;
                                memcpy(fn->name, start, n);
                                fn->name[n] = '\0';
                                fn->file = fi;
                                fn->first = line + 1;
                                fn->parked = f->parked[line];
                                cur = g_numFuncs++;
                                /* include the declarator lines above */
                                {
                                    /* walk back to the line holding the parameter list's '(' */
                                    int back = line, balance = 0, found = 0;
                                    while (back >= 0 && !found) {
                                        const char *c = f->lineCode[back];
                                        const char *e = c + strlen(c);
                                        if (back == line) {
                                            const char *brace = strchr(c, '{');
                                            if (brace)
                                                e = brace;
                                        }
                                        while (e > c) {
                                            e--;
                                            if (*e == ')')
                                                balance++;
                                            else if (*e == '(' && --balance == 0) {
                                                found = 1;
                                                break;
                                            }
                                        }
                                        if (!found)
                                            back--;
                                    }
                                    fn->first = (back < 0 ? 0 : back) + 1;
                                }
                            }
                        }
                    }
                }
                depth++;
                prev[0] = '\0';
            } else if (*p == '}') {
                depth--;
                if (depth == 0 && cur >= 0) {
                    g_funcs[cur].last = line + 1;
                    cur = -1;
                }
                prev[0] = '\0';
            } else if (depth == 0) {
                size_t n = strlen(prev);
                if (*p == ';') {
                    prev[0] = '\0';
                } else if (n + 1 < sizeof(prev)) {
                    prev[n] = *p;
                    prev[n + 1] = '\0';
                }
            }
        }
        if (depth == 0) {
            size_t n = strlen(prev);
            if (n + 1 < sizeof(prev)) {
                prev[n] = ' ';
                prev[n + 1] = '\0';
            }
        }
    }
}

static Function *FunctionAt(int fi, int line)
{
    int i;
    for (i = 0; i < g_numFuncs; i++) {
        if (g_funcs[i].file == fi && line >= g_funcs[i].first && line <= g_funcs[i].last)
            return &g_funcs[i];
    }
    return NULL;
}

/* ------------------------------------------------------------------ */
/* findings                                                            */
/* ------------------------------------------------------------------ */

static void Finding(int fi, int line, int isMatch, const char *rule, const char *fmt, const char *arg)
{
    Function *fn = FunctionAt(fi, line);
    char      msg[512];

    if (isMatch && g_files[fi].parked[line - 1])
        return;
    snprintf(msg, sizeof(msg), fmt, arg);
    if (isMatch) {
        g_matchCount++;
        if (fn)
            fn->matchFindings++;
    } else {
        g_adviceCount++;
        if (g_quiet)
            return;
    }
    printf("%s:%d: %s %s %s: %s\n", g_files[fi].path, line, isMatch ? "[match]" : "advice", rule,
           fn ? fn->name : "(file scope)", msg);
}

static int IsIdent(int c) { return isalnum(c) || c == '_'; }

/* next whole-word occurrence of word in s, or NULL */
static const char *FindWord(const char *s, const char *word)
{
    size_t      n = strlen(word);
    const char *p = s;
    while ((p = strstr(p, word)) != NULL) {
        if ((p == s || !IsIdent((unsigned char)p[-1])) && !IsIdent((unsigned char)p[n]))
            return p;
        p += n;
    }
    return NULL;
}

/* a placeholder-looking identifier at p (returns its length, or 0) */
static int PlaceholderAt(const char *s, const char *p)
{
    static const char *prefixes[] = { "func_", "D_", "var_", "arg", "unk_", "unk", "M2C_", "sp", "temp_",
                                      "FUN_", "DAT_", "sub_", "local_", "param_", "field_0x", "jtbl_", "lbl_", NULL };
    int i, len = 0;

    if (p > s && IsIdent((unsigned char)p[-1]))
        return 0;
    while (IsIdent((unsigned char)p[len]))
        len++;
    for (i = 0; prefixes[i]; i++) {
        size_t n = strlen(prefixes[i]);
        if ((size_t)len > n && strncmp(p, prefixes[i], n) == 0) {
            const char *rest = p + n;
            int         digits = 0, hex = 0, j;
            for (j = 0; j < len - (int)n; j++) {
                if (isdigit((unsigned char)rest[j]))
                    digits++;
                if (isxdigit((unsigned char)rest[j]))
                    hex++;
            }
            if (strcmp(prefixes[i], "M2C_") == 0)
                return len;
            /* the rest is a number (decimal or hex) */
            if (digits > 0 && hex == len - (int)n)
                return len;
            if (strcmp(prefixes[i], "var_") == 0 || strcmp(prefixes[i], "temp_") == 0)
                return len;
        }
    }
    /* address-like: a 6-8 digit hex run containing a digit, after an optional prefix */
    {
        int start = len;
        while (start > 0 && isxdigit((unsigned char)p[start - 1]))
            start--;
        if (len - start >= 6 && len - start <= 8) {
            int j, digit = 0;
            for (j = start; j < len; j++) {
                if (isdigit((unsigned char)p[j]))
                    digit = 1;
            }
            if (digit && (start == 0 || p[start - 1] == '_'))
                return len;
        }
    }
    return 0;
}

static void CheckLine(int fi, int line)
{
    SourceFile *f = &g_files[fi];
    const char *code = f->lineCode[line - 1];
    const char *text = f->lineText[line - 1];
    const char *p;
    Function   *fn = FunctionAt(fi, line);

    /* R18: assembly */
    if (FindWord(code, "asm") || FindWord(code, "__asm__") || FindWord(code, "__asm") || strstr(code, "INCLUDE_ASM"))
        Finding(fi, line, 1, "R18", "%sinline assembly", "");

    /* R2: extern declarations belong in headers */
    if (!f->isHeader && !f->inMacro[line - 1] && FindWord(code, "extern"))
        Finding(fi, line, 1, "R2", "%sextern declaration outside a header", "");

    /* R4: offset casts */
    if (strstr(code, "M2C_FIELD") && !f->inMacro[line - 1])
        Finding(fi, line, 1, "R4", "%sM2C_FIELD offset access", "");
    for (p = code; (p = strchr(p, '(')) != NULL; p++) {
        static const char *bytePtrs[] = { "char", "u8", "s8", "unsigned char", "signed char", "uintptr_t", "u32", "s32", "int", NULL };
        int k;
        for (k = 0; bytePtrs[k]; k++) {
            size_t      n = strlen(bytePtrs[k]);
            const char *q = p + 1;
            while (*q == ' ')
                q++;
            if (strncmp(q, bytePtrs[k], n) != 0 || IsIdent((unsigned char)q[n]))
                continue;
            q += n;
            while (*q == ' ')
                q++;
            if (k < 6) {
                if (*q != '*')
                    continue;
                q++;
                while (*q == ' ')
                    q++;
            }
            if (*q != ')')
                continue;
            /* skip the operand: an identifier, member chain or parenthesised expression */
            q++;
            while (*q == ' ')
                q++;
            if (*q == '(') {
                int level = 0;
                do {
                    if (*q == '(')
                        level++;
                    else if (*q == ')')
                        level--;
                    q++;
                } while (*q && level > 0);
            } else {
                while (IsIdent((unsigned char)*q) || *q == '.' || (*q == '-' && q[1] == '>') || (*q == '>' && q[-1] == '-') || *q == '[' || *q == ']')
                    q++;
            }
            while (*q == ' ')
                q++;
            if ((*q == '+' || *q == '-') && q[1] != '+' && q[1] != '-' && q[1] != '=') {
                q++;
                while (*q == ' ')
                    q++;
                if (isdigit((unsigned char)*q)) {
                    Finding(fi, line, 1, "R4", "%soffset arithmetic on a byte or integer cast", "");
                    goto r4done;
                }
            }
        }
    }
r4done:

    /* R17: volatile without evidence, +0 / *1, self-assignment, empty if, do-while(0) */
    if (FindWord(code, "volatile") && !strstr(text, "volatile:") && !f->inMacro[line - 1])
        Finding(fi, line, 1, "R17", "%svolatile without a // volatile: <evidence> note", "");
    for (p = code; (p = strchr(p, '+')) != NULL; p++) {
        const char *q = p + 1;
        if (p > code && (p[-1] == '+'))
            continue;
        while (*q == ' ')
            q++;
        if (*q == '0' && !isxdigit((unsigned char)q[1]) && q[1] != 'x' && q[1] != '.' && p[1] != '+' && p[1] != '=') {
            Finding(fi, line, 1, "R17", "%s'+ 0'", "");
            break;
        }
    }
    for (p = code; (p = strchr(p, '*')) != NULL; p++) {
        const char *q = p + 1;
        const char *b = p;
        while (b > code && b[-1] == ' ')
            b--;
        if (b == code || !(IsIdent((unsigned char)b[-1]) || b[-1] == ')' || b[-1] == ']'))
            continue;  /* a dereference or declarator, not a multiply */
        while (*q == ' ')
            q++;
        if (*q == '1' && !isalnum((unsigned char)q[1]) && q[1] != '.' && p[1] != '=') {
            Finding(fi, line, 1, "R17", "%s'* 1'", "");
            break;
        }
    }
    {
        const char *q = code;
        char        lhs[128];
        int         n = 0;
        while (isspace((unsigned char)*q))
            q++;
        while ((IsIdent((unsigned char)*q) || *q == '.' || *q == '-' || *q == '>') && n < 127) {
            if (*q == '-' && q[1] != '>')
                break;
            lhs[n++] = *q++;
        }
        lhs[n] = '\0';
        while (*q == ' ')
            q++;
        if (n > 0 && *q == '=' && q[1] != '=') {
            q++;
            while (*q == ' ')
                q++;
            if (strncmp(q, lhs, n) == 0 && !IsIdent((unsigned char)q[n])) {
                const char *r = q + n;
                while (*r == ' ')
                    r++;
                if (*r == ';')
                    Finding(fi, line, 1, "R17", "%sself-assignment", "");
            }
        }
    }
    if (fn && !f->inMacro[line - 1]) {
        const char *q = strstr(code, "while");
        if (q && strstr(q, "(0)") && strchr(code, '}'))
            Finding(fi, line, 1, "R17", "%sdo { } while (0) outside a macro", "");
        q = FindWord(code, "if");
        if (q) {
            const char *r = strchr(q, '(');
            if (r) {
                int level = 0;
                do {
                    if (*r == '(')
                        level++;
                    else if (*r == ')')
                        level--;
                    r++;
                } while (*r && level > 0);
                while (*r == ' ')
                    r++;
                if (*r == ';' || (r[0] == '{' && (r[1] == '}' || (r[1] == ' ' && r[2] == '}'))))
                    Finding(fi, line, 1, "R17", "%sempty if body", "");
            }
        }
    }
    if (!fn) {
        /* padding members */
        const char *q = code;
        while ((q = strstr(q, "pad")) != NULL) {
            if ((q == code || !IsIdent((unsigned char)q[-1])) &&
                (isdigit((unsigned char)q[3]) || q[3] == ';' || q[3] == '[' || q[3] == '_')) {
                Finding(fi, line, 1, "R17", "%spadding member", "");
                break;
            }
            q += 3;
        }
    }

    /* advice: placeholders (R7) */
    for (p = code; *p; p++) {
        int n;
        if (!IsIdent((unsigned char)*p) || (p > code && IsIdent((unsigned char)p[-1])))
            continue;
        n = PlaceholderAt(code, p);
        if (n > 0) {
            char word[128];
            if (n > 127)
                n = 127;
            memcpy(word, p, n);
            word[n] = '\0';
            Finding(fi, line, 0, "R7", "placeholder name %s", word);
            p += n - 1;
        }
    }
    /* advice: local extern (R25) is covered by R2; campaign notes in comments (R21) */
    {
        static const char *notes[] = { "claude", "autopilot", "floor", "FLOOR", "harness", "best ", "score",
                                       "Matching trick", "fakematch", "agent", NULL };
        int k;
        if (strcmp(code, text) != 0) {
            for (k = 0; notes[k]; k++) {
                if (strstr(text, notes[k])) {
                    Finding(fi, line, 0, "R21", "campaign note in a comment (%s)", notes[k]);
                    break;
                }
            }
        }
    }
    if (strstr(code, "#if 0") || strstr(code, "#if  0"))
        Finding(fi, line, 0, "R37", "%s#if 0 block", "");
}

/* R33: each function has a header comment directly above it */
static void CheckBanners(void)
{
    int i;
    for (i = 0; i < g_numFuncs; i++) {
        SourceFile *f = &g_files[g_funcs[i].file];
        int         l = g_funcs[i].first - 2;  /* 0-based line above */
        while (l >= 0 && f->lineText[l][strspn(f->lineText[l], " \t\r")] == '\0')
            l--;
        if (l < 0 || (!strstr(f->lineText[l], "*/") && strncmp(f->lineText[l] + strspn(f->lineText[l], " \t"), "//", 2) != 0))
            Finding(g_funcs[i].file, g_funcs[i].first, 0, "R33", "%sno header comment", "");
    }
}

/* R1: a type defined in more than one file */
static void CheckTypes(void)
{
    typedef struct { char name[128]; int file, line; } TypeDef;
    TypeDef *defs = malloc(sizeof(TypeDef) * 65536);
    int      n = 0, i, j;

    for (i = 0; i < g_numFiles; i++) {
        SourceFile *f = &g_files[i];
        int         line;
        for (line = 0; line < f->numLines && n < 65536; line++) {
            const char *c = f->lineCode[line];
            const char *p;
            char        name[128];
            int         k = 0;

            if (f->parked[line])
                continue;
            /* "struct Name {" or "typedef ... Name;" (outside braces is approximated by ';' at end) */
            if ((p = FindWord(c, "struct")) != NULL || (p = FindWord(c, "union")) != NULL) {
                const char *q = p + (p[0] == 's' ? 6 : 5);
                while (*q == ' ')
                    q++;
                while (IsIdent((unsigned char)*q) && k < 127)
                    name[k++] = *q++;
                name[k] = '\0';
                while (*q == ' ')
                    q++;
                if (k > 0 && *q == '{') {
                    strcpy(defs[n].name, name);
                    defs[n].file = i;
                    defs[n].line = line + 1;
                    n++;
                }
            }
            if (FindWord(c, "typedef") && strchr(c, ';') && !strchr(c, '(')) {
                const char *end = strrchr(c, ';');
                const char *q = end;
                /* skip spaces and trailing array dimensions "[N]", never digits of the name */
                for (;;) {
                    while (q > c && q[-1] == ' ')
                        q--;
                    if (q > c && q[-1] == ']') {
                        while (q > c && q[-1] != '[')
                            q--;
                        if (q > c)
                            q--;
                        continue;
                    }
                    break;
                }
                end = q;
                while (q > c && IsIdent((unsigned char)q[-1]))
                    q--;
                if (end > q && end - q < 127) {
                    memcpy(defs[n].name, q, end - q);
                    defs[n].name[end - q] = '\0';
                    defs[n].file = i;
                    defs[n].line = line + 1;
                    n++;
                }
            }
            /* "} Name;" closing a typedef'd struct */
            if (c[strspn(c, " ")] == '}') {
                const char *q = c + strspn(c, " ") + 1;
                while (*q == ' ')
                    q++;
                k = 0;
                while (IsIdent((unsigned char)*q) && k < 127)
                    name[k++] = *q++;
                name[k] = '\0';
                if (k > 0 && *q == ';') {
                    strcpy(defs[n].name, name);
                    defs[n].file = i;
                    defs[n].line = line + 1;
                    n++;
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i != j && defs[i].file != defs[j].file && strcmp(defs[i].name, defs[j].name) == 0) {
                char msg[300];
                snprintf(msg, sizeof(msg), "type %s also defined in %s", defs[i].name, g_files[defs[j].file].path);
                Finding(defs[i].file, defs[i].line, 1, "R1", "%s", msg);
                break;
            }
        }
    }
    free(defs);
}

/* ------------------------------------------------------------------ */

int main(int argc, char **argv)
{
    const char *matchesPath = NULL, *jsonPath = NULL, *totalsPath = NULL;
    int         i, line;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-m") == 0 && i + 1 < argc)
            matchesPath = argv[++i];
        else if (strcmp(argv[i], "-j") == 0 && i + 1 < argc)
            jsonPath = argv[++i];
        else if (strcmp(argv[i], "-T") == 0 && i + 1 < argc)
            totalsPath = argv[++i];
        else if (strcmp(argv[i], "-q") == 0)
            g_quiet = 1;
        else if (g_numFiles < MAX_FILES && !LoadSource(argv[i]))
            fprintf(stderr, "cannot read %s\n", argv[i]);
    }
    if (g_numFiles == 0) {
        fprintf(stderr, "usage: rules [-m fncheck.txt] [-q] file.c file.h ...\n");
        return 2;
    }

    for (i = 0; i < g_numFiles; i++)
        FindFunctions(i);
    for (i = 0; i < g_numFiles; i++) {
        for (line = 1; line <= g_files[i].numLines; line++)
            CheckLine(i, line);
    }
    CheckBanners();
    CheckTypes();

    printf("%d [match] finding(s), %d advice finding(s)\n", g_matchCount, g_adviceCount);

    if (matchesPath != NULL) {
        FILE *m = fopen(matchesPath, "r");
        char  buf[1024];
        int   bytes = 0, rules = 0;
        unsigned bytesSize = 0, rulesSize = 0;

        if (m == NULL) {
            perror(matchesPath);
            return 2;
        }
        while (fgets(buf, sizeof(buf), m)) {
            char     status[32], name[512];
            unsigned addr, size;
            int      k, pending = 0, found = 0;

            if (sscanf(buf, "%31s %x %u %511s", status, &addr, &size, name) != 4 || strcmp(status, "MATCH") != 0)
                continue;
            bytes++;
            bytesSize += size;
            for (k = 0; k < g_numFuncs; k++) {
                if (strcmp(g_funcs[k].name, name) == 0) {
                    found = 1;
                    if (g_funcs[k].matchFindings)
                        pending = 1;
                }
            }
            if (!found || pending) {
                printf("MATCH-PENDING %08X %s%s\n", addr, name, found ? "" : " (source not found)");
                continue;
            }
            rules++;
            rulesSize += size;
        }
        fclose(m);
        printf("matched under the rules: %d function(s), %u bytes\n", rules, rulesSize);
        printf("bytes matched:           %d function(s), %u bytes\n", bytes, bytesSize);

        if (jsonPath != NULL) {
            unsigned long totalCode = 0, totalFuncs = 0;
            FILE         *t = totalsPath ? fopen(totalsPath, "r") : NULL;
            FILE         *j;
            double        codePct, funcPct;

            while (t && fgets(buf, sizeof(buf), t)) {
                sscanf(buf, "total_code %lu", &totalCode);
                sscanf(buf, "total_functions %lu", &totalFuncs);
            }
            if (t)
                fclose(t);
            if (totalCode == 0 || totalFuncs == 0) {
                fprintf(stderr, "-j needs -T with total_code and total_functions\n");
                return 2;
            }
            codePct = 100.0 * rulesSize / totalCode;
            funcPct = 100.0 * rules / totalFuncs;
            j = fopen(jsonPath, "w");
            if (j == NULL) {
                perror(jsonPath);
                return 2;
            }
            /* "matched" counts only functions that also pass every [match] rule */
            fprintf(j, "{\"measures\": {\"fuzzy_match_percent\": %f, \"total_code\": %lu, \"matched_code\": %u, "
                       "\"matched_code_percent\": %f, \"total_data\": 0, \"matched_data\": 0, \"matched_data_percent\": 0.0, "
                       "\"total_functions\": %lu, \"matched_functions\": %d, \"matched_functions_percent\": %f, "
                       "\"complete_code\": %u, \"complete_code_percent\": %f, \"complete_data\": 0, "
                       "\"complete_data_percent\": 0.0, \"total_units\": 1, \"complete_units\": 0}}\n",
                    codePct, totalCode, rulesSize, codePct, totalFuncs, rules, funcPct, rulesSize, codePct);
            fclose(j);
        }
    }
    return 0;
}
