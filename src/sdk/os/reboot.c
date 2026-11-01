/* Reference: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/os/OSReboot.c.
 */
#include "os_context.h"
#include "dvd.h"
#include "os_reset.h"
#include "os_interrupt.h"
#define NULL 0
#define TRUE 1
#define FALSE 0
#define OSRoundUp32B(value) (((u32)(value) + 31) & ~31)
extern void *BOOT_REGION_START;
__asm__(".set BOOT_REGION_START, 0x812FDFF0");
extern void *BOOT_REGION_END;
__asm__(".set BOOT_REGION_END, 0x812FDFEC");
extern void __DVDPrepareResetAsync(DVDCBCallback);
extern void ICInvalidateRange(void *, u32);
extern void Run(void (*)(void));
static void *SaveStart = NULL;
static void *SaveEnd = NULL;
static volatile s32 Prepared;

extern u8 OS_REBOOT_BOOL;
__asm__(".set OS_REBOOT_BOOL, 0x800030E2");
extern u32 UNK_817FFFF8;
__asm__(".set UNK_817FFFF8, 0x817FFFF8");
extern u32 UNK_817FFFFC;
__asm__(".set UNK_817FFFFC, 0x817FFFFC");

#define OS_BOOTROM_ADDR ((void *)0x81300000)

typedef struct _ApploaderHeader {
    char date[16];
    u32 entry;
    u32 size;
    u32 rebootSize;
    u32 reserved2;
} ApploaderHeader;

static ApploaderHeader Header __attribute__((aligned(32)));

static inline void ReadApploader(DVDCommandBlock *dvdCmd, void *addr, u32 offset,
                                 u32 numBytes)
{

    while (Prepared == FALSE) {
    }
    DVDReadAbsAsyncForBS(dvdCmd, addr, numBytes, offset + 0x2440, NULL);

    while (TRUE) {
        switch (dvdCmd->state) {
        case DVD_STATE_END:
            break;
        case DVD_STATE_BUSY:
        default:
            continue;
        case DVD_STATE_FATAL_ERROR:
        case DVD_STATE_WAITING:
        case DVD_STATE_COVER_CLOSED:
        case DVD_STATE_NO_DISK:
        case DVD_STATE_COVER_OPEN:
        case DVD_STATE_WRONG_DISK:
        case DVD_STATE_MOTOR_STOPPED:
        case DVD_STATE_PAUSING:
        case DVD_STATE_IGNORED:
        case DVD_STATE_CANCELED:
        case DVD_STATE_RETRY:
            __OSDoHotReset(UNK_817FFFFC);
            continue;
        }
        break;
    }
}

static void Callback(s32 result, struct DVDCommandBlock *block)
{
    Prepared = TRUE;
}

#pragma peephole off
void __OSReboot(u32 resetCode, u32 bootDol)
{
    OSContext exceptionContext;
    DVDCommandBlock dvdCmd;
    DVDCommandBlock dvdCmd2;
    u32 numBytes;
    u32 offset;

    OSDisableInterrupts();

    UNK_817FFFFC = 0;
    UNK_817FFFF8 = 0;
    OS_REBOOT_BOOL = TRUE;
    BOOT_REGION_START = SaveStart;
    BOOT_REGION_END = SaveEnd;
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    DVDInit();
    DVDSetAutoInvalidation(TRUE);

    __DVDPrepareResetAsync(Callback);

    if (!DVDCheckDisk()) {
        __OSDoHotReset(UNK_817FFFFC);
    }

    __OSMaskInterrupts(OS_INTERRUPTMASK_MEM | OS_INTERRUPTMASK_DSP | OS_INTERRUPTMASK_AI |
                       OS_INTERRUPTMASK_EXI | OS_INTERRUPTMASK_PI);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_DI);

    OSEnableInterrupts();

    offset = 0;
    numBytes = 32;
    ReadApploader(&dvdCmd, (void *)&Header, offset, numBytes);

    offset = Header.size + sizeof(ApploaderHeader);
    numBytes = OSRoundUp32B(Header.rebootSize);
    ReadApploader(&dvdCmd2, OS_BOOTROM_ADDR, offset, numBytes);

    ICInvalidateRange(OS_BOOTROM_ADDR, numBytes);
    Run(OS_BOOTROM_ADDR);
}
