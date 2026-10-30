/* Reference: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/os/OS.c. */
#include "os.h"
#pragma peephole off
#include "os_arena.h"
#include "dvd.h"
#include "db.h"
#include "asm_ops.h"
#include "os_interrupt.h"
#define NULL 0
#define NOP 0x60000000
#define OSPhysicalToCached(address) ((void *)((u32)(address) + 0x80000000))
#define OS_EXCEPTION_MAX 15
#define OS_DBJUMPPOINT_ADDR 0x60
#define OS_EXCEPTIONTABLE_ADDR 0x3000
#define OS_BI2_DEBUG_ADDRESS 0x800000F4
#define DEBUGFLAG_ADDR 0x800030E8
#define OS_DEBUG_ADDRESS_2 0x800030E9
#define DI_CONFIG_IDX 9
#define DI_CONFIG_CONFIG_MASK 0xFF
extern volatile u32 __PIRegs[16] : 0xCC003000;
extern volatile u32 __DIRegs[16] : 0xCC006000;
extern volatile u16 __OSDeviceCode : 0x800030E6;
extern void *BOOT_REGION_START : 0x812FDFF0;
extern void *BOOT_REGION_END : 0x812FDFEC;
typedef OSExceptionHandler __OSExceptionHandler;
extern __OSExceptionHandler __OSSetExceptionHandler(u8, __OSExceptionHandler);
extern __OSExceptionHandler __OSGetExceptionHandler(u8);
extern void PPCDisableSpeculation(void);
extern void PPCSetFpNonIEEEMode(void);
extern u32 PPCMfhid2(void);
extern void PPCMthid2(u32);
extern u32 ICFlashInvalidate(void);
extern s64 __OSGetSystemTime(void);
extern void OSInitAlarm(void);
extern void __OSInitSystemCall(void);
extern void __OSModuleInit(void);
extern void __OSInterruptInit(void);
extern void __OSCacheInit(void);
extern void EXIInit(void);
extern void SIInit(void);
extern void __OSInitSram(void);
extern void __OSThreadInit(void);
extern void __OSInitAudioSystem(void);
extern void __OSInitMemoryProtection(void);
extern void OSReport(const char *, ...);
extern void EnableMetroTRKInterrupts(void);
extern u32 OSGetResetCode(void);
extern void __OSResetSWInterruptHandler(s16, OSContext *);
extern void DCInvalidateRange(void *, u32);
extern void DCFlushRangeNoSync(void *, u32);
extern void ICInvalidateRange(void *, u32);
extern void *memcpy(void *, const void *, u32);
extern void *memset(void *, s32, u32);
extern s32 __DBIsExceptionMarked(u8);
extern void OSDefaultExceptionHandler(u8, OSContext *);
extern void __OSEVStart(void), __OSEVEnd(void), __OSEVSetNumber(void), __DBVECTOR(void);
extern void __OSDBINTSTART(void), __OSDBJUMPSTART(void), __OSDBJUMPEND(void);
extern unsigned long __DVDLongFileNameFlag, __PADSpec;
extern unsigned char __ArenaLo[], __ArenaHi[];
extern char _stack_addr[];
typedef struct OSBootInfo_s {
    DVDDiskID diskId;
    unsigned long magic, version, memorySize, consoleType;
    void *arenaLo, *arenaHi, *FSTLocation;
    unsigned long FSTMaxLength;
} OSBootInfo;
static OSBootInfo *BootInfo = 0;
static volatile u32 *BI2DebugFlag = 0;
static u32 *BI2DebugFlagHolder = 0;
s32 __OSIsGcam = 0;
static s32 AreWeInitialized = 0;
static __OSExceptionHandler *OSExceptionTable = 0;
void *__OSSavedRegionEnd = 0;
void *__OSSavedRegionStart = 0;
s32 __OSInIPL = 0;
s64 __OSStartTime = 0;
static DVDDriveInfo DriveInfo __attribute__((aligned(32)));
static DVDCommandBlock DriveBlock;
typedef struct BI2Debug {
    /* 0x00 */ s32 debugMonSize;
    /* 0x04 */ s32 simMemSize;
    /* 0x08 */ u32 argOffset;
    /* 0x0C */ u32 debugFlag;
    /* 0x10 */ int trackLocation;
    /* 0x14 */ int trackSize;
    /* 0x18 */ u32 countryCode;
    /* 0x1C */ u8 unk[8];
    /* 0x24 */ u32 padSpec;
} BI2Debug;
#define OS_CONSOLE_MASK 0xf0000000
#define OS_CONSOLE_RETAIL 0x00000000
#define OS_CONSOLE_DEVELOPMENT 0x10000000
#define OS_CONSOLE_TDEV 0x20000000
#define OS_CONSOLE_RETAIL4 0x00000004
#define OS_CONSOLE_RETAIL3 0x00000003
#define OS_CONSOLE_RETAIL2 0x00000002
#define OS_CONSOLE_RETAIL1 0x00000001
#define OS_CONSOLE_TDEVHW4 0x20000007
#define OS_CONSOLE_TDEVHW3 0x20000006
#define OS_CONSOLE_TDEVHW2 0x20000005
#define OS_CONSOLE_TDEVHW1 0x20000004
#define OS_CONSOLE_DEVHW4 0x10000007
#define OS_CONSOLE_DEVHW3 0x10000006
#define OS_CONSOLE_DEVHW2 0x10000005
#define OS_CONSOLE_DEVHW1 0x10000004
#define OS_CONSOLE_MINNOW 0x10000003
#define OS_CONSOLE_ARTHUR 0x10000002
#define OS_CONSOLE_PC_EMULATOR 0x10000001
#define OS_CONSOLE_EMULATOR 0x10000000

unsigned long OSGetConsoleType(void);
static void ClearArena(void);
static void InquiryCallback(s32 result, struct DVDCommandBlock *block);
void OSInit(void);
static void OSExceptionInit(void);
__OSExceptionHandler __OSSetExceptionHandler(__OSException exception,
                                             __OSExceptionHandler handler);
__OSExceptionHandler __OSGetExceptionHandler(__OSException exception);
u32 __OSGetDIConfig(void);
unsigned long OSGetConsoleType(void)
{
    if ((!BootInfo) || (BootInfo->consoleType == 0)) {
        return OS_CONSOLE_ARTHUR;
    }
    return BootInfo->consoleType;
}
static void ClearArena(void)
{
    if (OSGetResetCode() != -0x80000000) {
        __OSSavedRegionStart = NULL;
        __OSSavedRegionEnd = NULL;
        memset(OSGetArenaLo(), 0, (u8 *)OSGetArenaHi() - (u8 *)OSGetArenaLo());
        return;
    }

    __OSSavedRegionStart = BOOT_REGION_START;
    __OSSavedRegionEnd = BOOT_REGION_END;

    if (BOOT_REGION_START == NULL) {
        memset(OSGetArenaLo(), 0, (u8 *)OSGetArenaHi() - (u8 *)OSGetArenaLo());
        return;
    }

    if ((u8 *)OSGetArenaLo() < (u8 *)__OSSavedRegionStart) {
        if ((u8 *)OSGetArenaHi() <= (u8 *)__OSSavedRegionStart) {
            memset(OSGetArenaLo(), 0, (u8 *)OSGetArenaHi() - (u8 *)OSGetArenaLo());
            return;
        }

        memset(OSGetArenaLo(), 0, (u8 *)__OSSavedRegionStart - (u8 *)OSGetArenaLo());

        if ((u8 *)OSGetArenaHi() > (u8 *)__OSSavedRegionEnd) {
            memset(__OSSavedRegionEnd, 0,
                   (u8 *)OSGetArenaHi() - (u8 *)__OSSavedRegionEnd);
        }
    }
}
static void InquiryCallback(s32 result, struct DVDCommandBlock *block)
{
    switch (block->state) {
    case DVD_STATE_END:
        __OSDeviceCode = DriveInfo.deviceCode | 0x8000;
        break;
    default:
        __OSDeviceCode = 1;
        break;
    }
}
void OSInit(void)
{
    unsigned long consoleType;
    BI2Debug *bi2DebugInfo;

    if (AreWeInitialized != 0)
        return;

    AreWeInitialized = 1;
    __OSStartTime = __OSGetSystemTime();
    OSDisableInterrupts();
    PPCDisableSpeculation();
    PPCSetFpNonIEEEMode();
    BootInfo = (struct OSBootInfo_s *)OSPhysicalToCached(0);
    BI2DebugFlag = NULL;
    __DVDLongFileNameFlag = 0;
    bi2DebugInfo = (BI2Debug *)*(BI2Debug **)OS_BI2_DEBUG_ADDRESS;
    if (bi2DebugInfo != NULL) {
        BI2DebugFlag = &bi2DebugInfo->debugFlag;
        __PADSpec = bi2DebugInfo->padSpec;
        *((u8 *)DEBUGFLAG_ADDR) = (u8)*BI2DebugFlag;
        *((u8 *)OS_DEBUG_ADDRESS_2) = (u8)__PADSpec;
    } else if (BootInfo->arenaHi) {
        BI2DebugFlagHolder = (u32 *)*((u8 *)DEBUGFLAG_ADDR);
        BI2DebugFlag = (u32 *)&BI2DebugFlagHolder;
        __PADSpec = (u32) * ((u8 *)OS_DEBUG_ADDRESS_2);
    }

    __DVDLongFileNameFlag = 1;

    OSSetArenaLo(!BootInfo->arenaLo ? &__ArenaLo : BootInfo->arenaLo);
    if (!BootInfo->arenaLo && BI2DebugFlag && (*(u32 *)BI2DebugFlag < 2)) {
        OSSetArenaLo((void *)(((u32)(char *)&_stack_addr + 0x1F) & 0xFFFFFFE0));
    }
    OSSetArenaHi((!BootInfo->arenaHi) ? &__ArenaHi : BootInfo->arenaHi);
    OSExceptionInit();
    __OSInitSystemCall();
    OSInitAlarm();
    __OSModuleInit();
    __OSInterruptInit();
    __OSSetInterruptHandler(__OS_INTERRUPT_PI_RSW, &__OSResetSWInterruptHandler);
    __OSContextInit();
    __OSCacheInit();
    EXIInit();
    SIInit();
    __OSInitSram();
    __OSThreadInit();
    __OSInitAudioSystem();
    PPCMthid2(PPCMfhid2() & 0xbfffffff);
    if ((BootInfo->consoleType & OS_CONSOLE_DEVELOPMENT) != 0) {
        BootInfo->consoleType = OS_CONSOLE_DEVHW1;
    } else {
        BootInfo->consoleType = OS_CONSOLE_RETAIL1;
    }
    BootInfo->consoleType += (__PIRegs[11] & 0xF0000000) >> 28;
    if (__OSInIPL == 0) {
        __OSInitMemoryProtection();
    }

    OSReport("\nDolphin OS $Revision: 54 $.\n");
    OSReport("Kernel built : %s %s\n", "Jun  5 2002", "02:09:12");

    OSReport("Console Type : ");

    consoleType = OSGetConsoleType();
    if ((consoleType & OS_CONSOLE_DEVELOPMENT) == OS_CONSOLE_RETAIL) {
        OSReport("Retail %d\n", consoleType);
    } else {
        switch (consoleType) {
        case OS_CONSOLE_EMULATOR:
            OSReport("Mac Emulator\n");
            break;
        case OS_CONSOLE_PC_EMULATOR:
            OSReport("PC Emulator\n");
            break;
        case OS_CONSOLE_ARTHUR:
            OSReport("EPPC Arthur\n");
            break;
        case OS_CONSOLE_MINNOW:
            OSReport("EPPC Minnow\n");
            break;
        default:
            OSReport("Development HW%d\n", ((u32)consoleType - OS_CONSOLE_DEVELOPMENT) - 3);
            break;
        }
    }

    OSReport("Memory %d MB\n", (u32)BootInfo->memorySize >> 0x14U);

    OSReport("Arena : 0x%x - 0x%x\n", OSGetArenaLo(), OSGetArenaHi());

    if (BI2DebugFlag && ((*BI2DebugFlag) >= 2)) {
        EnableMetroTRKInterrupts();
    }
    ClearArena();
    OSEnableInterrupts();
    if (__OSInIPL == 0) {
        DVDInit();

        if (__OSIsGcam) {
            __OSDeviceCode = 0x9000;
            return;
        }

        DCInvalidateRange(&DriveInfo, 0x20);
        DVDInquiryAsync(&DriveBlock, &DriveInfo, InquiryCallback);
    }
}
static u32 __OSExceptionLocations[] = {
    0x00000100, 0x00000200, 0x00000300, 0x00000400, 0x00000500,
    0x00000600, 0x00000700, 0x00000800, 0x00000900, 0x00000C00,
    0x00000D00, 0x00000F00, 0x00001300, 0x00001400, 0x00001700,
};
static void OSExceptionInit(void)
{
    __OSException exception;
    void *destAddr;

    u32 *opCodeAddr;
    u32 oldOpCode;

    u8 *handlerStart;
    u32 handlerSize;

    opCodeAddr = (u32 *)__OSEVSetNumber;
    oldOpCode = *opCodeAddr;
    handlerStart = (u8 *)__OSEVStart;
    handlerSize = (u32)((u8 *)__OSEVEnd - (u8 *)__OSEVStart);

    destAddr = (void *)OSPhysicalToCached(OS_DBJUMPPOINT_ADDR);
    if (*(u32 *)destAddr == 0) {
        DBPrintf("Installing OSDBIntegrator\n");
        memcpy(destAddr, (void *)__OSDBINTSTART,
               (u32)__OSDBJUMPSTART - (u32)__OSDBINTSTART);
        DCFlushRangeNoSync(destAddr, (u32)__OSDBJUMPSTART - (u32)__OSDBINTSTART);
        __sync();
        ICInvalidateRange(destAddr, (u32)__OSDBJUMPSTART - (u32)__OSDBINTSTART);
    }

    for (exception = 0; exception < OS_EXCEPTION_MAX; exception++) {
        if (BI2DebugFlag && (*BI2DebugFlag >= 2) && __DBIsExceptionMarked(exception)) {

            DBPrintf(">>> OSINIT: exception %d commandeered by TRK\n", exception);
            continue;
        }

        *opCodeAddr = oldOpCode | exception;

        if (__DBIsExceptionMarked(exception)) {
            DBPrintf(">>> OSINIT: exception %d vectored to debugger\n", exception);
            memcpy((void *)__DBVECTOR, (void *)__OSDBJUMPSTART,
                   (u32)__OSDBJUMPEND - (u32)__OSDBJUMPSTART);
        } else {

            u32 *ops = (u32 *)__DBVECTOR;
            int cb;

            for (cb = 0; cb < (u32)__OSDBJUMPEND - (u32)__OSDBJUMPSTART;
                 cb += sizeof(u32)) {
                *ops++ = NOP;
            }
        }

        destAddr = (void *)OSPhysicalToCached(__OSExceptionLocations[(u32)exception]);
        memcpy(destAddr, handlerStart, handlerSize);
        DCFlushRangeNoSync(destAddr, handlerSize);
        __sync();
        ICInvalidateRange(destAddr, handlerSize);
    }

    OSExceptionTable = (void *)OSPhysicalToCached(OS_EXCEPTIONTABLE_ADDR);

    for (exception = 0; exception < OS_EXCEPTION_MAX; exception++) {
        __OSSetExceptionHandler(exception, OSDefaultExceptionHandler);
    }

    *opCodeAddr = oldOpCode;

    DBPrintf("Exceptions initialized...\n");
}
__OSExceptionHandler __OSSetExceptionHandler(__OSException exception,
                                             __OSExceptionHandler handler)
{
    __OSExceptionHandler oldHandler;

    oldHandler = OSExceptionTable[exception];
    OSExceptionTable[exception] = handler;
    return oldHandler;
}
__OSExceptionHandler __OSGetExceptionHandler(__OSException exception)
{
    return OSExceptionTable[exception];
}
void __OSPSInit(void)
{
    PPCMthid2(PPCMfhid2() | 0xA0000000);
    ICFlashInvalidate();
    __sync();
    ASM_INSN(li r3, 0);
    ASM_INSN(mtspr 912, r3);
}
u32 __OSGetDIConfig(void)
{
    return (__DIRegs[DI_CONFIG_IDX] & DI_CONFIG_CONFIG_MASK);
}
