#include "types.h"

u32 parsePackInfo(u8 **position)
{
    u8 byte;
    u32 count;
    s32 shift;

    byte = *((*position)++);
    count = ((byte >> 4) & 7) + 1;
    shift = 3;
    if ((byte & 0x80) == 0) {
        return count;
    }
    do {
        byte = *((*position)++);
        count += (byte & 0x7F) << shift;
        shift += 7;
    } while ((byte & 0x80) != 0);
    return count;
}
