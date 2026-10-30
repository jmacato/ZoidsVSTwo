#include "audio.h"

extern void SeOn(u16 id);

void SeWinon(void)
{
    SeOn(0x1d);
}
