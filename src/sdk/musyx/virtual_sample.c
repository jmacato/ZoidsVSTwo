#include "musyx_voice.h"
#include "musyx_hardware.h"
typedef struct VIRTUAL_SAMPLE_INFO {
    u16 sampleID;
    u16 generation;
    u32 firstOffset;
    u32 firstLength;
    u32 secondOffset;
    u32 secondLength;
} VIRTUAL_SAMPLE_INFO;
typedef struct VIRTUAL_SAMPLE {
    u8 state;
    u8 hwId;
    u8 sampleType;
    u8 voice;
    u32 writePosition;
    u32 remaining;
    u32 lastPosition;
    VIRTUAL_SAMPLE_INFO info;
} VIRTUAL_SAMPLE;
typedef struct VIRTUAL_SAMPLES {
    u8 count;
    u8 reserved01[3];
    u32 length;
    VIRTUAL_SAMPLE entries[64];
    u8 voiceMap[64];
    u16 generation;
    u16 reserved94A;
    u32 (*callback)(u32 reason, VIRTUAL_SAMPLE_INFO *info);
} VIRTUAL_SAMPLES;
static VIRTUAL_SAMPLES vs;
void vsInit(void)
{
    u32 index;
    vs.count = 0;
    for (index = 0; index < 64; index++)
        vs.voiceMap[index] = 0xFF;
    vs.generation = 0;
    vs.callback = 0;
}

void vsSampleEndNotify(u32 id)
{
    u8 index;
    if (id != 0xFFFFFFFF) {
        index = vs.voiceMap[(u8)id];
        if (index != 0xFF && vs.entries[index].info.generation == (u16)(id >> 8)) {
            if (vs.callback)
                vs.callback(2, &vs.entries[index].info);
            vs.entries[index].state = 0;
            vs.voiceMap[vs.entries[index].voice] = 0xFF;
        }
    }
}

extern SYNTH_VOICE *synthVoice;
extern u32 hwGetVirtualSampleState(u32 voice);
extern u32 hwGetPos(u32 voice);
extern u32 hwGetVirtualSampleID(u32 voice);
extern u32 hwVoiceInStartup(u32 voice);
static void vsUpdateBuffer(VIRTUAL_SAMPLE *sample, u32 position);
void vsSampleUpdates(void)
{
    u32 voice;
    u32 position;
    VIRTUAL_SAMPLE *sample;
    u32 aligned;
    s32 limit;
    if (vs.callback) {
        for (voice = 0; voice < 64; voice++) {
            if (vs.voiceMap[voice] != 0xFF && hwGetVirtualSampleState(voice)) {
                sample = &vs.entries[vs.voiceMap[voice]];
                position = hwGetPos(voice);
                if (sample->sampleType == 5)
                    aligned = position / 14 * 14;
                else
                    aligned = position;
                switch (sample->state) {
                case 1:
                    vsUpdateBuffer(sample, aligned);
                    break;
                case 2:
                    if (((sample->info.generation << 8) | sample->voice) == hwGetVirtualSampleID(sample->voice)) {
                        vsUpdateBuffer(sample, aligned);
                        if (position >= sample->lastPosition)
                            sample->remaining -= position - sample->lastPosition;
                        else
                            sample->remaining -= vs.length - (sample->lastPosition - position);
                        sample->lastPosition = position;
                        limit = ((s32)(synthVoice[sample->voice].currentPitch * 160) + 4095) / 4096;
                        if (limit > (s32)sample->remaining) {
                            if (!hwVoiceInStartup(sample->voice))
                                hwBreak(sample->voice);
                            sample->state = 0;
                            vs.voiceMap[sample->voice] = 0xFF;
                        }
                    } else {
                        sample->state = 0;
                        vs.voiceMap[sample->voice] = 0xFF;
                    }
                    break;
                }
            }
        }
    }
}

/* Control-flow reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/synth_vsamples.c (CC0-1.0). */
static inline u16 vsNewInstanceID(void)
{
    u8 index;
    u16 generation;
    do {
        generation = vs.generation++;
        for (index = 0; index < vs.count; index++) {
            if (vs.entries[index].state && vs.entries[index].info.generation == generation)
                break;
        }
    } while (index != vs.count);
    return generation;
}
static inline u8 vsAllocateBuffer(void)
{
    u8 index;
    for (index = 0; index < vs.count; index++) {
        if (vs.entries[index].state)
            continue;
        vs.entries[index].state = 1;
        vs.entries[index].writePosition = 0;
        return index;
    }
    return 0xFF;
}
static inline void vsFreeBuffer(u8 index)
{
    vs.entries[index].state = 0;
    vs.voiceMap[vs.entries[index].voice] = 0xFF;
}
extern u32 aramGetStreamBufferAddress(u8 index, u32 *length);
extern void hwSetVirtualSampleLoopBuffer(u32 voice, void *address, u32 length);
extern u16 hwGetSampleID(u32 voice);
extern u8 hwGetSampleType(u32 voice);
u32 vsSampleStartNotify(u8 voice)
{
    u8 slot;
    u8 index;
    unsigned long address;
    for (index = 0; index < vs.count; index++) {
        if (vs.entries[index].state && vs.entries[index].voice == voice)
            vsFreeBuffer(index);
    }
    slot = vs.voiceMap[voice] = vsAllocateBuffer();
    if (slot != 0xFF) {
        address = aramGetStreamBufferAddress(vs.voiceMap[voice], 0);
        hwSetVirtualSampleLoopBuffer(voice, (void *)address, vs.length);
        vs.entries[slot].info.sampleID = hwGetSampleID(voice);
        vs.entries[slot].info.generation = vsNewInstanceID();
        vs.entries[slot].sampleType = hwGetSampleType(voice);
        vs.entries[slot].voice = voice;
        if (vs.callback) {
            vs.callback(0, &vs.entries[slot].info);
            return (vs.entries[slot].info.generation << 8) | voice;
        }
        hwSetVirtualSampleLoopBuffer(voice, 0, 0);
    } else {
        hwSetVirtualSampleLoopBuffer(voice, 0, 0);
    }
    return 0xFFFFFFFF;
}

static void vsUpdateBuffer(VIRTUAL_SAMPLE *sample, u32 position)
{
    u32 done;
    if (sample->writePosition == position)
        return;
    if (sample->writePosition < position) {
        switch (sample->sampleType) {
        case 5: {
            u32 offset = sample->writePosition / 14 * 8;
            sample->info.firstOffset = offset;
            sample->info.firstLength = position - sample->writePosition;
            sample->info.secondOffset = 0;
            sample->info.secondLength = 0;
            if ((done = vs.callback(1, &sample->info)) != 0)
                sample->writePosition = (sample->writePosition + done) % vs.length;
            break;
        }
        }
    } else if (position == 0) {
        switch (sample->sampleType) {
        case 5: {
            u32 offset = sample->writePosition / 14 * 8;
            sample->info.firstOffset = offset;
            sample->info.firstLength = vs.length - sample->writePosition;
            sample->info.secondOffset = 0;
            sample->info.secondLength = 0;
            if ((done = vs.callback(1, &sample->info)) != 0)
                sample->writePosition = (sample->writePosition + done) % vs.length;
            break;
        }
        }
    } else {
        switch (sample->sampleType) {
        case 5: {
            u32 offset = sample->writePosition / 14 * 8;
            sample->info.firstOffset = offset;
            sample->info.firstLength = vs.length - sample->writePosition;
            sample->info.secondOffset = 0;
            sample->info.secondLength = position;
            if ((done = vs.callback(1, &sample->info)) != 0)
                sample->writePosition = (sample->writePosition + done) % vs.length;
            break;
        }
        }
    }
}
