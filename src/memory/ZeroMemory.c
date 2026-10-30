#include "types.h"

void ZeroMemory(u8 *memory, s32 size)
{
    s32 offset;

    for (offset = 0; offset < size; offset++) {
        *memory = 0;
        memory++;
    }
}
