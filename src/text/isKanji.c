#include "types.h"
s32 isKanji(s32 c)
{
    u8 lead = c & 0xe0;
    return lead == 0xe0 || lead == 0x80;
}
