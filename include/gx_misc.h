#ifndef ZOIDS_GX_MISC_H
#define ZOIDS_GX_MISC_H
#include "gx_pixel.h"
typedef enum _GXMiscToken {
    GX_MT_XF_FLUSH = 1,
    GX_MT_DL_SAVE_CONTEXT = 2,
    GX_MT_NULL = 0,
} GXMiscToken;
typedef enum _GXAlphaReadMode {
    GX_READ_00,
    GX_READ_FF,
    GX_READ_NONE,
} GXAlphaReadMode;
typedef void (*GXDrawSyncCallback)(u16);
typedef void (*GXDrawDoneCallback)(void);
void GXSetMisc(GXMiscToken token, u32 val);
void GXFlush(void);
void GXWaitDrawDone(void);
void GXDrawDone(void);
void GXPixModeSync(void);
void GXPokeAlphaMode(GXCompare func, u8 threshold);
void GXPokeAlphaRead(GXAlphaReadMode mode);
void GXPokeAlphaUpdate(u8 update_enable);
void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor,
                     GXBlendFactor dst_factor, GXLogicOp op);
void GXPokeColorUpdate(u8 update_enable);
void GXPokeDstAlpha(u8 enable, u8 alpha);
void GXPokeDither(u8 dither);
void GXPokeZMode(u8 compare_enable, GXCompare func, u8 update_enable);
GXDrawDoneCallback GXSetDrawDoneCallback(GXDrawDoneCallback cb);
#endif
