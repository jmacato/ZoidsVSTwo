#ifndef ZOIDS_OS_SRAM_H
#define ZOIDS_OS_SRAM_H
#include "types.h"
typedef struct OSSram {
    unsigned short checkSum;
    unsigned short checkSumInv;
    unsigned long ead0;
    unsigned long ead1;
    unsigned long counterBias;
    signed char displayOffsetH;
    unsigned char ntd;
    unsigned char language;
    unsigned char flags;
} OSSram;

typedef struct OSSramEx {
    unsigned char flashID[2][12];
    unsigned long wirelessKeyboardID;
    unsigned short wirelessPadID[4];
    unsigned char dvdErrorCode;
    unsigned char _padding0;
    unsigned char flashIDCheckSum[2];
    unsigned char _padding1[4];
} OSSramEx;

extern void __OSInitSram(void);
extern OSSram *__OSLockSram(void);
extern OSSramEx *__OSLockSramEx(void);
extern int __OSUnlockSram(int commit);
extern int __OSUnlockSramEx(int commit);
extern int __OSSyncSram(void);
extern int __OSReadROM(void *buffer, s32 length, s32 offset);
extern u32 OSGetSoundMode(void);
extern u16 OSGetWirelessID(s32 channel);
extern void OSSetWirelessID(s32 channel, u16 id);
#endif
