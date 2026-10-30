#include "card.h"
#include "exi.h"

s32 CARDProbe(s32 chan)
{
    if (*(u8 *)0x800030E3 & 0x80) {
        return 0;
    }
    return EXIProbe(chan);
}
