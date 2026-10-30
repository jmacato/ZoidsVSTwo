#include "types.h"

extern void sWeaponMountU(s32 part, void *zoid, s32 slot, s32 mount, s32 flags);

void sWeaponMount(s32 part, void *zoid, s32 slot, s32 mount)
{
    sWeaponMountU(part, zoid, slot, mount, 0);
}
