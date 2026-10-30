/* Types from
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/video.h. */
#ifndef ZOIDS_VIDEO_H
#define ZOIDS_VIDEO_H
#include "initialize.h"
#define HSD_VI_XFB_MAX 3
#define HSD_ANTIALIAS_OVERLAP 4
#define VI_DISPLAY_PIX_SZ 2
#define HSD_ANTIALIAS_GARBAGE_SIZE (640 * HSD_ANTIALIAS_OVERLAP * VI_DISPLAY_PIX_SZ)

typedef void (*HSD_VIGXDrawDoneCallback)(int);
typedef void (*HSD_VIRetraceCallback)(u32);

typedef enum _HSD_VIXFBDrawDispStatus {
    HSD_VI_XFB_NONE,
    HSD_VI_XFB_NOUSE,
    HSD_VI_XFB_FREE,
    HSD_VI_XFB_DRAWING,
    HSD_VI_XFB_WAITDONE,
    HSD_VI_XFB_DRAWDONE,
    HSD_VI_XFB_NEXT,
    HSD_VI_XFB_DISPLAY,
    HSD_VI_XFB_COPYEFB,
    HSD_VI_XFB_TERMINATE
} HSD_VIXFBDrawDispStatus;

typedef enum _HSD_VIEFBDrawDispStatus {
    HSD_VI_EFB_FREE,
    HSD_VI_EFB_DRAWDONE,
    HSD_VI_EFB_TERMINATE
} HSD_VIEFBDrawDispStatus;

typedef enum _HSD_RenderPass {
    HSD_RP_SCREEN,
    HSD_RP_TOPHALF,
    HSD_RP_BOTTOMHALF,
    HSD_RP_OFFSCREEN,
    HSD_RP_NUM
} HSD_RenderPass;

typedef struct _current {
    HSD_VIStatus vi;
    u8 chg_flag;
} HSD_VICurrent;

typedef struct _XFB {
    void *buffer;
    HSD_VIXFBDrawDispStatus status;
    HSD_VICurrent vi_all;
} HSD_VIXFB;

typedef struct HSD_VIDataState {
    HSD_VICurrent current;

    HSD_VIXFB xfb[HSD_VI_XFB_MAX];

    struct _EFB {
        HSD_VIEFBDrawDispStatus status;
        HSD_VICurrent vi_all;
    } efb;

    s32 nb_xfb;

    HSD_VIRetraceCallback pre_cb;
    HSD_VIRetraceCallback post_cb;

    struct drawdone {
        s32 waiting;
        s32 arg;
        HSD_VIGXDrawDoneCallback cb;
    } drawdone;

    struct perf {
        s32 frame_period;
        s32 frame_renew;
    } perf;

} HSD_VIInfo;

extern HSD_VIInfo HSD_VIData;
extern int HSD_VIGetXFBDrawEnable(void);
extern void HSD_VICopyEFB2XFBPtr(HSD_VIStatus *status, void *buffer,
                                 HSD_RenderPass pass);
extern void HSD_VICopyXFB(HSD_RenderPass pass);
extern void HSD_VISetXFBDrawDone(void);
extern void HSD_VIWaitXFBFlush(void);
extern void HSD_VIWaitEFBFlush(void);
extern void HSD_VISetBlack(int black);
extern void HSD_VIInit(HSD_VIStatus *status, void *first, void *second, void *third);
#endif
