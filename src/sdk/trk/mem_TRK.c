#include "types.h"
#include "metrotrk.h"

__declspec(section ".init") void *TRK_memcpy(void *dst, const void *src, u32 n)
{
    const u8 *s = (const u8 *)src - 1;
    u8 *d = (u8 *)dst - 1;
    n++;
    while (--n != 0)
        *++d = *++s;
    return dst;
}
