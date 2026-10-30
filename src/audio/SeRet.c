#include "audio.h"

extern void SeOn(u16 id);

void SeRet(void)
{
    SeOn(0x10);
}
