#include "dvd_file.h"

extern s32 rCbkR;

void rCbk(s32 result, DVDFileInfo *file)
{
    if (result < 0) {
        rCbkR = 2;
    }
    rCbkR = 3;
}
