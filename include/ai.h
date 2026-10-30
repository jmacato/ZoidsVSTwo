#ifndef ZOIDS_AI_H
#define ZOIDS_AI_H
#include "types.h"
typedef void (*AISCallback)(u32);
typedef void (*AIDCallback)(void);
#define AI_STREAM_START 1
#define AI_STREAM_STOP 0
#define AI_SAMPLERATE_32KHZ 0
#define AI_SAMPLERATE_48KHZ 1
AIDCallback AIRegisterDMACallback(AIDCallback callback);
void AIInitDMA(u32 start_addr, u32 length);
void AIStartDMA(void);
void AIStopDMA(void);
void AISetStreamPlayState(u32 state);
u32 AIGetStreamPlayState(void);
void AISetDSPSampleRate(u32 rate);
u32 AIGetDSPSampleRate(void);
u32 AIGetStreamSampleRate(void);
void AISetStreamVolLeft(u8 vol);
u8 AIGetStreamVolLeft(void);
void AISetStreamVolRight(u8 vol);
u8 AIGetStreamVolRight(void);
void AIInit(u8 *stack);
#endif
