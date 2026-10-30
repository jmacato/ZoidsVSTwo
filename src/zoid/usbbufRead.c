#include "types.h"

extern u8 hioRecvBuf[];

void usbbufRead(u8 *buf, u16 offset, u16 size)
{
    u16 i = 0;
    for (i = 0; i < size; i++)
        buf[i] = hioRecvBuf[i + offset];
}
