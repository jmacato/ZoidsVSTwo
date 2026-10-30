/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/video.c. */
#include "video.h"
extern int OSDisableInterrupts(void);
extern int OSRestoreInterrupts(int enabled);
extern void VIInit(void);
extern void VISetNextFrameBuffer(void *buffer);
extern void VIConfigure(GXRenderModeObj *mode);
extern void VISetBlack(int black);
extern void VIFlush(void);
extern void VIWaitForRetrace(void);
extern u32 VIGetTvFormat(void);
extern HSD_VIRetraceCallback VISetPreRetraceCallback(HSD_VIRetraceCallback callback);
extern HSD_VIRetraceCallback VISetPostRetraceCallback(HSD_VIRetraceCallback callback);
extern void (*GXSetDrawDoneCallback(void (*callback)(void)))(void);
extern void __assert(const char *file, u32 line, const char *condition);
extern void HSD_Panic(const char *file, u32 line, const char *message);
extern void GXSetCopyFilter(u8 aa, u8 samples[12][2], u8 vf, u8 filter[7]);
extern void GXSetDispCopyGamma(int gamma);
extern void HSD_StateSetColorUpdate(u8 enabled);
extern void HSD_StateSetAlphaUpdate(u8 enabled);
extern void HSD_StateSetZMode(u8 enabled, int compare, u8 write);
extern void GXSetCopyClear(GXColor color, u32 z);
extern void GXSetCopyClamp(int clamp);
extern void GXSetDispCopySrc(u16 left, u16 top, u16 width, u16 height);
extern u32 GXSetDispCopyYScale(f32 scale);
extern void GXSetDispCopyDst(u16 width, u16 height);
extern void GXCopyDisp(void *buffer, u8 clear);
extern void GXPixModeSync(void);
extern void GXWaitDrawDone(void);
extern void GXDrawDone(void);
#define GX_LEQUAL 3
#define GX_TRUE 1
#define VI_NTSC 0
#define GX_CLAMP_TOP 1
#define GX_CLAMP_BOTTOM 2
#define VIPadFrameBufferWidth(width) ((u16)(((width) + 15) & ~15))
#define HSD_ASSERT(line, condition)                                                    \
    do {                                                                               \
        if (!(condition))                                                              \
            __assert("video.c", line, #condition);                                     \
    } while (0)
HSD_VIInfo HSD_VIData;
static u8 garbage[HSD_ANTIALIAS_GARBAGE_SIZE] __attribute__((aligned(32)));
#define _p ((HSD_VIInfo *)&HSD_VIData)
void HSD_VICopyEFB2XFBPtr(HSD_VIStatus *vi, void *buffer, HSD_RenderPass pass);
static inline int HSD_VIGetNbXFB(void)
{
    return _p->nb_xfb;
}
static inline void *HSD_VIGetXFBPtr(int index)
{
    return _p->xfb[index].buffer;
}
static inline HSD_VIStatus *HSD_VIGetVIStatus(void)
{
    return &_p->current.vi;
}
static int HSD_VISearchXFBByStatus(HSD_VIXFBDrawDispStatus status)
{
    int i;

    for (i = 0; i < HSD_VI_XFB_MAX; i++) {
        if (_p->xfb[i].status == status) {
            return i;
        }
    }
    return -1;
}

static void HSD_VIPreRetraceCB(u32 retraceCount)
{
    int idx;
    int flush = 0;
    int renew = 0;

    idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_NEXT);
    if (idx != -1) {
        VISetNextFrameBuffer(_p->xfb[idx].buffer);
        if (_p->xfb[idx].vi_all.chg_flag) {
            VIConfigure(&_p->xfb[idx].vi_all.vi.rmode);
            VISetBlack(_p->xfb[idx].vi_all.vi.black);
        }
        flush = 1;
        renew = 1;
    } else if (HSD_VIGetNbXFB() == 1 && _p->efb.status == HSD_VI_EFB_DRAWDONE) {
        if ((idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_DISPLAY)) == -1) {
            idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_FREE);
            HSD_ASSERT(252, idx != -1);
            VISetNextFrameBuffer(_p->xfb[idx].buffer);
            flush = 1;
        }
        _p->xfb[idx].status = HSD_VI_XFB_COPYEFB;
        if (_p->efb.vi_all.chg_flag) {
            VIConfigure(&_p->efb.vi_all.vi.rmode);
            VISetBlack(_p->efb.vi_all.vi.black);
            flush = 1;
        }
        renew = 1;
    }

    if (flush) {
        VIFlush();
    }

    {
        static int vr_count = 0;
        static int renew_count = 0;

        if (renew) {
            renew_count++;
        }
        if (++vr_count >= _p->perf.frame_period) {
            _p->perf.frame_renew = renew_count;
            vr_count = renew_count = 0;
        }
    }

    if (_p->pre_cb) {
        _p->pre_cb(retraceCount);
    }
}

static void HSD_VIPostRetraceCB(u32 retraceCount)
{
    int idx;
    int next;

    if ((next = HSD_VISearchXFBByStatus(HSD_VI_XFB_NEXT)) != -1) {
        if ((idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_DISPLAY)) != -1) {
            _p->xfb[idx].status = HSD_VI_XFB_FREE;
        }
        _p->xfb[next].status = HSD_VI_XFB_DISPLAY;
        if ((idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_DRAWDONE)) != -1) {
            _p->xfb[idx].status = HSD_VI_XFB_NEXT;
        }
    } else if ((idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_COPYEFB)) != -1) {
        HSD_VICopyEFB2XFBPtr(&_p->efb.vi_all.vi, _p->xfb[idx].buffer, HSD_RP_SCREEN);
        _p->xfb[idx].status = HSD_VI_XFB_DISPLAY;
        _p->efb.status = HSD_VI_EFB_FREE;
    }

    if (_p->post_cb) {
        _p->post_cb(retraceCount);
    }
}

static void HSD_VIGXDrawDoneCB(void)
{
    _p->drawdone.waiting = 0;

    if (_p->drawdone.cb) {
        _p->drawdone.cb(_p->drawdone.arg);
    }
}

/* Keep the release SDK call to the waiting-flag accessor. */
#pragma push
#pragma dont_inline on
static int HSD_VIGetDrawDoneWaitingFlag(void)
{
    return _p->drawdone.waiting;
}
#pragma pop

int HSD_VIGetXFBDrawEnable(void)
{
    int intr;
    int idx = -1;

    if (HSD_VIGetNbXFB() >= 2) {
        intr = OSDisableInterrupts();

        if ((idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_DRAWING)) == -1) {
            if ((idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_FREE)) != -1) {
                _p->xfb[idx].status = HSD_VI_XFB_DRAWING;
            }
        }

        OSRestoreInterrupts(intr);
    }
    return idx;
}

static inline void HSD_VICopyEFB2XFBHiResoAA(GXRenderModeObj *rmode)
{
    int n_xfb_lines;

    GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight - HSD_ANTIALIAS_OVERLAP);
    n_xfb_lines = GXSetDispCopyYScale(1.0);
    GXSetDispCopyDst(rmode->fbWidth, n_xfb_lines);
}

void HSD_VICopyEFB2XFBPtr(HSD_VIStatus *vi, void *buffer, HSD_RenderPass rpass)
{
    GXRenderModeObj *rmode = &vi->rmode;
    int n_xfb_lines;
    u16 lines;
    u32 offset;

    GXSetCopyFilter(rmode->aa, rmode->sample_pattern, vi->vf, rmode->vfilter);
    GXSetDispCopyGamma(vi->gamma);

    HSD_StateSetColorUpdate(vi->update_clr);
    HSD_StateSetAlphaUpdate(vi->update_alpha);
    HSD_StateSetZMode(vi->update_z, GX_LEQUAL, GX_TRUE);

    GXSetCopyClear(vi->clear_clr, vi->clear_z);

    switch (rpass) {
    case HSD_RP_SCREEN:
        GXSetCopyClamp((int)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM));
        GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
        n_xfb_lines =
            GXSetDispCopyYScale((f32)(rmode->xfbHeight) / (f32)(rmode->efbHeight));
        GXSetDispCopyDst(rmode->fbWidth, n_xfb_lines);
        GXCopyDisp(buffer, GX_TRUE);
        break;

    case HSD_RP_TOPHALF:
        HSD_VICopyEFB2XFBHiResoAA(rmode);
        GXSetCopyClamp(GX_CLAMP_TOP);
        lines = rmode->efbHeight - HSD_ANTIALIAS_OVERLAP;
        GXSetDispCopySrc(0, 0, rmode->fbWidth, lines);
        GXCopyDisp(buffer, GX_TRUE);
        GXPixModeSync();
        return;

    case HSD_RP_BOTTOMHALF:
        HSD_VICopyEFB2XFBHiResoAA(rmode);
        GXSetCopyClamp(GX_CLAMP_BOTTOM);
        lines = rmode->efbHeight - HSD_ANTIALIAS_OVERLAP;
        GXSetDispCopySrc(0, HSD_ANTIALIAS_OVERLAP, rmode->fbWidth, lines);
        offset =
            (VIPadFrameBufferWidth(rmode->fbWidth) * lines * (u32)VI_DISPLAY_PIX_SZ);
        GXCopyDisp((void *)((u32)buffer + offset), GX_TRUE);
        GXSetDispCopySrc(0, 0, rmode->fbWidth, HSD_ANTIALIAS_OVERLAP);
        GXSetCopyClamp((int)(GX_CLAMP_TOP | GX_CLAMP_BOTTOM));
        GXCopyDisp((void *)garbage, GX_TRUE);
        break;

    default:
        HSD_Panic("video.c", 0x207, "unexpected type of render pass.\n");
    }

    GXPixModeSync();
}

static inline int HSD_VIWaitXFBFlush_sub(void)
{
    int intr;
    int val;

    intr = OSDisableInterrupts();

    val = (HSD_VISearchXFBByStatus(HSD_VI_XFB_WAITDONE) != -1 ||
           HSD_VISearchXFBByStatus(HSD_VI_XFB_DRAWDONE) != -1 ||
           HSD_VISearchXFBByStatus(HSD_VI_XFB_NEXT) != -1)
              ? 1
              : 0;

    OSRestoreInterrupts(intr);

    return val;
}

void HSD_VIWaitXFBFlush(void)
{
    if (HSD_VIGetNbXFB() < 2) {
        return;
    }

    while (HSD_VIWaitXFBFlush_sub()) {
        VIWaitForRetrace();
    }
}

void HSD_VISetBlack(int black)
{
    _p->current.vi.black = black;
    _p->current.chg_flag = 1;
}

void HSD_VIInit(HSD_VIStatus *vi, void *xfb0, void *xfb1, void *xfb2)
{
    int i, fbnum, idx;

    VIInit();

    _p->current.vi = *vi;
    _p->current.chg_flag = 0;
    _p->xfb[0].buffer = xfb0;
    _p->xfb[1].buffer = xfb1;
    _p->xfb[2].buffer = xfb2;

    for (i = 0, fbnum = 0; i < HSD_VI_XFB_MAX; i++) {
        _p->xfb[i].vi_all = _p->current;
        if (_p->xfb[i].buffer) {
            fbnum++;
            _p->xfb[i].status = HSD_VI_XFB_FREE;
        } else {
            _p->xfb[i].status = HSD_VI_XFB_NONE;
        }
    }

    _p->nb_xfb = fbnum;

    _p->efb.status = HSD_VI_EFB_FREE;
    _p->efb.vi_all = _p->current;

    VISetPreRetraceCallback(HSD_VIPreRetraceCB);
    VISetPostRetraceCallback(HSD_VIPostRetraceCB);

    _p->pre_cb = 0;
    _p->post_cb = 0;

    _p->drawdone.waiting = 0;
    _p->drawdone.arg = 0;

    GXSetDrawDoneCallback(HSD_VIGXDrawDoneCB);
    _p->drawdone.cb = 0;

    _p->perf.frame_period = VIGetTvFormat() == VI_NTSC ? 60 : 50;
    _p->perf.frame_renew = 0;

    VIConfigure(&_p->current.vi.rmode);
    VISetBlack(_p->current.vi.black);
    VIFlush();

    idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_FREE);
    HSD_VICopyEFB2XFBPtr(HSD_VIGetVIStatus(), HSD_VIGetXFBPtr(idx), HSD_RP_SCREEN);
}

static inline int HSD_VIWaitXFBDrawEnable(void)
{
    int index = -1;
    if (HSD_VIGetNbXFB() < 2)
        goto ret;
    while ((index = HSD_VIGetXFBDrawEnable()) == -1)
        VIWaitForRetrace();
ret:
    return index;
}
static inline void HSD_VIGXSetDrawDone(int arg)
{
    while (HSD_VIGetDrawDoneWaitingFlag())
        GXWaitDrawDone();
    _p->drawdone.waiting = 1;
    _p->drawdone.arg = arg;
    GXDrawDone();
}
void HSD_VICopyXFB(HSD_RenderPass pass)
{
    int index;
    if (HSD_VIGetNbXFB() < 2)
        return;
    index = HSD_VIWaitXFBDrawEnable();
    HSD_VICopyEFB2XFBPtr(HSD_VIGetVIStatus(), HSD_VIGetXFBPtr(index), pass);
    HSD_VIGXSetDrawDone(index);
}
static inline void HSD_VISetXFBWaitDone(int idx)
{
    int intr = OSDisableInterrupts();
    HSD_ASSERT(590, _p->xfb[idx].status == HSD_VI_XFB_DRAWING);
    _p->xfb[idx].status = HSD_VI_XFB_WAITDONE;
    _p->xfb[idx].vi_all = _p->current;
    _p->current.chg_flag = 0;
    OSRestoreInterrupts(intr);
}
static inline void HSD_VIDrawDoneXFB(int idx)
{
    int intr = OSDisableInterrupts();
    HSD_ASSERT(722, _p->xfb[idx].status == HSD_VI_XFB_WAITDONE);
    _p->xfb[idx].status = HSD_VISearchXFBByStatus(HSD_VI_XFB_NEXT) != -1
                              ? HSD_VI_XFB_DRAWDONE
                              : HSD_VI_XFB_NEXT;
    OSRestoreInterrupts(intr);
}
void HSD_VISetXFBDrawDone(void)
{
    int idx;
    int intr;
    if (HSD_VIGetNbXFB() < 2)
        return;
    intr = OSDisableInterrupts();
    idx = HSD_VISearchXFBByStatus(HSD_VI_XFB_DRAWING);
    HSD_ASSERT(763, idx != -1);
    HSD_VISetXFBWaitDone(idx);
    HSD_VIDrawDoneXFB(idx);
    OSRestoreInterrupts(intr);
}
static inline int HSD_VIWaitEFBFlush_sub(void)
{
    int intr = OSDisableInterrupts();
    int waiting = _p->efb.status != HSD_VI_EFB_FREE ? 1 : 0;
    OSRestoreInterrupts(intr);
    return waiting;
}
void HSD_VIWaitEFBFlush(void)
{
    if (HSD_VIGetNbXFB() > 1)
        return;
    while (HSD_VIWaitEFBFlush_sub())
        VIWaitForRetrace();
}
