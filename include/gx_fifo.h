/* Reference:
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/gx/GXFifo.h. */
#ifndef ZOIDS_GX_FIFO_H
#define ZOIDS_GX_FIFO_H
#include "types.h"
typedef struct GXFifoObj {
    u8 pad[128];
} GXFifoObj;
void GXInitFifoBase(GXFifoObj *fifo, void *base, u32 size);
void GXInitFifoPtrs(GXFifoObj *fifo, void *readPtr, void *writePtr);
void GXInitFifoLimits(GXFifoObj *fifo, u32 hiWatermark, u32 loWatermark);
void GXSetCPUFifo(GXFifoObj *fifo);
void GXSetGPFifo(GXFifoObj *fifo);
#endif
