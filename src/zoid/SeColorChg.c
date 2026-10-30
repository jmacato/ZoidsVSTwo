#include "audio.h"

extern void SeOn(u16 id);

void SeColorChg(void)
{
    SeOn(0x70);
}
