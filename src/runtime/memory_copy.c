/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/PowerPC_EABI_Support/src/Msl/MSL_C/MSL_Common/mem_funcs.c.
 */
#include "runtime_string.h"

void __copy_longs_aligned(void *dst, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;

    while (n--)
        *d++ = *s++;
}

void __copy_longs_rev_aligned(void *dst, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dst + n;
    const unsigned char *s = (const unsigned char *)src + n;

    while (n--)
        *--d = *--s;
}

void __copy_longs_unaligned(void *dst, const void *src, size_t n)
{
    __copy_longs_aligned(dst, src, n);
}

void __copy_longs_rev_unaligned(void *dst, const void *src, size_t n)
{
    __copy_longs_rev_aligned(dst, src, n);
}
