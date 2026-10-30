#include "types.h"
#include "asm_ops.h"
#include "metrotrk.h"

#define SUPPORT_TRAP 0x0FE00000

typedef struct TRKExtended1State {
    u32 sr[16];
    u32 tbl;
    u32 tbu;
    u32 hid0;
    u32 hid1;
    u32 msr;
    u32 pvr;
    u32 ibat0u;
    u32 ibat0l;
    u32 ibat1u;
    u32 ibat1l;
    u32 ibat2u;
    u32 ibat2l;
    u32 ibat3u;
    u32 ibat3l;
    u32 dbat0u;
    u32 dbat0l;
    u32 dbat1u;
    u32 dbat1l;
    u32 dbat2u;
    u32 dbat2l;
    u32 dbat3u;
    u32 dbat3l;
    u32 dmiss;
    u32 dcmp;
    u32 hash1;
    u32 hash2;
    u32 imiss;
    u32 icmp;
    u32 rpa;
    u32 sdr1;
    u32 dar;
    u32 dsisr;
    u32 sprg0;
    u32 sprg1;
    u32 sprg2;
    u32 sprg3;
    u32 dec;
    u32 iabr;
    u32 ear;
    u32 dabr;
    u32 pmc1;
    u32 pmc2;
    u32 pmc3;
    u32 pmc4;
    u32 sia;
    u32 mmcr0;
    u32 mmcr1;
    u32 thrm1;
    u32 thrm2;
    u32 thrm3;
    u32 ictc;
    u32 l2cr;
    u32 ummcr2;
    u32 ubamr;
    u32 ummcr0;
    u32 upmc1;
    u32 upmc2;
    u32 usia;
    u32 ummcr1;
    u32 upmc3;
    u32 upmc4;
    u32 usda;
    u32 mmcr2;
    u32 bamr;
    u32 sda;
    u32 msscr0;
    u32 msscr1;
    u32 pir;
    u32 exceptionID;
    u32 gqr[8];
    u32 hid2;
    u32 wpar;
    u32 dma_u;
    u32 dma_l;
} TRKExtended1State;

typedef char TRKExtended1StateSizeCheck[sizeof(TRKExtended1State) == 0x184 ? 1 : -1];

typedef struct TRKCPUState {
    u32 gpr[32];
    u32 pc;
    u32 lr;
    u32 cr;
    u32 ctr;
    u32 xer;
    u8 reserved_94[4];
    u64 fpr[32];
    u64 fpscr;
    u64 fpecr;
    TRKExtended1State extended1;
    u32 psr[32][2];
    u32 transport_handler_saved_ra;
} TRKCPUState;

typedef char TRKCPUStateSizeCheck[sizeof(TRKCPUState) == 0x430 ? 1 : -1];

typedef struct TRKState {
    u32 gpr[32];
    u32 lr;
    u32 ctr;
    u32 xer;
    u32 msr;
    u32 dar;
    u32 dsisr;
    u32 stopped;
    s32 inputActivated;
    void *inputPendingPtr;
} TRKState;

typedef char TRKStateSizeCheck[sizeof(TRKState) == 0xA4 ? 1 : -1];

extern TRKCPUState gTRKCPUState;
extern TRKState gTRKState;

s32 TRKTargetVersions(DSVersions *versions)
{
    versions->kernelMajor = 0;
    versions->kernelMinor = 5;
    versions->protocolMajor = 1;
    versions->protocolMinor = 9;
    return 0;
}

u32 TRKTargetGetPC(void)
{
    return gTRKCPUState.pc;
}

s32 TRKTargetStopped(void)
{
    return gTRKState.stopped;
}

void TRKTargetSetStopped(u32 stopped)
{
    gTRKState.stopped = stopped;
}

void TRKTargetSetInputPendingPtr(void *pointer)
{
    gTRKState.inputPendingPtr = pointer;
}

typedef struct TRKStepStatus {
    s32 active;
    u8 type;
    u32 count;
    u32 rangeStart;
    u32 rangeEnd;
} TRKStepStatus;

extern TRKStepStatus gTRKStepStatus;

s32 TRKTargetEnableTrace(s32 enable)
{
    if (enable)
        gTRKCPUState.extended1.msr |= MSR_SE;
    else
        gTRKCPUState.extended1.msr &= ~MSR_SE;
    return 0;
}

s32 TRKTargetStepDone(void)
{
    s32 done = 1;
    if (gTRKStepStatus.active && (u16)gTRKCPUState.extended1.exceptionID == PPC_TRACE) {
        switch (gTRKStepStatus.type) {
        case kDSStepIntoCount:
            if (gTRKStepStatus.count != 0)
                done = 0;
            break;
        case kDSStepIntoRange:
            if (gTRKCPUState.pc >= gTRKStepStatus.rangeStart &&
                gTRKCPUState.pc <= gTRKStepStatus.rangeEnd)
                done = 0;
            break;
        }
    }
    return done;
}

s32 TRKTargetDoStep(void)
{
    gTRKStepStatus.active = 1;
    TRKTargetEnableTrace(1);
    if (gTRKStepStatus.type == kDSStepIntoCount || gTRKStepStatus.type == kDSStepOverCount)
        gTRKStepStatus.count--;
    TRKTargetSetStopped(0);
    return 0;
}

s32 TRKTargetCheckStep(void)
{
    if (gTRKStepStatus.active) {
        TRKTargetEnableTrace(0);
        if (TRKTargetStepDone())
            gTRKStepStatus.active = 0;
        else
            TRKTargetDoStep();
    }
    return gTRKStepStatus.active;
}

s32 TRKTargetSingleStep(u32 count, s32 stepOver)
{
    if (stepOver)
        return kUnsupportedError;
    gTRKStepStatus.type = kDSStepIntoCount;
    gTRKStepStatus.count = count;
    return TRKTargetDoStep();
}

s32 TRKTargetStepOutOfRange(u32 start, u32 end, s32 stepOver)
{
    if (stepOver)
        return kUnsupportedError;
    gTRKStepStatus.type = kDSStepIntoRange;
    gTRKStepStatus.rangeStart = start;
    gTRKStepStatus.rangeEnd = end;
    return TRKTargetDoStep();
}

s32 TRKTargetCPUType(DSCPUType *type)
{
    type->cpuMajor = DS_CPU_MAJOR_PPC;
    type->cpuMinor = TRKTargetCPUMinorType();
    type->bigEndian = (u8)gTRKBigEndian;
    type->defaultTypeSize = 4;
    type->floatTypeSize = 8;
    type->extended1TypeSize = 4;
    type->extended2TypeSize = 8;
    return 0;
}

s32 TRKTargetSupportMask(u8 mask[32])
{
    mask[0] = 0x7A;
    mask[1] = 0x00;
    mask[2] = 0x4F;
    mask[3] = 0x07;
    mask[4] = 0x00;
    mask[5] = 0x00;
    mask[6] = 0x00;
    mask[7] = 0x00;
    mask[8] = 0x00;
    mask[9] = 0x00;
    mask[10] = 0x00;
    mask[11] = 0x00;
    mask[12] = 0x00;
    mask[13] = 0x00;
    mask[14] = 0x00;
    mask[15] = 0x00;
    mask[16] = 0x01;
    mask[17] = 0x00;
    mask[18] = 0x03;
    mask[19] = 0x00;
    mask[20] = 0x00;
    mask[21] = 0x00;
    mask[22] = 0x00;
    mask[23] = 0x00;
    mask[24] = 0x00;
    mask[25] = 0x00;
    mask[26] = 0x03;
    mask[27] = 0x00;
    mask[28] = 0x00;
    mask[29] = 0x00;
    mask[30] = 0x00;
    mask[31] = 0x80;
    return 0;
}

s32 TRKTargetStop(void)
{
    TRKTargetSetStopped(1);
    return 0;
}

s32 TRKInitializeTarget(void)
{
    gTRKState.stopped = 1;
    gTRKState.msr = __TRK_get_MSR();
    return 0;
}

typedef struct TRKMemoryRegion {
    u32 start;
    u32 end;
    s32 readable;
    s32 writable;
} TRKMemoryRegion;

extern TRKMemoryRegion gTRKMemMap[1];

typedef enum TRKValidMemoryOptions { TRK_MEMORY_READ, TRK_MEMORY_WRITE } TRKValidMemoryOptions;

s32 TRKValidMemory32(u32 address, u32 length, TRKValidMemoryOptions mode)
{
    s32 error = kInvalidMemory;
    s32 index;
    u32 end = address;
    end += length - 1;
    if (end < address)
        return error;
    for (index = 0; index < 1; index++) {
        if (address <= gTRKMemMap[index].end && end >= gTRKMemMap[index].start) {
            if (((u8)mode == TRK_MEMORY_READ && !gTRKMemMap[index].readable) ||
                ((u8)mode == TRK_MEMORY_WRITE && !gTRKMemMap[index].writable)) {
                error = kInvalidMemory;
                break;
            }
            error = 0;
            if (address < gTRKMemMap[index].start)
                error = TRKValidMemory32(address, gTRKMemMap[index].start - address, mode);
            if (error == 0 && end > gTRKMemMap[index].end)
                error = TRKValidMemory32(gTRKMemMap[index].end, end - gTRKMemMap[index].end, mode);
            break;
        }
    }
    return error;
}

typedef struct TRKExceptionStatus {
    u32 pc;
    u32 instruction;
    u16 exceptionID;
    u8 reserved0A[2];
    u8 inTRK;
    u8 exceptionDetected;
    u8 reserved0E[2];
} TRKExceptionStatus;

extern TRKExceptionStatus gTRKExceptionStatus;

s32 TRKTargetAccessMemory(void *data, u32 address, u32 *length, s32 accessMode, s32 read)
{
    TRKExceptionStatus saved = gTRKExceptionStatus;
    s32 error;
    u32 translated;
    u32 msr;
    u32 targetMSR;
    gTRKExceptionStatus.exceptionDetected = 0;
    translated = TRKTargetTranslate(address);
    error = TRKValidMemory32(translated, *length, read ? TRK_MEMORY_READ : TRK_MEMORY_WRITE);
    if (error != 0) {
        *length = 0;
    } else {
        msr = __TRK_get_MSR();
        targetMSR = msr | (gTRKCPUState.extended1.msr & MSR_DR);
        if (read) {
            TRK_ppc_memcpy(data, (void *)translated, *length, msr, targetMSR);
        } else {
            TRK_ppc_memcpy((void *)translated, data, *length, targetMSR, msr);
            TRK_flush_cache(translated, *length);
            if (address != translated)
                TRK_flush_cache(address, *length);
        }
    }
    if (gTRKExceptionStatus.exceptionDetected) {
        *length = 0;
        error = kCWDSException;
    }
    gTRKExceptionStatus = saved;
    return error;
}

s32 TRKTargetReadInstruction(u32 *instruction, u32 address)
{
    u32 length = 4;
    s32 error;
    error = TRKTargetAccessMemory(instruction, address, &length, kUserMemory, 1);
    if (error == 0 && length != 4)
        error = kInvalidMemory;
    return error;
}

s32 TRKTargetAddStopInfo(TRKBuffer *buffer)
{
    s32 error;
    u32 instruction;
    error = TRKAppendBuffer1_ui32(buffer, gTRKCPUState.pc);
    if (error == 0)
        error = TRKTargetReadInstruction(&instruction, gTRKCPUState.pc);
    if (error == 0)
        error = TRKAppendBuffer1_ui32(buffer, instruction);
    if (error == 0)
        error = TRKAppendBuffer1_ui16(buffer, (u16)gTRKCPUState.extended1.exceptionID);
    return error;
}

s32 TRKTargetAddExceptionInfo(TRKBuffer *buffer)
{
    s32 error;
    u32 instruction;
    error = TRKAppendBuffer1_ui32(buffer, gTRKExceptionStatus.pc);
    if (error == 0)
        error = TRKTargetReadInstruction(&instruction, gTRKExceptionStatus.pc);
    if (error == 0)
        error = TRKAppendBuffer1_ui32(buffer, instruction);
    if (error == 0)
        error = TRKAppendBuffer1_ui16(buffer, gTRKExceptionStatus.exceptionID);
    return error;
}

s32 TRKTargetSupportRequest(void)
{
    s32 error;
    u32 *length;
    u8 command = gTRKCPUState.gpr[3];
    TRKEvent event;
    u8 status;
    if (command != kDSReadFile && command != kDSWriteFile) {
        TRKConstructEvent(&event, kExceptionEvent);
        TRKPostEvent(&event);
        return 0;
    }
    length = (u32 *)gTRKCPUState.gpr[5];
    error = TRKSuppAccessFile((u8)gTRKCPUState.gpr[4],
                              (u8 *)gTRKCPUState.gpr[6],
                              length, &status, 1, command == kDSReadFile);
    if (status == kDSIONoError && error != 0)
        status = kDSIOError;
    gTRKCPUState.gpr[3] = status;
    if (command == kDSReadFile)
        TRK_flush_cache(gTRKCPUState.gpr[6], *length);
    gTRKCPUState.pc += 4;
    return error;
}

s32 TRKTargetFlushCache(u8 mode, u32 start, u32 end)
{
    s32 error;
    if (start < end) {
        TRK_flush_cache(start, end - start);
        error = 0;
    } else {
        error = kInvalidMemory;
    }
    return error;
}

/* Register-transfer flow: https://github.com/doldecomp/melee/blob/master/src/MetroTRK/targimpl.c */
s32 TRKTargetAccessDefault(u32 first, u32 last, TRKBuffer *buffer, u32 *storageSize, s32 read)
{
    s32 error;
    u32 count;
    u32 *registers;
    TRKExceptionStatus saved;
    if (last > TRK_DEFAULT_XER)
        return kInvalidRegister;
    saved = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;
    registers = (u32 *)&gTRKCPUState + first;
    count = last - first + 1;
    *storageSize = count * sizeof(u32);
    if (read)
        error = TRKAppendBuffer_ui32(buffer, registers, count);
    else
        error = TRKReadBuffer_ui32(buffer, registers, count);
    if (gTRKExceptionStatus.exceptionDetected) {
        *storageSize = 0;
        error = kCWDSException;
    }
    gTRKExceptionStatus = saved;
    return error;
}

extern u8 gTRKRestoreFlags[9];

s32 TRKTargetAccessFP(u32 first, u32 last, TRKBuffer *buffer, u32 *storageSize, s32 read)
{
    s32 error;
    u32 index;
    u64 value;
    TRKExceptionStatus saved;
    if (last > TRK_FLOAT_FPECR)
        return kInvalidRegister;
    saved = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;
    __TRK_set_MSR(__TRK_get_MSR() | MSR_FP);
    *storageSize = 0;
    error = 0;
    for (index = first; index <= last && error == 0; index++) {
        if (read) {
            TRKPPCAccessFPRegister(&value, index, read);
            error = TRKAppendBuffer1_ui64(buffer, value);
        } else {
            /* The original write path accesses the register after a short buffer read. */
            TRKReadBuffer1_ui64(buffer, &value);
            error = TRKPPCAccessFPRegister(&value, index, read);
        }
        *storageSize += 8;
    }
    if (gTRKExceptionStatus.exceptionDetected) {
        *storageSize = 0;
        error = kCWDSException;
    }
    gTRKExceptionStatus = saved;
    return error;
}

s32 TRKTargetAccessExtended1(u32 first, u32 last, TRKBuffer *buffer, u32 *storageSize, s32 read)
{
    TRKExceptionStatus saved;
    /* The original reversed-range path leaves its status uninitialized. */
    s32 error;
    u32 *registers;
    s32 count;
    if (last > sizeof(TRKExtended1State) / sizeof(u32) - 1)
        return kInvalidRegister;
    saved = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;
    *storageSize = 0;
    if (first <= last) {
        registers = (u32 *)&gTRKCPUState.extended1 + first;
        count = last - first + 1;
        *storageSize += count * sizeof(u32);
        if (read) {
            error = TRKAppendBuffer_ui32(buffer, registers, count);
        } else {
            if (registers <= &gTRKCPUState.extended1.tbu && registers + count - 1 >= &gTRKCPUState.extended1.tbl)
                gTRKRestoreFlags[0] = 1;
            if (registers <= &gTRKCPUState.extended1.dec && registers + count - 1 >= &gTRKCPUState.extended1.dec)
                gTRKRestoreFlags[1] = 1;
            error = TRKReadBuffer_ui32(buffer, registers, count);
        }
    }
    if (gTRKExceptionStatus.exceptionDetected) {
        *storageSize = 0;
        error = kCWDSException;
    }
    gTRKExceptionStatus = saved;
    return error;
}

s32 TRKTargetAccessExtended2(u32 first, u32 last, TRKBuffer *buffer, u32 *storageSize, s32 read)
{
    s32 error;
    u32 index;
    u32 value[2];
    TRKExceptionStatus saved;
    u32 control;
    if (last > 31)
        return kInvalidRegister;
    saved = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;
    TRKPPCAccessSPR(&control, SPR_HID2, 1);
    control |= HID2_LSQE | HID2_PSE;
    TRKPPCAccessSPR(&control, SPR_HID2, 0);
    control = 0;
    TRKPPCAccessSPR(&control, SPR_GQR0, 0);
    *storageSize = 0;
    error = 0;
    for (index = first; index <= last && error == 0; index++) {
        if (read) {
            TRKPPCAccessPairedSingleRegister((u64 *)value, index, read);
            error = TRKAppendBuffer1_ui64(buffer, *(u64 *)value);
        } else {
            TRKReadBuffer1_ui64(buffer, (u64 *)value);
            error = TRKPPCAccessPairedSingleRegister((u64 *)value, index, read);
        }
        *storageSize += 8;
    }
    if (gTRKExceptionStatus.exceptionDetected) {
        *storageSize = 0;
        error = kCWDSException;
    }
    gTRKExceptionStatus = saved;
    return error;
}

s32 TRKTargetInterrupt(TRKEvent *event)
{
    s32 error = 0;
    switch (event->eventType) {
    case kBreakpointEvent:
    case kExceptionEvent:
        if (!TRKTargetCheckStep()) {
            TRKTargetSetStopped(1);
            error = TRKDoNotifyStopped(kDSNotifyStopped);
        }
        break;
    }
    return error;
}

void TRKPostInterruptEvent(void)
{
    TRKEvent event;
    u32 instruction;
    s32 type;
    if (gTRKState.inputActivated) {
        gTRKState.inputActivated = 0;
    } else {
        switch ((u16)gTRKCPUState.extended1.exceptionID) {
        case PPC_PROGRAMERROR:
        case PPC_TRACE:
            /* The original path selects an event even when instruction fetch fails. */
            TRKTargetReadInstruction(&instruction, gTRKCPUState.pc);
            if (instruction == SUPPORT_TRAP)
                type = kSupportEvent;
            else
                type = kBreakpointEvent;
            break;
        default:
            type = kExceptionEvent;
            break;
        }
        TRKConstructEvent(&event, type);
        TRKPostEvent(&event);
    }
}

u32 __TRK_get_MSR(void)
{
    register u32 msr;

    ASM_MFMSR(msr);
    return msr;
}

void __TRK_set_MSR(register u32 msr)
{
    ASM_MTMSR(msr);
}
