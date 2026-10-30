#include "types.h"
#include "musyx_hardware.h"
typedef struct GROUP_ENTRY {
    u32 nextOffset;
    u16 id;
    u16 reserved;
} GROUP_ENTRY;
typedef struct GROUP_LAYER {
    GROUP_ENTRY header;
    u32 count;
} GROUP_LAYER;
extern u32 dataInsertMacro(u16 id, void *data);
extern u32 dataRemoveMacro(u16 id);
extern u32 dataInsertKeymap(u16 id, void *data);
extern u32 dataRemoveKeymap(u16 id);
extern u32 dataInsertLayer(u16 id, void *data, u16 count);
extern u32 dataRemoveLayer(u16 id);
extern u32 dataInsertCurve(u16 id, void *data);
extern u32 dataRemoveCurve(u16 id);
extern u32 dataAddSampleReference(u16 id);
extern u32 dataRemoveSampleReference(u16 id);
static inline GROUP_ENTRY *FindGroupData(void *group, u16 id, u32 offset)
{
    GROUP_ENTRY *entry;
    if (!group)
        return 0;
    for (entry = (GROUP_ENTRY *)((u8 *)group + *(u32 *)((u8 *)group + offset));
         entry->nextOffset != 0xFFFFFFFF;
         entry = (GROUP_ENTRY *)((u32)entry + *(u32 *)entry)) {
        if (entry->id == id)
            return entry;
    }
    return 0;
}
#pragma force_active on
static void InsertData(u16 id, void *group, u8 type, u32 remove)
{
    GROUP_ENTRY *entry;
    switch (type) {
    case 0:
        if (!remove) {
            entry = FindGroupData(group, id, 0);
            if (entry)
                dataInsertMacro(id, entry + 1);
            else
                dataInsertMacro(id, 0);
        } else {
            dataRemoveMacro(id);
        }
        break;
    case 2:
        id |= 0x4000;
        if (!remove) {
            entry = FindGroupData(group, id, 8);
            if (entry)
                dataInsertKeymap(id, entry + 1);
            else
                dataInsertKeymap(id, 0);
        } else {
            dataRemoveKeymap(id);
        }
        break;
    case 3:
        id |= 0x8000;
        if (!remove) {
            entry = FindGroupData(group, id, 12);
            if (entry) {
                GROUP_LAYER *layer = (GROUP_LAYER *)entry;
                dataInsertLayer(id, layer + 1, layer->count);
            } else {
                dataInsertLayer(id, 0, 0);
            }
        } else {
            dataRemoveLayer(id);
        }
        break;
    case 4:
        if (!remove) {
            entry = FindGroupData(group, id, 4);
            if (entry)
                dataInsertCurve(id, entry + 1);
            else
                dataInsertCurve(id, 0);
        } else {
            dataRemoveCurve(id);
        }
        break;
    case 1:
        if (!remove)
            dataAddSampleReference(id);
        else
            dataRemoveSampleReference(id);
        break;
    }
}
static void ScanIDList(u16 *list, void *group, u8 type, u32 remove);
static void ScanIDListReverse(u16 *list, void *group, u8 type, u32 remove);
#pragma force_active reset

typedef struct GROUP_HEADER {
    u32 nextOffset;
    u16 id;
    u16 type;
    u32 macros;
    u32 samples;
    u32 curves;
    u32 keymaps;
    u32 layers;
    u32 normalPage;
    u32 drumPage;
    u32 songs;
} GROUP_HEADER;
typedef struct SAMPLE_DATA SAMPLE_DATA;
typedef struct FX_DATA FX_DATA;
typedef struct GROUP_STACK {
    GROUP_HEADER *header;
    SAMPLE_DATA *sampleDirectory;
    void *base;
} GROUP_STACK;
typedef struct GROUP_FX {
    u16 count;
    u16 reserved;
} GROUP_FX;
static GROUP_STACK gs[128];
static s16 sp;

void dataInitStack(void)
{
    sp = 0;
}

extern u8 sndActive;
extern void *hwTransAddr(void *samples);
extern u32 dataInsertSDir(SAMPLE_DATA *directory, u32 sourceBase);
extern u32 dataRemoveSDir(SAMPLE_DATA *directory);
extern u32 dataInsertFX(u16 group, FX_DATA *entries, u16 count);
extern u32 dataRemoveFX(u16 group);
extern void hwSyncSampleMem(void);
extern void s3dKillEmitterByFXID(FX_DATA *entries, u16 count);
extern void seqKillInstancesByGroupID(u16 group);
extern void synthKillVoicesByMacroReferences(u16 *list);
/* Control-flow reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/s_data.c (CC0-1.0). */
static inline void InsertSamples(u16 *list, void *samples, SAMPLE_DATA *directory)
{
    samples = hwTransAddr(samples);
    if (dataInsertSDir(directory, (u32)samples))
        ScanIDList(list, directory, 1, 0);
}
u32 sndPushGroup(void *base, u16 group, void *samples, SAMPLE_DATA *directory, void *resources)
{
    GROUP_HEADER *header;
    GROUP_FX *fx;
    if (sndActive && sp < 128) {
        for (header = base; header->nextOffset != 0xFFFFFFFF;
             header = (GROUP_HEADER *)((u8 *)base + *(u32 *)header)) {
            if (header->id == group) {
                gs[sp].header = header;
                gs[sp].base = base;
                gs[sp].sampleDirectory = directory;
                InsertSamples((u16 *)((u8 *)base + header->samples), samples, directory);
                ScanIDList((u16 *)((u8 *)base + header->macros), resources, 0, 0);
                ScanIDList((u16 *)((u8 *)base + header->curves), resources, 4, 0);
                ScanIDList((u16 *)((u8 *)base + header->keymaps), resources, 2, 0);
                ScanIDList((u16 *)((u8 *)base + header->layers), resources, 3, 0);
                if (header->type == 1) {
                    fx = (GROUP_FX *)((u8 *)base + header->normalPage);
                    dataInsertFX(group, (FX_DATA *)(fx + 1), fx->count);
                }
                hwSyncSampleMem();
                sp++;
                return 1;
            }
        }
    }
    return 0;
}
u32 sndPopGroup(void)
{
    GROUP_HEADER *header;
    SAMPLE_DATA *directory;
    void *base;
    GROUP_FX *fx;
    header = gs[--sp].header;
    base = gs[sp].base;
    directory = gs[sp].sampleDirectory;
    hwDisableIrq();
    if (header->type == 1) {
        fx = (GROUP_FX *)((u8 *)base + header->normalPage);
        s3dKillEmitterByFXID((FX_DATA *)(fx + 1), fx->count);
    } else {
        seqKillInstancesByGroupID(header->id);
    }
    synthKillVoicesByMacroReferences((u16 *)((u8 *)base + header->macros));
    hwEnableIrq();
    ScanIDListReverse((u16 *)((u8 *)base + header->samples), 0, 1, 1);
    dataRemoveSDir(directory);
    ScanIDList((u16 *)((u8 *)base + header->macros), 0, 0, 1);
    ScanIDList((u16 *)((u8 *)base + header->curves), 0, 4, 1);
    ScanIDList((u16 *)((u8 *)base + header->keymaps), 0, 2, 1);
    ScanIDList((u16 *)((u8 *)base + header->layers), 0, 3, 1);
    if (header->type == 1)
        dataRemoveFX(header->id);
    return 1;
}
#pragma force_active on
static void ScanIDList(u16 *list, void *group, u8 type, u32 remove)
{
    u16 id;
    while (*list != 0xFFFF) {
        if (*list & 0x8000) {
            for (id = *list & 0x3FFF; id <= list[1]; id++)
                InsertData(id, group, type, remove);
            list += 2;
        } else {
            InsertData(*list++, group, type, remove);
        }
    }
}
static void ScanIDListReverse(u16 *list, void *group, u8 type, u32 remove)
{
    s16 id;
    u16 *current;
    if (*list != 0xFFFF) {
        current = list;
        while (*current != 0xFFFF)
            current++;
        current--;
        while (current >= list) {
            if (current != list) {
                if (current[-1] & 0x8000) {
                    for (id = *current; id >= (s16)(current[-1] & 0x3FFF); id--)
                        InsertData(id, group, type, remove);
                    current -= 2;
                } else {
                    InsertData(*current, group, type, remove);
                    current--;
                }
            } else {
                InsertData(*current, group, type, remove);
                current--;
            }
        }
    }
}
#pragma force_active reset


typedef struct PAGE PAGE;
typedef struct SND_PLAYPARA SND_PLAYPARA;
typedef struct MIDI_CHANNEL_SETUP {
    u8 program, volume, panning, reverb, chorus;
} MIDI_CHANNEL_SETUP;
typedef struct MIDISETUP {
    u16 songId, reserved;
    MIDI_CHANNEL_SETUP channel[16];
} MIDISETUP;
extern u32 seqStartPlay(PAGE *normal, PAGE *drum, MIDISETUP *midi, u32 *arrangement,
                       SND_PLAYPARA *parameters, u8 studio, u16 group);

u32 seqPlaySong(u16 sgid, u16 sid, void *arrfile, SND_PLAYPARA *para, u8 irq_call, u8 studio)
{
    int i;
    GROUP_HEADER *g;
    PAGE *norm;
    PAGE *drum;
    MIDISETUP *midiSetup;
    u32 seqId;
    void *prj;
    for (i = 0; i < sp; ++i) {
        if (gs[i].header->id != sgid)
            continue;
        if (gs[i].header->type == 0) {
            g = gs[i].header;
            prj = gs[i].base;
            norm = (PAGE *)((u32)prj + g->normalPage);
            drum = (PAGE *)((u32)prj + g->drumPage);
            midiSetup = (MIDISETUP *)((u32)prj + g->songs);
            while (midiSetup->songId != 0xFFFF) {
                if (midiSetup->songId == sid) {
                    if (irq_call != 0) {
                        seqId = seqStartPlay(norm, drum, midiSetup, arrfile, para, studio, sgid);
                    } else {
                        hwDisableIrq();
                        seqId = seqStartPlay(norm, drum, midiSetup, arrfile, para, studio, sgid);
                        hwEnableIrq();
                    }
                    return seqId;
                }
                ++midiSetup;
            }
            return 0xffffffff;
        } else {
            return 0xffffffff;
        }
    }
    return 0xffffffff;
}
