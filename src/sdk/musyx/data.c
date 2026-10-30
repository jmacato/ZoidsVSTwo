#include "musyx_sample.h"
#include "musyx_hardware.h"

static u16 dataSmpSDirNum;
static u16 dataCurveNum;
static u16 dataKeymapNum;
static u16 dataLayerNum;
static u16 dataMacTotal;
static u16 dataFXGroupNum;

typedef struct MEM_DATA {
    void *data;
    u16 id;
    u16 references;
} MEM_DATA;

extern void *sndBSearch(const void *key, void *base, s32 count, s32 size,
                        s32 (*compare)(const void *, const void *));
extern void hwExitSampleMem(void);

typedef struct MacroEntry {
    void *data;
    u16 id;
} MacroEntry;

typedef struct SampleEntry {
    u16 id;
} SampleEntry;

typedef struct SAMPLE_DATA {
    u16 id;
    u16 references;
    u32 offset;
    u32 address;
    SAMPLE_INFO info;
} SAMPLE_DATA;
typedef struct SAMPLE_DIRECTORY {
    SAMPLE_DATA *entries;
    u32 base;
    u16 count;
    u16 reserved0A;
} SAMPLE_DIRECTORY;


typedef struct LAYER_DATA {
    void *data;
    u16 id;
    u16 count;
    u16 references;
    u16 reserved0A;
} LAYER_DATA;

#pragma pack(push, 2)
typedef struct FX_DATA {
    u16 id;
    u16 macro;
    u8 maxVoices, priority, volume, panning, key, vGroup;
} FX_DATA;
#pragma pack(pop)
typedef struct FX_GROUP {
    u16 id;
    u16 count;
    FX_DATA *entries;
} FX_GROUP;

typedef struct MACRO_BUCKET {
    u16 count;
    u16 offset;
} MACRO_BUCKET;

static SAMPLE_DIRECTORY dataSmpSDirs[128];
static MEM_DATA dataCurveTab[2048];
static MEM_DATA dataKeymapTab[256];
static LAYER_DATA dataLayerTab[256];
static MACRO_BUCKET dataMacMainTab[512];
static MEM_DATA dataMacSubTabmem[2048];
static FX_GROUP dataFXGroups[128];


s32 maccmp(const void *a, const void *b)
{
    return ((MacroEntry *)a)->id - ((MacroEntry *)b)->id;
}
s32 smpcmp(const void *a, const void *b)
{
    return ((SampleEntry *)a)->id - ((SampleEntry *)b)->id;
}
s32 curvecmp(const void *a, const void *b)
{
    return ((MacroEntry *)a)->id - ((MacroEntry *)b)->id;
}
s32 layercmp(const void *a, const void *b)
{
    return ((MacroEntry *)a)->id - ((MacroEntry *)b)->id;
}
s32 fxcmp(const void *a, const void *b)
{
    return ((SampleEntry *)a)->id - ((SampleEntry *)b)->id;
}
void *dataGetMacro(u16 id)
{
    static u32 base;
    static u32 main;
    static MEM_DATA key;
    static MEM_DATA *result;
    main = (id >> 6) & 0x3FF;
    if (dataMacMainTab[main].count) {
        base = dataMacMainTab[main].offset;
        key.id = id;
        if ((result = sndBSearch(&key, &dataMacSubTabmem[base], dataMacMainTab[main].count,
                                 sizeof(MEM_DATA), maccmp)) != 0)
            return result->data;
    }
    return 0;
}
s32 dataGetSample(u16 id, SAMPLE_RESULT *output)
{
    static SAMPLE_DATA key;
    static SAMPLE_DATA *result;
    static SAMPLE_INFO *sheader;
    s32 index;
    key.id = id;
    for (index = 0; index < dataSmpSDirNum; index++) {
        if ((result = sndBSearch(&key, dataSmpSDirs[index].entries,
                                dataSmpSDirs[index].count, sizeof(SAMPLE_DATA), smpcmp)) != 0) {
            if (result->references != 0xFFFF) {
                sheader = &result->info;
                output->info = sheader->info;
                output->address = result->address;
                output->offset = 0;
                output->loop = sheader->loopOffset;
                output->length = sheader->length & 0xFFFFFF;
                output->loopLength = sheader->loopLength;
                output->compType = sheader->length >> 24;
                if (result->info.extraOffset)
                    output->extra = (void *)(result->info.extraOffset + (u32)dataSmpSDirs[index].entries);
                return 0;
            }
        }
    }
    return -1;
}
void *dataGetCurve(u16 id)
{
    static MEM_DATA key;
    static MEM_DATA *result;
    key.id = id;
    if ((result = sndBSearch(&key, dataCurveTab, dataCurveNum, sizeof(MEM_DATA), curvecmp)) != 0)
        return result->data;
    return 0;
}
void *dataGetKeymap(u16 id)
{
    static MEM_DATA key;
    static MEM_DATA *result;
    key.id = id;
    if ((result = sndBSearch(&key, dataKeymapTab, dataKeymapNum, sizeof(MEM_DATA), curvecmp)) != 0)
        return result->data;
    return 0;
}
void *dataGetLayer(u16 id, u16 *count)
{
    static LAYER_DATA key;
    static LAYER_DATA *result;
    key.id = id;
    if ((result = sndBSearch(&key, dataLayerTab, dataLayerNum, sizeof(LAYER_DATA), layercmp)) != 0) {
        *count = result->count;
        return result->data;
    }
    return 0;
}
void *dataGetFX(u16 id)
{
    static FX_DATA key;
    s32 index;
    FX_DATA *result;
    key.id = id;
    for (index = 0; index < dataFXGroupNum; index++) {
        if ((result = sndBSearch(&key, dataFXGroups[index].entries,
                                 dataFXGroups[index].count, sizeof(FX_DATA), fxcmp)) != 0)
            return result;
    }
    return 0;
}

u32 dataInsertFX(u16 id, FX_DATA *entries, u16 count)
{
    s32 index;
    for (index = 0; index < dataFXGroupNum && dataFXGroups[index].id != id; index++)
        ;
    if (index == dataFXGroupNum && dataFXGroupNum < 128) {
        hwDisableIrq();
        dataFXGroups[dataFXGroupNum].id = id;
        dataFXGroups[dataFXGroupNum].count = count;
        dataFXGroups[dataFXGroupNum].entries = entries;
        for (index = count; index > 0; index--, entries++)
            entries->vGroup = 31;
        dataFXGroupNum++;
        hwEnableIrq();
        return 1;
    }
    return 0;
}
u32 dataRemoveFX(u16 id)
{
    s32 index;
    s32 next;
    for (index = 0; index < dataFXGroupNum && dataFXGroups[index].id != id; index++)
        ;
    if (index != dataFXGroupNum) {
        hwDisableIrq();
        for (next = index + 1; next < dataFXGroupNum; next++)
            dataFXGroups[next - 1] = dataFXGroups[next];
        dataFXGroupNum--;
        hwEnableIrq();
        return 1;
    }
    return 0;
}

u32 dataAddSampleReference(u16 id)
{
    SAMPLE_DATA *sample = 0;
    u32 index;
    SAMPLE_DATA *entry;
    SAMPLE_INFO *info;
    for (index = 0; index < dataSmpSDirNum; index++) {
        for (entry = dataSmpSDirs[index].entries; entry->id != 0xFFFF; entry++) {
            if (entry->id == id && entry->references != 0xFFFF) {
                sample = entry;
                goto found;
            }
        }
    }
found:
    if (!sample->references) {
        sample->address = sample->offset + dataSmpSDirs[index].base;
        info = &sample->info;
        hwSaveSample(&info, &sample->address);
    }
    sample->references++;
    return 1;
}
u32 dataRemoveSampleReference(u16 id)
{
    SAMPLE_DATA *entry;
    u32 index;
    for (index = 0; index < dataSmpSDirNum; index++) {
        for (entry = dataSmpSDirs[index].entries; entry->id != 0xFFFF; entry++) {
            if (entry->id == id && entry->references != 0xFFFF) {
                entry->references--;
                if (entry->references == 0)
                    hwRemoveSample(&entry->info, entry->address);
                return 1;
            }
        }
    }
    return 0;
}

u32 dataInsertKeymap(u16 id, void *data)
{
    s32 index;
    s32 next;
    hwDisableIrq();
    for (index = 0; index < dataKeymapNum && dataKeymapTab[index].id < id; index++)
        ;
    if (index < dataKeymapNum) {
        if (dataKeymapTab[index].id != id) {
            if (dataKeymapNum < 256) {
                for (next = dataKeymapNum - 1; next >= index; next--)
                    dataKeymapTab[next + 1] = dataKeymapTab[next];
                dataKeymapNum++;
            } else {
                hwEnableIrq();
                return 0;
            }
        } else {
            dataKeymapTab[index].references++;
            hwEnableIrq();
            return 0;
        }
    } else {
        if (dataKeymapNum < 256) {
            dataKeymapNum++;
        } else {
            hwEnableIrq();
            return 0;
        }
    }
    dataKeymapTab[index].id = id;
    dataKeymapTab[index].data = data;
    dataKeymapTab[index].references = 1;
    hwEnableIrq();
    return 1;
}
u32 dataRemoveKeymap(u16 id)
{
    s32 index;
    s32 next;
    hwDisableIrq();
    for (index = 0; index < dataKeymapNum && dataKeymapTab[index].id != id; index++)
        ;
    if (index != dataKeymapNum && --dataKeymapTab[index].references == 0) {
        for (next = index + 1; next < dataKeymapNum; next++)
            dataKeymapTab[next - 1] = dataKeymapTab[next];
        dataKeymapNum--;
        hwEnableIrq();
        return 1;
    }
    hwEnableIrq();
    return 0;
}
u32 dataInsertCurve(u16 id, void *data)
{
    s32 index;
    s32 next;
    hwDisableIrq();
    for (index = 0; index < dataCurveNum && dataCurveTab[index].id < id; index++)
        ;
    if (index < dataCurveNum) {
        if (dataCurveTab[index].id != id) {
            if (dataCurveNum < 2048) {
                for (next = dataCurveNum - 1; next >= index; next--)
                    dataCurveTab[next + 1] = dataCurveTab[next];
                dataCurveNum++;
            } else {
                hwEnableIrq();
                return 0;
            }
        } else {
            hwEnableIrq();
            dataCurveTab[index].references++;
            return 0;
        }
    } else {
        if (dataCurveNum < 2048) {
            dataCurveNum++;
        } else {
            hwEnableIrq();
            return 0;
        }
    }
    dataCurveTab[index].id = id;
    dataCurveTab[index].data = data;
    dataCurveTab[index].references = 1;
    hwEnableIrq();
    return 1;
}
u32 dataRemoveCurve(u16 id)
{
    s32 index;
    s32 next;
    hwDisableIrq();
    for (index = 0; index < dataCurveNum && dataCurveTab[index].id != id; index++)
        ;
    if (index != dataCurveNum && --dataCurveTab[index].references == 0) {
        for (next = index + 1; next < dataCurveNum; next++)
            dataCurveTab[next - 1] = dataCurveTab[next];
        dataCurveNum--;
        hwEnableIrq();
        return 1;
    }
    hwEnableIrq();
    return 0;
}
u32 dataInsertLayer(u16 id, void *data, u16 count)
{
    s32 index;
    s32 next;
    hwDisableIrq();
    for (index = 0; index < dataLayerNum && dataLayerTab[index].id < id; index++)
        ;
    if (index < dataLayerNum) {
        if (dataLayerTab[index].id != id) {
            if (dataLayerNum < 256) {
                for (next = dataLayerNum - 1; next >= index; next--)
                    dataLayerTab[next + 1] = dataLayerTab[next];
                dataLayerNum++;
            } else {
                hwEnableIrq();
                return 0;
            }
        } else {
            dataLayerTab[index].references++;
            hwEnableIrq();
            return 0;
        }
    } else {
        if (dataLayerNum < 256) {
            dataLayerNum++;
        } else {
            hwEnableIrq();
            return 0;
        }
    }
    dataLayerTab[index].id = id;
    dataLayerTab[index].data = data;
    dataLayerTab[index].count = count;
    dataLayerTab[index].references = 1;
    hwEnableIrq();
    return 1;
}
u32 dataRemoveLayer(u16 id)
{
    s32 index;
    s32 next;
    hwDisableIrq();
    for (index = 0; index < dataLayerNum && dataLayerTab[index].id != id; index++)
        ;
    if (index != dataLayerNum && --dataLayerTab[index].references == 0) {
        for (next = index + 1; next < dataLayerNum; next++)
            dataLayerTab[next - 1] = dataLayerTab[next];
        dataLayerNum--;
        hwEnableIrq();
        return 1;
    }
    hwEnableIrq();
    return 0;
}

u32 dataInsertSDir(SAMPLE_DATA *entries, u32 sourceBase)
{
    s32 index;
    u16 count;
    SAMPLE_DATA *current;
    s32 directory;
    u16 row;
    u16 other;
    for (index = 0; index < dataSmpSDirNum && dataSmpSDirs[index].entries != entries; index++)
        ;
    if (index == dataSmpSDirNum) {
        if (dataSmpSDirNum < 128) {
            count = 0;
            for (current = entries; current->id != 0xFFFF; current++)
                count++;
            hwDisableIrq();
            for (row = 0; row < count; row++) {
                for (directory = 0; directory < dataSmpSDirNum; directory++) {
                    for (other = 0; other < dataSmpSDirs[directory].count; other++) {
                        if (entries[row].id == dataSmpSDirs[directory].entries[other].id)
                            goto found;
                    }
                }
found:
                if (directory != dataSmpSDirNum)
                    entries[row].references = 0xFFFF;
                else
                    entries[row].references = 0;
            }
            dataSmpSDirs[dataSmpSDirNum].entries = entries;
            dataSmpSDirs[dataSmpSDirNum].count = count;
            dataSmpSDirs[dataSmpSDirNum].base = sourceBase;
            dataSmpSDirNum++;
            hwEnableIrq();
            return 1;
        }
        return 0;
    }
    return 1;
}
u32 dataRemoveSDir(SAMPLE_DATA *entries)
{
    s32 index;
    s32 directory;
    s32 row;
    s32 next;
    SAMPLE_DATA *entry;
    for (index = 0; index < dataSmpSDirNum && dataSmpSDirs[index].entries != entries; index++)
        ;
    if (index != dataSmpSDirNum) {
        hwDisableIrq();
        for (entry = entries; entry->id != 0xFFFF; entry++) {
            if (entry->references != 0xFFFF && entry->references != 0)
                break;
        }
        if (entry->id == 0xFFFF) {
            for (entry = entries; entry->id != 0xFFFF; entry++) {
                if (entry->references != 0xFFFF) {
                    for (directory = 0; directory < dataSmpSDirNum; directory++) {
                        if (dataSmpSDirs[directory].entries != entries) {
                            for (row = 0; row < dataSmpSDirs[directory].count; row++) {
                                if (entry->id == dataSmpSDirs[directory].entries[row].id &&
                                    dataSmpSDirs[directory].entries[row].references == 0xFFFF) {
                                    dataSmpSDirs[directory].entries[row].references = 0;
                                    break;
                                }
                            }
                            if (row != dataSmpSDirs[directory].count)
                                break;
                        }
                    }
                }
            }
            for (entry = entries; entry->id != 0xFFFF; entry++)
                entry->references = 0;
            for (next = index + 1; next < dataSmpSDirNum; next++)
                dataSmpSDirs[next - 1] = dataSmpSDirs[next];
            dataSmpSDirNum--;
            hwEnableIrq();
            return 1;
        }
        hwEnableIrq();
    }
    return 0;
}

u32 dataInsertMacro(u16 id, void *data)
{
    u32 main;
    s32 index;
    s32 slot;
    s32 base;
    hwDisableIrq();
    main = (id & 0xFFC0) >> 6;
    if (!dataMacMainTab[main].count) {
        base = dataMacMainTab[main].offset = dataMacTotal;
        slot = base;
    } else {
        base = dataMacMainTab[main].offset;
        for (index = 0; index < dataMacMainTab[main].count && dataMacSubTabmem[base + index].id < id; index++)
            ;
        if (index < dataMacMainTab[main].count) {
            slot = base + index;
            if (dataMacSubTabmem[slot].id == id) {
                dataMacSubTabmem[slot].references++;
                hwEnableIrq();
                return 0;
            }
        } else {
            slot = base + index;
        }
    }
    if (dataMacTotal < 2048) {
        for (index = 0; index < 512; index++) {
            if (dataMacMainTab[index].offset > base)
                dataMacMainTab[index].offset++;
        }
        for (index = dataMacTotal - 1; index >= slot; index--)
            dataMacSubTabmem[index + 1] = dataMacSubTabmem[index];
        dataMacSubTabmem[slot].id = id;
        dataMacSubTabmem[slot].data = data;
        dataMacSubTabmem[slot].references = 1;
        dataMacMainTab[main].count++;
        dataMacTotal++;
        hwEnableIrq();
        return 1;
    }
    hwEnableIrq();
    return 0;
}
u32 dataRemoveMacro(u16 id)
{
    u32 main;
    s32 base;
    s32 index;
    s32 slot;
    hwDisableIrq();
    if (dataMacMainTab[(id >> 6) & 0x3FF].count) {
        main = ((u32)id >> 6) & 0x3FF;
        base = dataMacMainTab[main].offset;
        for (index = 0; index < dataMacMainTab[main].count && dataMacSubTabmem[base + index].id != id; index++)
            ;
        if (index < dataMacMainTab[main].count) {
            slot = base + index;
            if (--dataMacSubTabmem[slot].references == 0) {
                for (index = slot + 1; index < dataMacTotal; index++)
                    dataMacSubTabmem[index - 1] = dataMacSubTabmem[index];
                for (index = 0; index < 512; index++) {
                    if (dataMacMainTab[index].offset > base)
                        dataMacMainTab[index].offset--;
                }
                dataMacMainTab[main].count--;
                dataMacTotal--;
            }
        }
    }
    hwEnableIrq();
    return 0;
}

/* Reference: https://github.com/AxioDL/musyx/blob/main/src/musyx/runtime/synthdata.c (CC0-1.0). */

extern void hwInitSampleMem(u32 base, u32 length);
void dataInit(u32 base, u32 length)
{
    long index;
    dataSmpSDirNum = 0;
    dataCurveNum = 0;
    dataKeymapNum = 0;
    dataLayerNum = 0;
    dataFXGroupNum = 0;
    dataMacTotal = 0;
    for (index = 0; index < 512; index++) {
        dataMacMainTab[index].count = 0;
        dataMacMainTab[index].offset = 0;
    }
    hwInitSampleMem(base, length);
}
