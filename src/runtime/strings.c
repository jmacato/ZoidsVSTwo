/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common/string.c.
 */
#include "runtime_string.h"
#define NULL 0

static int K1 = 0x80808080;
static int K2 = 0xFEFEFEFF;

size_t strlen(const char *str)
{
    size_t len = -1;
    unsigned char *p = (unsigned char *)str - 1;

    do {
        len++;
    } while (*++p);

    return len;
}

char *strcpy(char *dst, const char *src)
{
    register unsigned char *destb, *fromb;
    register unsigned long w, t, align;
    register unsigned int k1;
    register unsigned int k2;

    fromb = (unsigned char *)src;
    destb = (unsigned char *)dst;

    if ((align = ((int)fromb & 3)) != ((int)destb & 3)) {
        goto bytecopy;
    }

    if (align) {
        if ((*destb = *fromb) == 0) {
            return dst;
        }

        for (align = 3 - align; align; align--) {
            if ((*(++destb) = *(++fromb)) == 0) {
                return dst;
            }
        }
        ++destb;
        ++fromb;
    }

    k1 = K1;
    k2 = K2;

    w = *((int *)(fromb));

    t = w + k2;

    t &= k1;
    if (t) {
        goto bytecopy;
    }
    --((int *)(destb));

    do {
        *(++((int *)(destb))) = w;
        w = *(++((int *)(fromb)));

        t = w + k2;
        t &= k1;
        if (t) {
            goto adjust;
        }
    } while (1);

adjust:
    ++((int *)(destb));

bytecopy:
    if ((*destb = *fromb) == 0) {
        return dst;
    }

    do {
        if ((*(++destb) = *(++fromb)) == 0) {
            return dst;
        }
    } while (1);

    return dst;
}

char *strncpy(char *dst, const char *src, size_t n)
{
    const unsigned char *p = (const unsigned char *)src - 1;
    unsigned char *q = (unsigned char *)dst - 1;

    n++;
    while (--n) {
        if (!(*++q = *++p)) {
            while (--n) {
                *++q = 0;
            }
            break;
        }
    }

    return dst;
}

char *strcat(char *dst, const char *src)
{
    const unsigned char *p = (unsigned char *)src - 1;
    unsigned char *q = (unsigned char *)dst - 1;

    while (*++q) {
    }

    q--;

    while (*++q = *++p) {
    }

    return dst;
}

int strcmp(const char *str1, const char *str2)
{
    register unsigned char *left = (unsigned char *)str1;
    register unsigned char *right = (unsigned char *)str2;
    unsigned long align, l1, r1, x;

    l1 = *left;
    r1 = *right;
    if (l1 - r1) {
        return l1 - r1;
    }

    if ((align = ((int)left & 3)) != ((int)right & 3)) {
        goto bytecopy;
    }

    if (align) {
        if (l1 == 0) {
            return 0;
        }
        for (align = 3 - align; align; align--) {
            l1 = *(++left);
            r1 = *(++right);
            if (l1 - r1) {
                return l1 - r1;
            }
            if (l1 == 0) {
                return 0;
            }
        }
        left++;
        right++;
    }

    l1 = *(int *)left;
    r1 = *(int *)right;
    x = l1 + K2;
    if (x & K1) {
        goto adjust;
    }

    while (l1 == r1) {
        l1 = *(++((int *)(left)));
        r1 = *(++((int *)(right)));
        x = l1 + K2;
        if (x & K1) {
            goto adjust;
        }
    }

    if (l1 > r1) {
        return 1;
    }
    return -1;

adjust:
    l1 = *left;
    r1 = *right;
    if (l1 - r1) {
        return l1 - r1;
    }

bytecopy:
    if (l1 == 0) {
        return 0;
    }

    do {
        l1 = *(++left);
        r1 = *(++right);
        if (l1 - r1) {
            return l1 - r1;
        }
        if (l1 == 0) {
            return 0;
        }
    } while (1);
}

char *strchr(const char *str, int c)
{
    const unsigned char *p = (unsigned char *)str - 1;
    unsigned long chr = (c & 0xFF);

    unsigned long ch;
    while (ch = *++p) {
        if (ch == chr) {
            return (char *)p;
        }
    }

    return chr ? NULL : (char *)p;
}

int strncmp(const char *left, const char *right, size_t count)
{
    const unsigned char *p = (const unsigned char *)left - 1;
    const unsigned char *q = (const unsigned char *)right - 1;
    unsigned long a;
    unsigned long b;
    count++;
    while (--count) {
        a = *++p;
        b = *++q;
        if (a != b)
            return a - b;
        if (a == 0)
            break;
    }
    return 0;
}
