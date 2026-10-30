/* Reference: https://github.com/doldecomp/dolsdk2001/blob/main/src/gx/GXDisplayList.c.
 */
#include "gx_state.h"
void GXCallDisplayList(void *list, u32 nbytes)
{

    if (gx->dirtyState != 0) {
        __GXSetDirtyState();
    }

    if (*(u32 *)&gx->vcdClear == 0) {
        __GXSendFlushPrim();
    }
    GX_WRITE_U8(0x40);
    GX_WRITE_U32(list);
    GX_WRITE_U32(nbytes);
}
