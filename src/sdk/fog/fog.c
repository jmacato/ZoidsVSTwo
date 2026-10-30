/* Based on https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/fog.c.
 */
#include "fog.h"
#include "cobj.h"
#include "gx_pixel.h"
extern void *memset(void *memory, s32 value, u32 size);
extern void *memcpy(void *destination, const void *source, u32 size);
extern void __assert(const char *file, u32 line, const char *condition);
extern void HSD_AObjRemove(HSD_AObj *animation);
extern void GXGetViewportv(f32 *viewport);
extern void GXGetProjectionv(f32 *projection);
extern void HSD_Panic(const char *file, u32 line, const char *message);
static void FogInfoInit(void);
static void FogAdjInfoInit(void);
HSD_FogInfo hsdFog = {{FogInfoInit}};
HSD_ClassInfo hsdFogAdj = {FogAdjInfoInit};
static inline HSD_Fog *HSD_FogAlloc(void)
{
    HSD_Fog *fog = hsdNew(&hsdFog.parent);
    fog ? (void)0 : __assert("fog.c", 160, "fog");
    return fog;
}
static inline HSD_FogAdj *HSD_FogAdjAlloc(void)
{
    HSD_FogAdj *adj = hsdNew(&hsdFogAdj);
    adj ? (void)0 : __assert("fog.c", 248, "adj");
    return adj;
}
static inline HSD_FogAdj *fog_get_adj(HSD_Fog *fog)
{
    if (fog != 0)
        return fog->adjustment;
    return 0;
}

void HSD_FogSet(HSD_Fog *fog)
{
    HSD_FogAdj *adjustment;
    HSD_CObj *cobj;
    u32 flags;
    if (fog == 0) {
        GXColor black = {0};
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, black);
        return;
    }
    cobj = HSD_CObjGetCurrent();
    if (cobj == 0)
        HSD_Panic("fog.c", 87, "You must specify CObj first.\n");
    GXSetFog(fog->type, fog->start, fog->end, HSD_CObjGetNear(cobj), HSD_CObjGetFar(cobj), fog->color);
    adjustment = fog_get_adj(fog);
    flags = HSD_FogAdjGetFlags(adjustment);
    if (adjustment != 0 &&
        (flags & (HSD_FOGADJ_USE_CENTER | HSD_FOGADJ_USE_WIDTH | HSD_FOGADJ_USE_MTX))) {
        GXFogAdjTable table;
        f32 viewport[6];
        Mtx44 matrix;
        f32 projection[7];
        f32 (*pmtx)[4];
        s32 center;
        s32 width;

        GXGetViewportv(viewport);
        if (flags & HSD_FOGADJ_USE_CENTER)
            center = viewport[0] + viewport[2] * ((adjustment == 0 ? -1 : adjustment->center) + 320) / 640.0f;
        else
            center = viewport[0] + viewport[2] / 2.0f;
        if (flags & HSD_FOGADJ_USE_WIDTH)
            width = adjustment == 0 ? -1 : adjustment->width;
        else
            width = viewport[2];
        if (flags & HSD_FOGADJ_USE_MTX) {
            pmtx = adjustment == 0 ? 0 : adjustment->matrix;
        } else {
            pmtx = matrix;
            memset(matrix, 0, sizeof(Mtx44));
            GXGetProjectionv(projection);
            switch ((s32)projection[0]) {
            case 0:
                matrix[0][0] = projection[1];
                matrix[0][2] = projection[2];
                matrix[1][1] = projection[3];
                matrix[1][2] = projection[4];
                matrix[2][2] = projection[5];
                matrix[2][3] = projection[6];
                matrix[3][2] = -1.0f;
                break;
            default:
                matrix[0][0] = projection[1];
                matrix[0][3] = projection[2];
                matrix[1][1] = projection[3];
                matrix[1][3] = projection[4];
                matrix[2][2] = projection[5];
                matrix[2][3] = projection[6];
                matrix[3][3] = 1.0f;
                break;
            }
        }
        GXInitFogAdjTable(&table, width, pmtx);
        GXSetFogRangeAdj(1, center, &table);
    } else {
        GXSetFogRangeAdj(0, 0, 0);
    }
}
HSD_Fog *HSD_FogLoadDesc(HSD_FogDesc *description)
{
    HSD_Fog *fog = HSD_FogAlloc();
    /* This SDK string preserves the shared data-pool layout. */
    (void)"You must specify CObj first.\n";

    if (fog == 0)
        __assert("fog.c", 173, "fog");
    HSD_FogInit(fog, description);
    if (description->adjustment != 0)
        fog->adjustment = HSD_FogAdjLoadDesc(description->adjustment);
    return fog;
}
void HSD_FogInit(HSD_Fog *fog, HSD_FogDesc *description)
{
    if (fog != 0) {
        if (description != 0) {
            fog->type = description->type;
            fog->start = description->start;
            fog->end = description->end;
            fog->color = description->color;
        } else {
            f32 viewport[6];
            GXGetViewportv(viewport);
            fog->type = GX_FOG_LIN;
            fog->start = viewport[4];
            fog->end = viewport[5];
            fog->color.r = 255;
            fog->color.g = 255;
            fog->color.b = 255;
            fog->color.a = 255;
        }
    }
}
HSD_FogAdj *HSD_FogAdjLoadDesc(HSD_FogAdjDesc *description)
{
    HSD_FogAdj *adjustment = HSD_FogAdjAlloc();
    if (adjustment == 0)
        __assert("fog.c", 264, "adj");
    HSD_FogAdjInit(adjustment, description);
    return adjustment;
}
void HSD_FogAdjInit(HSD_FogAdj *adjustment, HSD_FogAdjDesc *description)
{
    if (adjustment != 0) {
        if (description != 0) {
            adjustment->flags = description->flags;
            adjustment->width = description->width;
            adjustment->center = description->center;
            memcpy(adjustment->matrix, description->matrix, sizeof(Mtx44));
        } else {
            adjustment->flags = 0;
            adjustment->width = 0;
            adjustment->center = 0;
            memset(adjustment->matrix, 0, sizeof(Mtx44));
        }
    }
}
u32 HSD_FogAdjGetFlags(HSD_FogAdj *adjustment)
{
    return adjustment ? adjustment->flags : 0;
}
static inline void set_center(HSD_Fog *fog, s32 center)
{
    HSD_FogAdj *adjustment = fog ? fog->adjustment : 0;
    if (adjustment != 0) {
        if (center <= -320)
            adjustment->center = -320;
        else if (center >= 320)
            adjustment->center = 320;
        else
            adjustment->center = (s16)center;
    }
}
static inline void set_width(HSD_Fog *fog, s32 width)
{
    HSD_FogAdj *adjustment = fog ? fog->adjustment : 0;
    if (adjustment != 0) {
        if (width <= 0)
            adjustment->width = 0;
        else if (width >= 640)
            adjustment->width = 640;
        else
            adjustment->width = width;
    }
}
static void FogUpdateFunc(void *object, s32 type, AnimationValue *value)
{
    HSD_Fog *fog = object;
    if (fog != 0) {
        switch (type) {
        case 1:
            fog->start = value->scalar;
            return;
        case 2:
            fog->end = value->scalar;
            return;
        case 5:
            fog->color.r = 255.0f * value->scalar;
            return;
        case 6:
            fog->color.g = 255.0f * value->scalar;
            return;
        case 7:
            fog->color.b = 255.0f * value->scalar;
            return;
        case 8:
            fog->color.a = 255.0f * value->scalar;
            return;
        case 20:
            set_center(fog, value->scalar);
            break;
        case 21:
            set_width(fog, value->scalar);
            break;
        }
    }
}
static inline s32 decrement_reference(void *object)
{
    s32 result;
    u16 *count = &((HSD_Obj *)object)->reference_count;
    if ((result = (*count == (u16)-1)))
        return result;
    return (*count)-- == 0;
}
static void FogRelease(HSD_Class *object)
{
    HSD_Fog *fog = (HSD_Fog *)object;
    HSD_FogAdj *adjustment;
    if ((adjustment = fog->adjustment) != 0) {
        if (decrement_reference(adjustment)) {
            if (adjustment != 0) {
                adjustment->parent.parent.class_info->release((HSD_Class *)adjustment);
                adjustment->parent.parent.class_info->destroy((HSD_Class *)adjustment);
            }
        }
    }
    HSD_AObjRemove(fog->animation);
    hsdFog.parent.head.parent->release(object);
}
static void FogInfoInit(void)
{
    hsdInitClassInfo(&hsdFog.parent, &hsdObj, "sysdolphin_base_library", "hsd_fog",
                     sizeof(HSD_FogInfo), sizeof(HSD_Fog));
    hsdFog.parent.release = FogRelease;
    hsdFog.update = FogUpdateFunc;
}
static void FogAdjInfoInit(void)
{
    hsdInitClassInfo(&hsdFogAdj, &hsdObj, "sysdolphin_base_library", "hsd_fogadj",
                     sizeof(HSD_ClassInfo), sizeof(HSD_FogAdj));
}
