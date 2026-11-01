/* References: https://github.com/doldecomp/dolsdk2001/blob/main/src/ar/ar.c
 * and https://github.com/doldecomp/dolsdk2004/blob/main/src/ar/ar.c. */
#include "ar.h"
#include "os_context.h"
#include "os_interrupt.h"
#define OSPhysicalToUncached(address) ((void *)((u32)(address) + 0xC0000000))
extern volatile u16 __DSPRegs[32];
__asm__(".set __DSPRegs, 0xCC005000");
extern void DCFlushRange(void *, u32);
extern void DCInvalidateRange(void *, u32);
extern void PPCSync(void);
extern void *memset(void *, s32, u32);
static ARCallback __AR_Callback = 0;
static u32 __AR_Size = 0;
static u32 __AR_InternalSize = 0;
static u32 __AR_ExpansionSize = 0;
static u32 __AR_StackPointer = 0;
static u32 __AR_FreeBlocks = 0;
static u32 *__AR_BlockLength = 0;
static s32 __AR_init_flag = 0;
static void __ARHandler(s16, OSContext *);
static void __ARChecksize(void);
#define DSP_ARAM_DMA_MM_HI 16
#define DSP_ARAM_DMA_MM_LO 17
#define DSP_ARAM_DMA_ARAM_HI 18
#define DSP_ARAM_DMA_ARAM_LO 19
#define DSP_ARAM_DMA_SIZE_HI 20
#define DSP_ARAM_DMA_SIZE_LO 21
static inline void __ARClearInterrupt(void)
{
    u16 tmp;

    tmp = __DSPRegs[5];
    tmp = (tmp & ~0x88) | 0x20;
    __DSPRegs[5] = (tmp);
}
static inline void __ARWaitForDMA(void)
{
    while (__DSPRegs[5] & 0x200)
        ;
}
static inline void __ARWriteDMA(u32 mmem_addr, u32 aram_addr, u32 length)
{

    __DSPRegs[DSP_ARAM_DMA_MM_HI] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_MM_HI] & ~0x03ff) | (u16)(mmem_addr >> 16));
    __DSPRegs[DSP_ARAM_DMA_MM_LO] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_MM_LO] & ~0xffe0) | (u16)(mmem_addr & 0xffff));

    __DSPRegs[DSP_ARAM_DMA_ARAM_HI] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_ARAM_HI] & ~0x03ff) | (u16)(aram_addr >> 16));
    __DSPRegs[DSP_ARAM_DMA_ARAM_LO] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_ARAM_LO] & ~0xffe0) | (u16)(aram_addr & 0xffff));

    __DSPRegs[DSP_ARAM_DMA_SIZE_HI] = (u16)(__DSPRegs[DSP_ARAM_DMA_SIZE_HI] & ~0x8000);

    __DSPRegs[DSP_ARAM_DMA_SIZE_HI] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_SIZE_HI] & ~0x03ff) | (u16)(length >> 16));
    __DSPRegs[DSP_ARAM_DMA_SIZE_LO] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_SIZE_LO] & ~0xffe0) | (u16)(length & 0xffff));

    __ARWaitForDMA();
    __ARClearInterrupt();
}
static inline void __ARReadDMA(u32 mmem_addr, u32 aram_addr, u32 length)
{

    __DSPRegs[DSP_ARAM_DMA_MM_HI] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_MM_HI] & ~0x03ff) | (u16)(mmem_addr >> 16));
    __DSPRegs[DSP_ARAM_DMA_MM_LO] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_MM_LO] & ~0xffe0) | (u16)(mmem_addr & 0xffff));

    __DSPRegs[DSP_ARAM_DMA_ARAM_HI] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_ARAM_HI] & ~0x03ff) | (u16)(aram_addr >> 16));
    __DSPRegs[DSP_ARAM_DMA_ARAM_LO] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_ARAM_LO] & ~0xffe0) | (u16)(aram_addr & 0xffff));

    __DSPRegs[DSP_ARAM_DMA_SIZE_HI] = (u16)(__DSPRegs[DSP_ARAM_DMA_SIZE_HI] | 0x8000);

    __DSPRegs[DSP_ARAM_DMA_SIZE_HI] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_SIZE_HI] & ~0x03ff) | (u16)(length >> 16));
    __DSPRegs[DSP_ARAM_DMA_SIZE_LO] =
        (u16)((__DSPRegs[DSP_ARAM_DMA_SIZE_LO] & ~0xffe0) | (u16)(length & 0xffff));

    __ARWaitForDMA();
    __ARClearInterrupt();
}
ARCallback ARRegisterDMACallback(ARCallback callback)
{
    ARCallback old_callback;
    s32 old;

    old_callback = __AR_Callback;
    old = OSDisableInterrupts();
    __AR_Callback = callback;
    OSRestoreInterrupts(old);
    return old_callback;
}
void ARStartDMA(u32 type, u32 mainmem_addr, u32 aram_addr, u32 length)
{
    s32 old;

    old = OSDisableInterrupts();
    __DSPRegs[16] = (__DSPRegs[16] & 0xFFFFFC00 | (mainmem_addr >> 0x10));
    __DSPRegs[17] = (__DSPRegs[17] & 0xFFFF001F | ((u16)mainmem_addr));
    __DSPRegs[18] = (__DSPRegs[18] & 0xFFFFFC00 | (aram_addr >> 0x10));
    __DSPRegs[19] = (__DSPRegs[19] & 0xFFFF001F | ((u16)aram_addr));
    __DSPRegs[20] = __DSPRegs[20] & ~0x8000 | ((type << 0xF) & ~0x7FFF);
    __DSPRegs[20] = (__DSPRegs[20] & 0xFFFFFC00) | (length >> 0x10);
    __DSPRegs[21] = (__DSPRegs[21] & 0xFFFF001F) | (length & 0x0000FFFF);
    OSRestoreInterrupts(old);
}
u32 ARAlloc(u32 length)
{
    u32 tmp;
    s32 old;

    old = OSDisableInterrupts();

    tmp = __AR_StackPointer;
    __AR_StackPointer += length;
    *__AR_BlockLength = length;
    __AR_BlockLength += 1;
    __AR_FreeBlocks -= 1;
    OSRestoreInterrupts(old);
    return tmp;
}
u32 ARInit(u32 *stack_index_addr, u32 num_entries)
{
    s32 old;
    u16 refresh;

    if (__AR_init_flag == 1) {
        return 0x4000;
    }

    old = OSDisableInterrupts();
    __AR_Callback = 0;
    __OSSetInterruptHandler(__OS_INTERRUPT_DSP_ARAM, __ARHandler);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_DSP_ARAM);
    __AR_StackPointer = 0x4000;
    __AR_FreeBlocks = num_entries;
    __AR_BlockLength = stack_index_addr;
    refresh = __DSPRegs[13] & 0xFF;

    __DSPRegs[13] = (u16)((__DSPRegs[13] & ~0xFF) | (refresh & 0xFF));

    __ARChecksize();
    __AR_init_flag = 1;
    OSRestoreInterrupts(old);
    return __AR_StackPointer;
}
u32 ARGetBaseAddress(void)
{
    return 0x4000;
}
u32 ARGetSize(void)
{
    return __AR_Size;
}
static void __ARHandler(s16 exception, OSContext *context)
{
    OSContext exceptionContext;
    u16 tmp;

    tmp = __DSPRegs[5];
    tmp = (tmp & ~0x88) | 0x20;
    __DSPRegs[5] = (tmp);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    if (__AR_Callback) {
        __AR_Callback();
    }
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(context);
}
static void __ARChecksize(void)
{
    u8 test_data_pad[63];
    u8 dummy_data_pad[63];
    u8 buffer_pad[63];
    u32 *test_data;
    u32 *dummy_data;
    u32 *buffer;
    u16 ARAM_mode;
    u32 ARAM_size;
    u32 i;

    ARAM_mode = 0;
    ARAM_size = 0;
    do {
    } while (!(__DSPRegs[11] & 1));
    ARAM_mode = 3;
    ARAM_size = __AR_InternalSize = 0x1000000;
    __DSPRegs[9] = ((__DSPRegs[9] & 0xFFFFFFC0) | 3) | 0x20;
    test_data = (void *)(((u32)&test_data_pad + 0x1F) & 0xFFFFFFE0);
    dummy_data = (void *)(((u32)&dummy_data_pad + 0x1F) & 0xFFFFFFE0);
    buffer = (void *)(((u32)&buffer_pad + 0x1F) & 0xFFFFFFE0);
    for (i = 0; i < 8; i++) {
        test_data[i] = 0xDEADBEEF;
        dummy_data[i] = 0xBAD0BAD0;
    }
    DCFlushRange(test_data, 0x20);
    DCFlushRange(dummy_data, 0x20);
    __AR_ExpansionSize = 0;
    __ARWriteDMA((u32)dummy_data, ARAM_size, 0x20U);
    __ARWriteDMA((u32)dummy_data, ARAM_size + 0x200000, 0x20U);
    __ARWriteDMA((u32)dummy_data, ARAM_size + 0x01000000, 0x20U);
    __ARWriteDMA((u32)dummy_data, ARAM_size + 0x200, 0x20U);
    __ARWriteDMA((u32)dummy_data, ARAM_size + 0x400000, 0x20U);
    memset(buffer, 0, 0x20);
    DCFlushRange(buffer, 0x20);
    __ARWriteDMA((u32)test_data, ARAM_size, 0x20U);
    DCInvalidateRange(buffer, 0x20);
    __ARReadDMA((u32)buffer, ARAM_size, 0x20U);
    PPCSync();
    if (*buffer == *test_data) {
        memset(buffer, 0, 0x20);
        DCFlushRange(buffer, 0x20);
        __ARReadDMA((u32)buffer, ARAM_size + 0x200000, 0x20U);
        PPCSync();
        if (*buffer == *test_data) {
            __AR_ExpansionSize = 0x200000;
            ARAM_size += 0x200000;
        } else {
            memset(buffer, 0, 0x20);
            DCFlushRange(buffer, 0x20);
            __ARReadDMA((u32)buffer, ARAM_size + 0x01000000, 0x20U);
            PPCSync();
            if (*buffer == *test_data) {
                ARAM_mode |= 8;
                __AR_ExpansionSize = 0x400000;
                ARAM_size += 0x400000;
            } else {
                memset(buffer, 0, 0x20);
                DCFlushRange(buffer, 0x20);
                __ARReadDMA((u32)buffer, ARAM_size + 0x200, 0x20U);
                PPCSync();
                if (*buffer == *test_data) {
                    ARAM_mode |= 0x10;
                    __AR_ExpansionSize = 0x800000;
                    ARAM_size += 0x800000;
                } else {
                    memset(buffer, 0, 0x20);
                    DCFlushRange(buffer, 0x20);
                    __ARReadDMA((u32)buffer, ARAM_size + 0x400000, 0x20U);
                    PPCSync();
                    if (*buffer == *test_data) {
                        ARAM_mode |= 0x18;
                        __AR_ExpansionSize = 0x01000000;
                        ARAM_size += 0x01000000;
                    } else {
                        ARAM_mode |= 0x20;
                        __AR_ExpansionSize = 0x02000000;
                        ARAM_size += 0x02000000;
                    }
                }
            }
        }
        __DSPRegs[9] = ((u16)(__DSPRegs[9] & 0xFFFFFFC0) | ARAM_mode);
    }
    *(u32 *)OSPhysicalToUncached(0xD0) = ARAM_size;
    __AR_Size = ARAM_size;
}
