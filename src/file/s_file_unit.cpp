extern "C" {
#pragma readonly_strings on
#pragma dont_inline on
#pragma bool off
#include "types.h"

typedef struct MapInfo2 {
    u8 pad_0000[0x321C];
} MapInfo2;
extern s32 strcmp(const char *a, const char *b);
extern char *strcpy(char *dst, const void *src);
extern u32 strlen(const char *s);
extern s32 sprintf(char *buf, const char *fmt, ...);
extern void OSReport(const char *fmt, ...);
extern void ZeroMemory(void *p, u32 size);
extern void MemCpy(void *src, void *dst, u32 size);
extern void *sReadFileLenU(char *name, s32 mode, s32 *length);
extern void *sReadFileU(char *name, s32 mode);
extern void free(void *p);
extern void *malloc2(u32 size);
extern void *mallocSys(u32 size);
extern void mgSearchEnemy(void *mg);
extern void debug11(void);
extern void dspLoadNow(void);
extern s32 DVDOpen(char *name, void *info);
extern s32 DVDClose(void *info);
extern s32 fReadO(void *file, void *mem, s32 len);
extern s32 fReadO2(void *file, void *mem, s32 len, s32 off);
typedef struct DVDFileInfo3 {
    u8 pad_00[0x34];
    s32 length;
    u8 pad_38[8];
} DVDFileInfo3;
extern s32 fexist(char *name);
extern void *HSD_ArchiveParse(void *arc, void *data, u32 size);
typedef struct ImageDesc3 {
    void *image;
    u16 width;
    u16 height;
    s32 format;
} ImageDesc3;
typedef struct TObj3 {
    u8 pad_00[0x58];
    ImageDesc3 *imagedesc;
} TObj3;
extern void putErr(const char *msg);
extern TObj3 *searchTObj(void *jobj);
extern s32 calc_doltexture_size(s32 format, s32 w, s32 h);
extern s32 SetTex2(void *data, void *dest, s32 size);
typedef struct ModelAnimation {
    void *joint;
    void **joint_animations;
    void **material_animations;
    void **shape_animations;
} ModelAnimation;
typedef struct ModelAnimationTable {
    ModelAnimation **models;
    void **camera;
    void **lights;
    void **fog;
} ModelAnimationTable;
typedef struct ModelArchive {
    char name[0x20];
    void *model_data;
    ModelAnimationTable *animations;
    void *owned_data;
    void *camera;
    void *lights;
    void *fog;
    s32 model_count;
    void *models[1];
} ModelArchive;
extern void *sLoad(char *name, void **data);
extern void *LoadArcLight(ModelArchive *archive);
extern void *HSD_CObjLoadDesc(void *desc);
extern void *HSD_FogLoadDesc(void *desc);
extern void *HSD_JObjLoadJoint(void *desc);
extern void HSD_JObjAddAnimAll(void *jobj, void *a, void *b, void *c);
extern void HSD_JObjReqAnimAll(void *jobj, f32 frame);
extern void HSD_ArchiveLoadDepends(void *arc, void *a, void *b, void *c);
extern void HSD_ArchiveResolveReference(void *arc, void *a);
extern ModelAnimationTable *HSD_ArchiveGetPublicAddress(void *arc, const char *name);
extern void sMakeMatrix(void *jobj);
extern void setAnimRate(void *jobj, f32 rate);
extern void Str2Lows(char *s);
extern void sArchivePurge(void *arc);
extern void *TEA_USBFileArchiveLoad(char *name, void **data);
extern s32 fexistU(char *name, s32 usb);
extern void *sArchiveMem(void *mem, s32 size);
typedef struct ClassInfo3 {
    u8 pad_00[0x30];
    void (*release)(void *obj);
    void (*destroy)(void *obj);
} ClassInfo3;
typedef struct HSD_Obj3 {
    ClassInfo3 *class_info;
    u16 reference_count;
} HSD_Obj3;
extern void HSD_LObjRemoveAll(void *lobj);
extern void HSD_JObjRemoveAnimAll(void *jobj);
extern void HSD_JObjUnref(void *jobj);
typedef struct GXColor {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} GXColor;
typedef struct DVDFileInfo {
    u8 cb[0x30];
    u32 startAddr;
    u32 length;
    void *callback;
} DVDFileInfo;
typedef struct DVDDiskID {
    char gameName[4];
    char company[2];
    u8 diskNumber;
    u8 gameVersion;
    u8 streaming;
    u8 streamingBufSize;
    u8 padding[22];
} DVDDiskID;
typedef struct VIStatus3 {
    u8 pad_00[0x48];
    GXColor clear_clr;
} VIStatus3;
typedef struct Fog3 {
    u8 pad_00[0x18];
    GXColor color;
} Fog3;
extern VIStatus3 HSD_VIData;
extern Fog3 *fogG;
extern void HSD_VISetBlack(int black);
extern void HSD_VICopyXFB(int pass);
extern void HSD_VISetXFBDrawDone(void);
extern void VIWaitForRetrace(void);
extern void DVDCancelAll(void);
extern s32 DVDConvertPathToEntrynum(char *path);
extern s32 DVDReadAsyncPrio(void *file, void *address, s32 length, s32 offset, void (*callback)(s32, DVDFileInfo *), s32 prio);
extern void sbk(s32 result, DVDFileInfo *file);
extern void rCbk(s32 result, DVDFileInfo *file);
extern void diskError(s32 mode);
typedef struct GameModeState2 {
    u8 pad_00[8];
    s32 f08;
} GameModeState2;
extern GameModeState2 GameMode;
typedef struct JObj3 {
    u8 pad_00[8];
    struct JObj3 *next;
    struct JObj3 *parent;
    struct JObj3 *child;
} JObj3;
extern s32 DVDSeekAsyncPrio(DVDFileInfo *file, s32 offset, void (*callback)(s32, DVDFileInfo *), s32 prio);
extern s32 DVDCheckDisk(void);
extern s32 DVDGetCommandBlockStatus(void *block);
extern DVDDiskID *DVDGetCurrentDiskID(void);
extern s32 strncmp(const char *a, const char *b, u32 n);
extern void TEA_VoiceOff(void);
extern void HSD_FogSet(Fog3 *fog);
extern void HSD_StartRender(int pass);
extern void HSD_EndRender(void);
extern void sPadAll(void);
extern s32 DVDGetDriveStatus(void);
extern void diskError2(s32 error);
extern void diskError3(s32 message);
extern s32 fChk(char *path);
extern void HSD_StateInvalidate(s32 state);
extern void HSD_SetZSortMode(s32 a, s32 b);
extern void GXSetCullMode(s32 mode);
extern s32 sPadResetSTOP;
extern s32 sPutFontCamPrologue(void);
extern void sPutStringXYC2(char *text, GXColor color, GXColor shadow, s32 alpha, s32 flags, s32 align, f32 x,
                           f32 y, f32 z, f32 scale_x, f32 scale_y);
extern void HSD_CObjEndCurrent(void);
#pragma dont_inline off
static inline void setEraseColor(GXColor color)
{
    HSD_VIData.clear_clr = color;
}
static inline s32 decrement_reference(HSD_Obj3 *object)
{
    s32 result;
    if ((result = (object->reference_count == (u16)-1)))
        return result;
    result = object->reference_count == 0;
    object->reference_count--;
    return result;
}
static inline void delete_object(void *object)
{
    if (object != 0) {
        ((HSD_Obj3 *)object)->class_info->release(object);
        ((HSD_Obj3 *)object)->class_info->destroy(object);
    }
}
static inline void unreference_object(void *object)
{
    if (object != 0 && decrement_reference((HSD_Obj3 *)object))
        delete_object(object);
}
#pragma dont_inline on

#pragma force_active on
static s32 adr[16];
MapInfo2 mgI;
static char det_Fname[80];
static char det_Mem[400];
static s32 arc2;
s32 usbForce;
static s32 dOpen;
s32 dspLoadF;
static s32 rCbkR;
s32 dbgAllocS;
s32 dbgAllocU;
s32 enemyZoid[2];
s32 enemyChara[2];
#pragma force_active off
#pragma dont_inline on
#pragma force_active on
static u32 diskErrMsg[10] = {
    0x80118708, 0x80118714, 0x8011876C, 0x801187A0, 0x8011880C, 0x8011885C, 0x802F05CC, 0x802F05CC, 
    0x802F05CC, 0x802F05CC, 
};
static u32 rdMapStr[5] = {
    0x802F06A8, 0x802F06AC, 0x802F06B0, 0x802F06B4, 0x802F06B8, 
};
static u32 lit_1733[15] = {
    0x800132D0, 0x800132F0, 0x800132F0, 0x80013308, 0x80013308, 0x80013308, 0x80013324, 0x80013340, 
    0x800132D0, 0x800132F0, 0x80013308, 0x80013340, 0x80013340, 0x80013340, 0x800132D0, 
};
static const char lit_386[9] = "LOADing\n";
#pragma force_active off
s32 fChk(char *path)
{
    DVDFileInfo file;

    if (DVDCheckDisk() == 0)
        return 0;
    if (DVDConvertPathToEntrynum("ZOIDS.VS2") == -1)
        return 0;
    if (DVDOpen(path, &file) == 0)
        return 0;
    DVDSeekAsyncPrio(&file, 10, sbk, 2);
    DVDClose(&file);
    if (DVDCheckDisk() == 0)
        return 0;
    if (DVDConvertPathToEntrynum("ZOIDS.VS2") == -1)
        return 0;
    if (DVDOpen(path, &file) == 0)
        return 0;
    DVDSeekAsyncPrio(&file, 0, sbk, 2);
    DVDClose(&file);
    if (DVDCheckDisk() == 0)
        return 0;
    return 1;
}
#undef lit_481
#define lit_481 "ZOIDS.VS2"
#pragma force_active on
static const char lit_482[11] = "file error";
static const char lit_483[86] = "\x83" "f\x83" "B\x83X\x83N\x83J\x83o\x81[\x82\xaa\x8aJ\x82\xa2\x82\xc4\x82\xa2\x82\xdc\x82\xb7\x81" "B\x83Q\x81[\x83\x80\x82\xf0\x91\xb1\x82\xaf\x82\xe9\x8f\xea\x8d\x87\x82\xcd\x81" "A\n\x83" "f\x83" "B\x83X\x83N\x83J\x83o\x81[\x82\xf0\x95\xc2\x82\xdf\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\x81" "B";
static const char lit_484[50] = "\x83]\x83" "C\x83h\x83o\x81[\x83T\x83X\x82Q\x82\xcc\x83" "f\x83" "B\x83X\x83N\x82\xf0\x83Z\x83" "b\x83g\n\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\x81" "B";
static const char lit_485[108] = "\x82\xb1\x82\xcc\x83" "f\x83" "B\x83X\x83N\x82\xcd\x83]\x83" "C\x83h\x83o\x81[\x83T\x83X\x82Q\x82\xcc\n\x83" "f\x83" "B\x83X\x83N\x82\xc5\x82\xcd\x82\xa0\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81" "B\n\x83]\x83" "C\x83h\x83o\x81[\x83T\x83X\x82Q\x82\xcc\x83" "f\x83" "B\x83X\x83N\x82\xf0\x83Z\x83" "b\x83g\n\x82\xb5\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\x81" "B";
static const char lit_486[77] = "\x83" "f\x83" "B\x83X\x83N\x82\xf0\x93\xc7\x82\xdf\x82\xdc\x82\xb9\x82\xf1\x82\xc5\x82\xb5\x82\xbd\x81" "B\n\x82\xad\x82\xed\x82\xb5\x82\xad\x82\xcd\x81" "A\x96{\x91\xcc\x82\xcc\x8e\xe6\x88\xb5\x90\xe0\x96\xbe\x8f\x91\x82\xf0\n\x82\xa8\x93\xc7\x82\xdd\x82\xad\x82\xbe\x82\xb3\x82\xa2\x81" "B";
static const char lit_487[110] = "\x83G\x83\x89\x81[\x82\xaa\x94\xad\x90\xb6\x82\xb5\x82\xdc\x82\xb5\x82\xbd\x81" "B\n\x96{\x91\xcc\x82\xcc\x83p\x83\x8f\x81[\x83{\x83^\x83\x93\x82\xf0\x89\x9f\x82\xb5\x82\xc4\x93" "d\x8c\xb9\x82\xf0\n\x82n\x82" "e\x82" "e\x82\xc9\x82\xb5\x81" "A\x96{\x91\xcc\x82\xcc\x8e\xe6\x88\xb5\x90\xe0\x96\xbe\x8f\x91\x82\xcc\n\x8ew\x8e\xa6\x82\xc9\x8f]\x82\xc1\x82\xc4\x82\xad\x82\xbe\x82\xb3\x82\xa2\x81" "B";
static const char lit_662[9] = "PreOpen:";
static const char lit_700[50] = "error alloc from usrHeap .......................\n";
static const char lit_707[19] = "\x83\x81\x83\x82\x83\x8a\x82\xaa\x91\xab\x82\xe8\x82\xdc\x82\xb9\x82\xf1";
static const f32 lit_378[1] = { 0.0f };
static const char lit_460[7] = "error\n";
namespace { __declspec(section ".sdata2") extern const f32 lit_461[1] = { 0.0f }; }
static const f32 lit_462[1] = { -300.0f };
static const f32 lit_463[1] = { 0.6f };
static const f32 lit_464[1] = { 0.7f };
static const char lit_488[7] = "\x83G\x83\x89\x81[";
#pragma force_active off
void diskError3(s32 message)
{
    s32 i;
    for (i = 0; i < 10; i++) {
        HSD_StartRender(0);
        OSReport("E%d.", message);
        if (sPutFontCamPrologue() != 0) {
            if (message != 0) {
                if (message < 0)
                    message = 6;
                if (message > 7)
                    message = 6;
                sPutStringXYC2(((char **)diskErrMsg)[message], (GXColor){255, 255, 255, 255},
                               (GXColor){0, 0, 0, 0}, 0, 2, 3, lit_461[0], lit_461[0], lit_462[0], lit_463[0], lit_464[0]);
                HSD_StateInvalidate(-1);
            }
            HSD_CObjEndCurrent();
        }
        OSReport("n.");
        HSD_EndRender();
        HSD_VICopyXFB(0);
        HSD_VISetXFBDrawDone();
        VIWaitForRetrace();
        sPadAll();
        DVDGetDriveStatus();
    }
}
#undef lit_513
#define lit_513 "E%d."
#undef lit_514
#define lit_514 "n."
#pragma dont_inline off
void diskError2(s32 error)
{
    s32 message = 0;
    s32 pad_b;
    s32 pad_a;
    DVDFileInfo file;
    s32 status;
    s32 timer;
    s32 opened;

    OSReport(lit_460);
    pad_a = 0;
    pad_b = 0;
    timer = 15;
    opened = 0;
    VIWaitForRetrace();
    HSD_StateInvalidate(-1);
    HSD_SetZSortMode(0, 0);
    GXSetCullMode(0);
    setEraseColor((GXColor){ 0, 0, 0, 0 });
    HSD_FogSet(0);
    status = 0;
    for (;;) {
        DVDCheckDisk();
        DVDGetDriveStatus();
        VIWaitForRetrace();
        sPadAll();
        if (timer != 0)
            timer--;
        else
            error = 0;
        if (error != 0)
            message = error;
        if (status == 1) {
            status = DVDGetDriveStatus();
            continue;
        }
        diskError3(message);
        status = DVDGetDriveStatus();
        OSReport("A0");
        message = 0;
        if (status == 5) {
            message = 1;
            error = 0;
        }
        if (status == 4)
            message = 2;
        if (status == 6)
            message = 3;
        if (status == 0 && error != 3 && fChk(lit_481) == 0) {
            (void)0;
            break;
        }
        if (status == 11)
            message = 4;
        if (status == -1)
            message = 5;
        OSReport("[%d.%d]", status, message);
        if (message != 0)
            timer = 60;
        if (timer != 0 && DVDCheckDisk() != 0) {
            message = 1;
            continue;
        }
        if (message != 0 || timer != 0 || (status != 0 && status != -1) || error != 0 || status != 0)
            continue;
        break;
    }
    if (opened != 0)
        DVDClose(&file);
    OSReport("A9 ");
    if (fogG != 0)
        setEraseColor(fogG->color);
    HSD_StateInvalidate(-1);
    DVDCheckDisk();
    DVDGetDriveStatus();
    VIWaitForRetrace();
    sPadAll();
}
#pragma dont_inline on
#undef lit_571
#define lit_571 "A0"
#undef lit_572
#define lit_572 "[%d.%d]"
#undef lit_573
#define lit_573 "A9 "
#pragma dont_inline off
void diskError(s32 mode)
{
    DVDFileInfo file;
    DVDDiskID *id;
    s32 state;
    s32 bad;
    s32 opened = 0;

    dOpen = 0;
    HSD_VISetBlack(0);
    HSD_VICopyXFB(0);
    HSD_VISetXFBDrawDone();
    VIWaitForRetrace();
    HSD_VICopyXFB(0);
    HSD_VISetXFBDrawDone();
    VIWaitForRetrace();
    for (;;) {
        diskError2(mode);
        (void)-opened;
        DVDCancelAll();
        (void)(u16)bad;
        OSReport("D-");
        if (DVDConvertPathToEntrynum(lit_481) == -1) {
            mode = 3;
            OSReport("D3");
            continue;
        }
        if (!(opened = DVDOpen(lit_481, &file))) {
            mode = 3;
            OSReport("D4");
            continue;
        }
        if (!DVDReadAsyncPrio(&file, adr, 32, 0, sbk, 2)) {
            mode = 3;
            OSReport("D6");
            continue;
        }
        for (;;) {
            (void)mode;
            (void)mode;
            (void)mode;
            (void)mode;
            (void)mode;
            (void)mode;
            (void)mode;
            DVDCheckDisk();
            state = DVDGetCommandBlockStatus(&file.cb);
            OSReport("%d.", state);
            if (state == 0)
                break;
            if (state == 1)
                continue;
            if (state == 2)
                continue;
            if (state == 4)
                diskError2(2);
            else if (state == 6)
                diskError2(3);
            break;
        }
        DVDClose(&file);
        if (!DVDCheckDisk()) {
            OSReport("D/");
            mode = 1;
            DVDClose(&file);
            continue;
        }
        bad = 0;
        id = DVDGetCurrentDiskID();
        if (id == (void *)0)
            bad = 1;
        if (id != 0 && strncmp(id->gameName, "GZSJ", 4) != 0)
            bad = 1;
        if (id != 0 && strncmp(id->company, "DA", 2) != 0)
            bad = 1;
        if (bad != 0) {
            OSReport("Din");
            mode = 3;
            continue;
        }
        if (!(opened = DVDOpen(lit_481, &file))) {
            mode = 3;
            OSReport("D7");
            continue;
        }
        break;
    }
    DVDCancelAll();
    DVDClose(&file);
    TEA_VoiceOff();
    OSReport("Dh");
    setEraseColor((GXColor){ 0, 0, 0, 0 });
    HSD_FogSet(0);
    HSD_StartRender(0);
    HSD_EndRender();
    HSD_VICopyXFB(0);
    HSD_VISetXFBDrawDone();
    VIWaitForRetrace();
    sPadAll();
    DVDGetDriveStatus();
    if (fogG != 0)
        HSD_FogSet(fogG);
    if (fogG != 0)
        setEraseColor(fogG->color);
}
#pragma dont_inline on
#undef lit_626
#define lit_626 "D-"
#undef lit_627
#define lit_627 "D3"
#undef lit_628
#define lit_628 "D4"
#undef lit_629
#define lit_629 "D6"
#undef lit_630
#define lit_630 "%d."
#undef lit_631
#define lit_631 "D/"
#undef lit_632
#define lit_632 "GZSJ"
#undef lit_633
#define lit_633 "DA"
#undef lit_634
#define lit_634 "Din"
#undef lit_635
#define lit_635 "D7"
#undef lit_636
#define lit_636 "Dh"
s32 chkVS3(void)
{
    DVDFileInfo file;
    s32 ok;
    ok = DVDOpen("ZOIDS.VS2", &file);
    if (!ok)
        return 0;
    DVDClose(&file);
    return 1;
}
s32 chkVS(void)
{
    free(sReadFileU("ZOIDS.VS2", 0));
    return 0;
}
#pragma force_active on
static const char lit_663[4] = "Cl:";
static const char lit_664[4] = "V3n";
static const char lit_665[4] = "V3a";
static const char lit_666[4] = "Op:";
static const char lit_667[8] = "PreRet:";
#pragma force_active off
s32 fReadO2(void *file, void *memory, s32 length, s32 offset)
{
    s32 result;

    rCbkR = 0;
    OSReport("Syns");
    result = DVDReadAsyncPrio(file, memory, (length + 31) & ~31, offset, rCbk, 2);
    OSReport("Syne");
    if (result == 0) {
        OSReport("Synr");
        return -1;
    }
    for (;;) {
        s32 state;

        if (!DVDCheckDisk())
            diskError(0);
        state = DVDGetCommandBlockStatus(file);
        if (state == 0)
            break;
        if (state == 1 || state == 2)
            continue;
        diskError(0);
    }
    return 1;
}
#undef lit_687
#define lit_687 "Syns"
#undef lit_688
#define lit_688 "Syne"
#undef lit_689
#define lit_689 "Synr"
void *sReadFileLen(char *name, s32 *size)
{
    DVDFileInfo3 file;
    s32 length;
    void *memory;

    if (size != 0)
        *size = 0;
    debug11();
    dspLoadNow();
    memory = 0;
    OSReport("loading file (%s) ...", name);
    if (!DVDOpen(name, &file)) {
        OSReport("cannot open file\n", name);
        return 0;
    }
    length = file.length;
    OSReport("size[%d]..", length);
    if (length != 0) {
        memory = malloc2((length + 31) & ~31);
        if (memory == 0) {
            OSReport("error........cannot allocate memory\n");
            DVDClose(&file);
            return 0;
        }
        OSReport("memAloc...[%X]....", memory);
        if (fReadO(&file, memory, length) < 0) {
            OSReport("cannot load file\n");
            DVDClose(&file);
            free(memory);
            OSReport(lit_460);
            return 0;
        }
    }
    DVDClose(&file);
    OSReport("Done\n");
    if (size != 0)
        *size = length;
    return memory;
}
#undef lit_734
#define lit_734 "loading file (%s) ..."
#undef lit_735
#define lit_735 "cannot open file\n"
#undef lit_736
#define lit_736 "size[%d].."
#undef lit_737
#define lit_737 "error........cannot allocate memory\n"
#undef lit_738
#define lit_738 "memAloc...[%X]...."
#undef lit_739
#define lit_739 "cannot load file\n"
#undef lit_740
#define lit_740 "Done\n"
s32 sReadFileLenPtr(char *name, char *buf, s32 ofst, s32 len)
{
    DVDFileInfo3 file;
    s32 size;
    s32 base;
    s32 result;
    char *mem;

    debug11();
    dspLoadNow();
    OSReport("loading file (%s) ...", name);
    if (!DVDOpen(name, &file)) {
        OSReport("cannot open file\n", name);
        return -1;
    }
    size = file.length;
    OSReport("size[%d]..", size);
    if (size <= ofst)
        size = 0;
    if (size < ofst + len)
        len = size - ofst;
    if (len <= 0)
        len = 0;
    if (len == 0)
        size = 0;
    result = len;
    if (size != 0) {
        base = ofst & ~3;
        mem = (char *)malloc2(len + 32);
        if (mem != 0) {
            fReadO2(&file, mem, (len + (ofst - base) + 31) & ~31, base);
            MemCpy(mem + (ofst - base), buf, len);
            free(mem);
        }
    }
    buf[len] = 0;
    DVDClose(&file);
    OSReport("Done\n");
    return result;
}
void *sArchiveLoad(char *name, void **out)
{
    s32 length;
    void *data;
    void *arc;

    if (out != 0)
        *out = 0;
    OSReport("archive load %s ", name);
    if (!fexist(name)) {
        OSReport("where?\n");
        return 0;
    }
    data = sReadFileLen(name, &length);
    if (data == 0)
        return 0;
    if (out != 0)
        *out = data;
    arc = mallocSys(0x44);
    if (arc == 0) {
        OSReport("error lost \n");
        free(data);
        if (out != 0)
            *out = 0;
        return 0;
    }
    OSReport("archive ok \n");
    HSD_ArchiveParse(arc, data, length);
    return arc;
}
#undef lit_779
#define lit_779 "archive load %s "
#undef lit_780
#define lit_780 "where?\n"
#undef lit_781
#define lit_781 "error lost \n"
#undef lit_782
#define lit_782 "archive ok \n"
#pragma force_active on
static const char lit_788[21] = "sArcMem error lost \n";
static const char lit_838[6] = "%s_%s";
static const char lit_868[7] = "lobj.h";
static const char lit_869[5] = "lobj";
#pragma force_active off
namespace { extern const f32 lit_461[1]; }
s32 LoadArc(ModelArchive *archive, char *name)
{
    char *base;
    char *p;
    s32 length;
    s32 unused = 0;
    void *data;
    void *joint;

    p = name;
    base = name;
    for (; *p != 0; p++) {
        if (*p == '/')
            base = p + 1;
    }
    length = strlen(base);
    strcpy(archive->name, base);
    Str2Lows(archive->name);
    if (length > 4)
        archive->name[length - 4] = 0;
    archive->model_data = 0;
    archive->owned_data = 0;
    archive->models[0] = 0;
    if (!fexist(name)) {
        OSReport("not found. [%s]*************************\n", name);
        return 0;
    }
    archive->model_data = sLoad(name, &data);
    archive->owned_data = data;
    HSD_ArchiveLoadDepends(archive->model_data, 0, 0, 0);
    HSD_ArchiveResolveReference(archive->model_data, 0);
    archive->animations = HSD_ArchiveGetPublicAddress(archive->model_data, "scene_data");
    archive->lights = LoadArcLight(archive);
    if (archive->animations->models != 0) {
        joint = HSD_JObjLoadJoint(archive->animations->models[0]->joint);
        archive->models[0] = joint;
        HSD_JObjAddAnimAll(joint,
                           archive->animations->models[0]->joint_animations ? *archive->animations->models[0]->joint_animations : 0,
                           archive->animations->models[0]->material_animations ? *archive->animations->models[0]->material_animations : 0,
                           archive->animations->models[0]->shape_animations ? *archive->animations->models[0]->shape_animations : 0);
    }
    sMakeMatrix(joint);
    HSD_JObjReqAnimAll(joint, lit_461[0]);
    setAnimRate(joint, 1.0f);
    archive->camera = HSD_CObjLoadDesc(*archive->animations->camera);
    if (archive->animations->fog != 0 && *archive->animations->fog != 0)
        archive->fog = HSD_FogLoadDesc(*archive->animations->fog);
    OSReport("LoadArc  -----%5d %s\n", arc2, name);
    arc2++;
    return 1;
}
#undef lit_899
#define lit_899 "not found. [%s]*************************\n"
#undef lit_900
#define lit_900 "scene_data"
#undef lit_902
#define lit_902 "LoadArc  -----%5d %s\n"
namespace { extern const f32 lit_461[1]; extern const f32 lit_901[1]; }
ModelArchive *LoadArc2_(char *name, s32 usb)
{
    char *p;
    s32 count;
    ModelAnimationTable *scene;
    char *base;
    s32 i = 0;
    void *joint;
    s32 size;
    void *model_data;
    ModelArchive *archive;
    void *data;

    data = 0;
    if (usbForce != 0)
        usb = 1;
    if (usb != 0) {
        model_data = TEA_USBFileArchiveLoad(name, &data);
    } else {
        if (!fexistU(name, usb)) {
            OSReport(lit_899, name);
            return 0;
        }
        model_data = sLoad(name, &data);
    }
    if (model_data == 0)
        return 0;
    OSReport("LoadArc2 -----%5d %s\n", arc2, name);
    arc2++;
    HSD_ArchiveLoadDepends(model_data, 0, 0, 0);
    HSD_ArchiveResolveReference(model_data, 0);
    scene = HSD_ArchiveGetPublicAddress(model_data, lit_900);
    count = 0;
    if (scene->models != 0) {
        while (scene->models[count] != 0)
            count++;
    }
    size = count * 4 + 0x40;
    archive = (ModelArchive *)malloc2(size);
    if (archive == (void *)0) {
        if (data != 0)
            free(data);
        if (model_data != 0)
            sArchivePurge(model_data);
        putErr(lit_707);
        return 0;
    }
    ZeroMemory(archive, size);
    archive->owned_data = data;
    archive->animations = scene;
    archive->model_data = model_data;
    (size = 0, arc2);
    archive->model_count = count;
    p = name;
    i = count + -name[count];
    base = name;
    for (; *p != 0; p++) {
        if (*p == '/')
            base = p + 1;
    }
    size = strlen(base);
    strcpy(archive->name, base);
    if (size > 4)
        archive->name[size - 4] = 0;
    if (scene->models != 0) {
        for (i = 0; i < count; i++) {
            joint = HSD_JObjLoadJoint(scene->models[i]->joint);
            if (joint != (void *)0) {
                HSD_JObjAddAnimAll(joint,
                                   scene->models[i]->joint_animations ? *scene->models[i]->joint_animations : 0,
                                   scene->models[i]->material_animations ? *scene->models[i]->material_animations : 0,
                                   scene->models[i]->shape_animations ? *scene->models[i]->shape_animations : 0);
                archive->models[i] = joint;
                sMakeMatrix(joint);
                HSD_JObjReqAnimAll(joint, lit_461[0]);
                setAnimRate(joint, 1.0f);
            }
        }
    }
    archive->lights = LoadArcLight(archive);
    archive->camera = HSD_CObjLoadDesc(*scene->camera);
    if (scene->fog != 0 && *scene->fog != 0)
        archive->fog = HSD_FogLoadDesc(*scene->fog);
    return archive;
}
#undef lit_973
#define lit_973 "LoadArc2 -----%5d %s\n"
namespace { extern const f32 lit_461[1]; extern const f32 lit_901[1]; }
ModelArchive *LoadArc2B(void *mem, s32 memsize, char *name)
{
    s32 i = 0;
    ModelAnimationTable *scene;
    void *joint;
    s32 count;
    ModelArchive *archive;
    void *model_data;
    s32 size;

    model_data = sArchiveMem(mem, memsize);
    if (model_data == 0)
        return 0;
    OSReport("LoadArc2B -----%5d %s\n", arc2, name);
    arc2++;
    HSD_ArchiveLoadDepends(model_data, 0, 0, 0);
    HSD_ArchiveResolveReference(model_data, 0);
    scene = HSD_ArchiveGetPublicAddress(model_data, lit_900);
    count = 0;
    if (scene->models != 0) {
        while (scene->models[count] != 0)
            count++;
    }
    size = count * 4 + 0x40;
    archive = (ModelArchive *)malloc2(size);
    if (archive == (void *)0) {
        if (model_data != 0)
            sArchivePurge(model_data);
        putErr(lit_707);
        return 0;
    }
    ZeroMemory(archive, size);
    archive->owned_data = 0;
    archive->animations = scene;
    archive->model_data = model_data;
    archive->model_count = count;
    strcpy(archive->name, name);
    if (scene->models != 0) {
        for (i = 0; i < count; i++) {
            joint = HSD_JObjLoadJoint(scene->models[i]->joint);
            if (joint != (void *)0) {
                HSD_JObjAddAnimAll(joint,
                                   scene->models[i]->joint_animations ? *scene->models[i]->joint_animations : 0,
                                   scene->models[i]->material_animations ? *scene->models[i]->material_animations : 0,
                                   scene->models[i]->shape_animations ? *scene->models[i]->shape_animations : 0);
                archive->models[i] = joint;
                sMakeMatrix(joint);
                HSD_JObjReqAnimAll(joint, lit_461[0]);
                setAnimRate(joint, 1.0f);
            }
        }
    }
    archive->lights = LoadArcLight(archive);
    archive->camera = HSD_CObjLoadDesc(*scene->camera);
    if (scene->fog != 0 && *scene->fog != 0)
        archive->fog = HSD_FogLoadDesc(*scene->fog);
    return archive;
}
#undef lit_1003
#define lit_1003 "LoadArc2B -----%5d %s\n"
#pragma force_active on
static const char lit_1092[21] = "light anim TBL non \n";
static const char lit_1093[32] = "light non anim  req=%d(in %d) \n";
static const char lit_1149[35] = "PurgeArc2 -----------------%5d %s\n";
#pragma force_active off
#pragma dont_inline off
void PurgeArc2(ModelArchive *archive)
{
    if (archive != (void *)0 && archive->model_data != 0) {
        s32 model;
        OSReport("lobj free ---------------------------------\n");
        if (archive->lights != 0) {
            HSD_LObjRemoveAll(archive->lights);
            archive->lights = 0;
        }
        OSReport("cobj free \n");
        if (archive->camera != 0) {
            unreference_object(archive->camera);
            archive->camera = 0;
        }
        for (model = 0; model < archive->model_count; model++) {
            if (archive->models[model] != 0) {
                HSD_JObjRemoveAnimAll(archive->models[model]);
                HSD_JObjUnref(archive->models[model]);
                archive->models[model] = 0;
            }
        }
        if (archive->fog != 0) {
            unreference_object(archive->fog);
            archive->fog = 0;
        }
        if (archive->model_data != 0)
            sArchivePurge(archive->model_data);
        archive->model_data = 0;
        arc2--;
        OSReport(lit_1149, arc2, archive);
        if (archive->owned_data != 0)
            free(archive->owned_data);
        archive->owned_data = 0;
        OSReport("fl ");
        free(archive);
        OSReport("free OK \n ");
    }
}
#pragma dont_inline on
#undef lit_1190
#define lit_1190 "lobj free ---------------------------------\n"
#undef lit_1191
#define lit_1191 "cobj free \n"
#undef lit_1192
#define lit_1192 "fl "
#undef lit_1193
#define lit_1193 "free OK \n "
JObj3 *GetChild(JObj3 *joint, s32 first, s32 second, s32 third, s32 fourth, s32 fifth)
{
    s32 path[8];
    s32 *p;

    if (first == -1)
        return joint;
    path[0] = first;
    path[1] = second;
    path[2] = third;
    path[3] = fourth;
    path[4] = fifth;
    path[5] = -1;
    p = path;
    for (;;) {
        for (first = *p; first > 0; first--) {
            joint = joint ? joint->next : 0;
            if (joint == (JObj3 *)0)
                return 0;
        }
        if (*++p == -1)
            return joint;
        joint = joint ? joint->child : 0;
    }
}
void *rdMapInfo(s32 mode, s32 number)
{
    char name[256];
    void *data;

    if (mode == 3) {
        if (GameMode.f08 == 1)
            mode = 4;
    }
    ZeroMemory(&mgI, 0x321C);
    sprintf(name, "menu/map_info_dat/%s%03d.mg", ((char **)rdMapStr)[mode], number);
    data = sReadFileU(name, 0);
    if (data == (void *)0)
        return 0;
    MemCpy(data, &mgI, 0x321C);
    free(data);
    mgSearchEnemy(&mgI);
    return &mgI;
}
#undef lit_1709
#define lit_1709 "menu/map_info_dat/%s%03d.mg"
#pragma force_active on
static const char lit_1735[22] = "imgsize=%d tsize=%d \n";
#pragma force_active off
char *det_Load(char *name)
{
    s32 length;
    void *data;
    if (strcmp(name, det_Fname) == 0)
        return det_Mem;
    det_Mem[0] = 0;
    strcpy(det_Fname, name);
    data = sReadFileLenU(name, 0, &length);
    if (data == 0)
        return det_Mem;
    if (length > 0x18d)
        length = 0x18d;
    if (length < 0)
        length = 0;
    ((char *)data)[length] = 0;
    strcpy(det_Mem, data);
    free(data);
    return det_Mem;
}
s32 jobjColor(void *jobj, char *name, s32 usb)
{
    ImageDesc3 *desc;
    TObj3 *tobj;
    void *data;
    void *image;
    s32 size;

    if (jobj == (void *)0) {
        putErr("jobjColor : jobj not found.");
        return 0;
    }
    tobj = searchTObj(jobj);
    if (tobj == (void *)0) {
        putErr("jobjColor : tobj not found.");
        return 0;
    }
    desc = tobj->imagedesc;
    if (desc == (void *)0) {
        putErr("jobjColor : imagedesc(tobj) not found.");
        return 0;
    }
    size = calc_doltexture_size(desc->format, desc->width, desc->height);
    image = desc->image;
    data = sReadFileU(name, usb);
    if (data != 0 && SetTex2(data, image, size) != 0) {
        free(data);
        return 1;
    }
    if (data == (void *)0) {
        putErr("jobjColor : file not found.");
        return 0;
    }
    OSReport("tex set. error   jobj-tex (%d,%d,[%d])\n", desc->width, desc->height, desc->format);
    if (desc->format != 14) {
        putErr("jobjColor : set Tex  error    jobj-tex not CMPR");
        OSReport("jobj -tex   not CMPR\n");
    }
    free(data);
    return 0;
}
#undef lit_1751
#define lit_1751 "jobjColor : jobj not found."
#undef lit_1752
#define lit_1752 "jobjColor : tobj not found."
#undef lit_1753
#define lit_1753 "jobjColor : imagedesc(tobj) not found."
#undef lit_1754
#define lit_1754 "jobjColor : file not found."
#undef lit_1755
#define lit_1755 "tex set. error   jobj-tex (%d,%d,[%d])\n"
#undef lit_1756
#define lit_1756 "jobjColor : set Tex  error    jobj-tex not CMPR"
#undef lit_1757
#define lit_1757 "jobj -tex   not CMPR\n"
#pragma force_active on
static const char lit_1403[7] = "jobj.h";
static const char lit_1404[5] = "jobj";
static const f64 lit_1624[1] = { 1.0 };
static const char lit_1697[2] = "B";
static const char lit_1698[2] = "M";
static const char lit_1699[2] = "D";
static const char lit_1700[2] = "V";
static const char lit_1701[2] = "F";
#pragma force_active off
}
