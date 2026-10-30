#ifndef ZOIDS_INITIALIZE_H
#define ZOIDS_INITIALIZE_H
#include "types.h"
#include "gx_tev.h"

typedef struct GXRenderModeObj {
    u32 viTVmode;
    u16 fbWidth;
    u16 efbHeight;
    u16 xfbHeight;
    u16 viXOrigin;
    u16 viYOrigin;
    u16 viWidth;
    u16 viHeight;
    u32 xfbMode;
    u8 field_rendering;
    u8 aa;
    u8 sample_pattern[12][2];
    u8 vfilter[7];
} GXRenderModeObj;
typedef struct HSD_VIStatus {
    GXRenderModeObj rmode;
    int black;
    u8 vf;
    int gamma;
    GXColor clear_clr;
    u32 clear_z;
    u8 update_clr;
    u8 update_alpha;
    u8 update_z;
} HSD_VIStatus;

typedef enum HSD_InitParam {
    HSD_INIT_FIFO_SIZE,
    HSD_INIT_XFB_MAX_NUM,
    HSD_INIT_HEAP_MAX_NUM,
    HSD_INIT_AUDIO_HEAP_SIZE,
    HSD_INIT_RENDER_MODE_OBJ,
    HSD_INIT_HEAP_SIZE,
    HSD_INIT_HEAP_ARENA
} HSD_InitParam;

extern void HSD_InitComponent(void);
extern void HSD_Init(void);
extern void HSD_DVDInit(void);
extern void **HSD_AllocateXFB(s32 count, GXRenderModeObj *mode);
extern void *HSD_AllocateFIFO(u32 size);
extern void HSD_GXInit(void);
extern void HSD_OSInit(void);
extern s32 HSD_GetHeap(void);
extern void HSD_GetCurrentArena(void **lo, void **hi);
extern void HSD_ClearHeap(void);
extern int HSD_GetCurrentRenderPass(void);
extern void HSD_StartRender(int pass);
extern void HSD_EndRender(void);
extern void _HSD_NeedCacheInvalidate(int flags);
extern void HSD_ObjInit(void);
extern void HSD_ObjDumpStat(void);
extern int HSD_SetInitParameter(HSD_InitParam param, ...);
#endif
