/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Runtime/__mem.c.
 */
#include "runtime_string.h"

__declspec(section ".init") void *memcpy(void *dst, const void *src, size_t n)
{
    const char *p;
    char *q;
    int rev = ((unsigned long)src < (unsigned long)dst);

    if (!rev) {
        for (p = (const char *)src - 1, q = (char *)dst - 1, n++; --n;)
            *++q = *++p;

    } else {
        for (p = (const char *)src + n, q = (char *)dst + n, n++; --n;)
            *--q = *--p;
    }
    return dst;
}

__declspec(section ".init") void __fill_mem(void *dst, int val, unsigned long n)
{
    unsigned char *p = (unsigned char *)dst;

    while (n--)
        *p++ = (unsigned char)val;
}

__declspec(section ".init") void *memset(void *dst, int val, size_t n)
{
    __fill_mem(dst, val, n);

    return dst;
}
