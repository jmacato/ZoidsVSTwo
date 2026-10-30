#include "types.h"

char toLow(s8 c)
{
    if (c < 0x41) {
        return c;
    }
    if (c > 0x5a) {
        return c;
    }
    return c | 0x20;
}
