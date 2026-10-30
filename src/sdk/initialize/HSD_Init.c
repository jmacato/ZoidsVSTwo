#include "initialize.h"

typedef struct HSD_MemReport {
    u32 total;
    u32 system;
    u32 xfb;
    u32 gxfifo;
    u32 heap;
} HSD_MemReport;

struct GXFifoObj;

extern HSD_MemReport memReport;
extern int iparam_xfb_max_num;
extern u32 iparam_fifo_size;
extern GXRenderModeObj *rmode;
extern struct GXFifoObj *DefaultFifoObj;

extern void DVDInit(void);
extern void **HSD_AllocateXFB(s32 count, GXRenderModeObj *mode);
extern void *HSD_AllocateFIFO(u32 size);
extern struct GXFifoObj *GXInit(void *fifo, u32 size);
extern void HSD_InitComponent(void);

void HSD_Init(void)
{
    void *fifo;
    struct GXFifoObj *object;

    DVDInit();
    HSD_AllocateXFB(iparam_xfb_max_num, rmode);
    fifo = HSD_AllocateFIFO(iparam_fifo_size);
    object = GXInit(fifo, iparam_fifo_size);
    memReport.gxfifo = iparam_fifo_size;
    DefaultFifoObj = object;
    HSD_InitComponent();
}
