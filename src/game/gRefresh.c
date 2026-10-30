#include "types.h"

extern void GXClearVtxDesc(void);
extern void GXSetNumChans(u8 count);
extern void GXSetNumTexGens(u8 count);
extern void GXSetTevClampMode(s32 stage, s32 mode);
extern void GXSetNumTevStages(u8 count);
extern void GXSetTevOp(s32 stage, s32 mode);
extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
extern void GXSetTexCoordGen2(s32 coord, s32 function, s32 source, u32 matrix, u8 normalize, u32 post_matrix);
extern void GXInvalidateVtxCache(void);
extern void GXInvalidateTexAll(void);
extern void HSD_StateInvalidate(u32 flags);

void gRefresh(void)
{
    GXClearVtxDesc();
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevClampMode(0, 0);
    GXSetNumTevStages(1);
    GXSetTevOp(0, 2);
    GXSetTevOrder(0, 0, 0, 0xff);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
    HSD_StateInvalidate(-1);
}
