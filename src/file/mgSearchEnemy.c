#include "types.h"

extern s32 enemyChara[2];
extern s32 enemyZoid[2];

void mgSearchEnemy(u8 *state)
{
    s32 i;
    s32 count;
    if (state == (void *)0)
        return;
    for (i = 0; i < 2; i++) {
        enemyChara[i] = -1;
        enemyZoid[i] = -1;
    }
    for (i = 0, count = 0; i < 0x11; i++) {
        u8 *p;
        if (count == 2)
            break;
        p = state + 0x12C + i * 0x24C;
        if (*(s32 *)(p + 0x20) != 1)
            continue;
        if (*(s8 *)(p + 0xBC) == 0)
            continue;
        if (*(s8 *)(p + 0x111) == 1)
            continue;
        enemyChara[count] = (s8)*(u8 *)(p + 0x110);
        enemyZoid[count] = *(s32 *)(p + 0xC0);
        count++;
    }
}
