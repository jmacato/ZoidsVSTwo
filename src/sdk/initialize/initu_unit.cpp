extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"

#include "initialize.h"
#include "runtime_stdarg.h"
typedef struct HSD_MemReport {
    u32 total;
    u32 system;
    u32 xfb;
    u32 gxfifo;
    u32 heap;
} HSD_MemReport;
extern GXRenderModeObj GXNtsc480IntDf;
extern void *OSGetArenaLo(void);
extern void *OSGetArenaHi(void);
extern void OSSetArenaLo(void *);
extern u32 OSGetPhysicalMemSize(void);
extern s32 OSCreateHeap(void *, void *);
extern s32 OSSetCurrentHeap(s32);
extern void *OSAllocFromArenaLo(u32, s32);
extern void OSReport(const char *, ...);
extern void HSD_ObjSetHeap(u32, void *);
extern void HSD_Panic(const char *, s32, const char *);
extern void HSD_VIInit(HSD_VIStatus *, void *, void *, void *);
extern void HSD_IDSetup(void);
extern void VIWaitForRetrace(void);
#pragma readonly_strings off
typedef struct GXLightObj {
    u32 words[16];
} GXLightObj;
extern void GXInitLightPos(GXLightObj *light, f32 x, f32 y, f32 z);
extern void GXInitLightDir(GXLightObj *light, f32 x, f32 y, f32 z);
extern void GXInitLightAttn(GXLightObj *light, f32 a0, f32 a1, f32 a2, f32 k0, f32 k1, f32 k2);
extern void GXInitLightColor(GXLightObj *light, GXColor color);
extern void GXLoadLightObjImm(GXLightObj *light, int id);
extern int HSD_Index2LightID(int index);
extern void HSD_StateInvalidate(u32 flags);

void HSD_InitComponent(void);
void **HSD_AllocateXFB(s32 count, GXRenderModeObj *mode);
void HSD_GXInit(void);
void HSD_OSInit(void);
int HSD_SetInitParameter(HSD_InitParam param, ...);
#pragma force_active on
static void *FrameBuffer[3];
static HSD_MemReport memReport;
static void *hsd_heap_arena_lo;
static void *hsd_heap_arena_hi;
static void *hsd_heap_next_arena_lo;
static void *hsd_heap_next_arena_hi;
static s32 current_render_pass;
static s32 DefaultFifoObj;
static s32 current_pix_fmt;
static s32 init_done;
static s32 iparam_heap_size;
static s32 hsd_heap = -1;
static GXRenderModeObj *rmode = &GXNtsc480IntDf;
#pragma force_active off
void HSD_InitComponent(void)
{
    HSD_VIStatus vi;
    HSD_OSInit();
    vi.rmode = *rmode;
    vi.black = 1;
    vi.vf = 1;
    vi.gamma = 0;
    {
        GXColor clr = {0, 0, 0, 0};
        vi.clear_clr = clr;
    }
    vi.clear_z = 0xFFFFFF;
    vi.update_clr = 1;
    vi.update_alpha = 1;
    vi.update_z = 1;
    HSD_VIInit(&vi, FrameBuffer[0], FrameBuffer[1], FrameBuffer[2]);
    HSD_GXInit();
    HSD_DVDInit();
    HSD_IDSetup();
    VIWaitForRetrace();
    HSD_ObjInit();
    init_done = 1;
}

static s32 current_z_fmt = 2;
static u32 iparam_fifo_size = 0x40000;
static u32 iparam_xfb_max_num = 2;
void **HSD_AllocateXFB(s32 count, GXRenderModeObj *mode)
{
    s32 i;
    u32 size;
    if (mode == 0)
        return 0;
    size = (u16)((mode->fbWidth + 15) & ~15) * mode->xfbHeight * 2;
    memReport.xfb = size * count;
    for (i = 0; i < count; i++) {
        if ((FrameBuffer[i] = OSAllocFromArenaLo(size, 32)) == 0)
            HSD_Panic("initialize.c", 232, "No memory space remains for XFB.\n");
    }
    for (i = count; i < 3; i++)
        FrameBuffer[i] = 0;
    return FrameBuffer;
}

void HSD_GXInit(void)
{
    GXLightObj light;
    int i;
    static GXColor black = {0};

    GXInitLightPos(&light, 1, 0, 0);
    GXInitLightDir(&light, 1, 0, 0);
    GXInitLightAttn(&light, 1, 0, 0, 1, 0, 0);
    GXInitLightColor(&light, black);
    for (i = 0; i < 8; i++)
        GXLoadLightObjImm(&light, HSD_Index2LightID(i));
    HSD_StateInvalidate(-1);
}

void HSD_OSInit(void)
{
    u32 lo;
    OSGetArenaLo();
    OSGetArenaHi();
    memReport.total = OSGetPhysicalMemSize();
    lo = (u32)OSGetArenaLo();
    memReport.system = memReport.total - (u32)OSGetArenaHi() - (0 - lo) - memReport.xfb - memReport.gxfifo;
    if (hsd_heap_next_arena_lo != 0 && hsd_heap_next_arena_hi != 0) {
        hsd_heap_arena_lo = hsd_heap_next_arena_lo;
        hsd_heap_arena_hi = hsd_heap_next_arena_hi;
        hsd_heap_next_arena_lo = 0;
        hsd_heap_next_arena_hi = 0;
        hsd_heap = OSCreateHeap(hsd_heap_arena_lo, hsd_heap_arena_hi);
        OSSetCurrentHeap(hsd_heap);
        memReport.heap = (u32)hsd_heap_arena_hi - (u32)hsd_heap_arena_lo;
        HSD_ObjSetHeap(memReport.heap, 0);
    } else {
        u32 hi;
        lo = ((u32)OSGetArenaLo() + 0x1F) & ~0x1F;
        hi = (u32)OSGetArenaHi();
        hsd_heap_arena_lo = (void *)lo;
        if (iparam_heap_size > 0) {
            hsd_heap_arena_hi = (void *)(lo + iparam_heap_size);
            if ((u32)hsd_heap_arena_hi > hi)
                hsd_heap_arena_hi = (void *)hi;
            hsd_heap_arena_hi = (void *)((u32)hsd_heap_arena_hi & ~0x1F);
        } else {
            hsd_heap_arena_hi = (void *)(hi & ~0x1F);
        }
        hsd_heap = OSCreateHeap(hsd_heap_arena_lo, hsd_heap_arena_hi);
        OSSetCurrentHeap(hsd_heap);
        memReport.heap = (u32)hsd_heap_arena_hi - (u32)hsd_heap_arena_lo;
        HSD_ObjSetHeap(memReport.heap, 0);
        OSSetArenaLo(hsd_heap_arena_hi);
    }
}

static char lit_381[31] = "no space remains for gx fifo.\n";
static char lit_416[19] = "missing argument.\n";
static char lit_439[24] __attribute__((aligned(8))) = "sysdolphin_base_library";
static char lit_463[28] = "pix_fmt != GX_PF_RGB565_Z16";
static char lit_506[11] = "objalloc.h";
static char lit_508[40] __attribute__((aligned(8))) = "objalloc: %s\tusing %d\tfreed %d\tpeak %d\n";
int HSD_SetInitParameter(HSD_InitParam param, ...)
{
    va_list ap;
    int result = 0;
    static s32 shown;
    u32 value;
    u32 lo;
    u32 hi;
    if (init_done) {
        if (!shown) {
            OSReport("init parameter should be set before invoking HSD_Init().\n");
            shown = 1;
        }
        return 0;
    }
    va_start(ap, param);
    switch (param) {
    case HSD_INIT_FIFO_SIZE:
        value = va_arg(ap, u32);
        if (value) {
            iparam_fifo_size = value;
            result = 1;
        }
        break;
    case HSD_INIT_XFB_MAX_NUM:
        value = va_arg(ap, u32);
        if (value) {
            iparam_xfb_max_num = value;
            result = 1;
        }
        break;
    case HSD_INIT_HEAP_SIZE:
        value = va_arg(ap, u32);
        if (value < OSGetPhysicalMemSize()) {
            iparam_heap_size = value;
            result = 1;
        }
        break;
    case HSD_INIT_HEAP_ARENA:
        lo = va_arg(ap, u32);
        hi = va_arg(ap, u32);
        result = 1;
        hsd_heap_next_arena_lo = (void *)lo;
        hsd_heap_next_arena_hi = (void *)hi;
        break;
    case HSD_INIT_RENDER_MODE_OBJ:
        value = va_arg(ap, u32);
        if (value) {
            rmode = (GXRenderModeObj *)value;
            result = 1;
        }
        break;
    case HSD_INIT_HEAP_MAX_NUM:
        OSReport("ERROR in HSD_SetInitParameter():\n");
        OSReport("  HSD_INIT_HEAP_MAX_NUM is obsolete since 1.3.0.0. \n");
        if (va_arg(ap, s32) == 0)
            result = 1;
        break;
    case HSD_INIT_AUDIO_HEAP_SIZE:
        OSReport("ERROR in HSD_SetInitParameter():\n");
        OSReport("  HSD_INIT_AUDIO_HEAP_SIZE is obsolete since 1.3.0.0. \n");
        break;
    }
    return result;
}

#pragma force_active on
s32 need_invalidate;
#pragma force_active off
}
