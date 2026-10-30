#include "audio.h"

extern void SeOn(u16 id);

void SeSelectB(void)
{
    SeOn(0x6f);
}
