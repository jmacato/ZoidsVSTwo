#include "audio.h"

extern void SeOn(u16 id);

void SeConfirm(void)
{
    SeOn(0x71);
}
