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
    char *p = dst;

    while ((*p++ = *src++) != 0)
        ;
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
        l1 = *(int *)(left += 4);
        r1 = *(int *)(right += 4);
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
