#include "types.h"

u8 *cpyCr(u8 *source, u8 *destination)
{
    u8 value;
    s32 count;
    if (source == 0)
        return 0;
    if (destination == 0)
        return 0;
    count = 0;
    while (*source != 0) {
        value = *source;
        source++;
        if (value == 0)
            break;
        if (value == 13)
            break;
        if (value == 10)
            continue;
        *destination++ = value;
        count++;
    }
    *destination = 0;
    return source;
}
