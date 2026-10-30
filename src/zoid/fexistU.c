#include "types.h"

extern s32 usbForce;
extern s32 fexist(char *path);

s32 fexistU(char *name, s32 mode)
{
    if (usbForce != 0) {
        mode = 1;
    }
    return fexist(name);
}
