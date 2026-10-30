#include "zoid3.h"
#include "model_archive.h"

#pragma auto_inline off
#pragma readonly_strings on

extern ModelArchive *ending;
extern s32 dOn;
extern struct HSD_Fog *fogG;
extern void HSD_FogSet(struct HSD_Fog *fog);
extern void HSD_SetZSortMode(s32 a, s32 b);
extern void jobjDispAll(HSD_JObj *joint);
extern void jobjCopyPosS(HSD_JObj *source, HSD_JObj *target);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *jobj);
extern void sWeaponAnimAll(void);
extern void sWeaponMoveAll(void);
extern void sWeaponDispAll(void);
extern void __assert(const char *file, u32 line, const char *condition);

static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & 0x00800000) && (jobj->flags & 0x40);
}

static inline void HSD_JObjSetRotationY(HSD_JObj *jobj, f32 y)
{
    jobj ? (void)0 : __assert("jobj.h", 696, "jobj");
    !(jobj->flags & 0x20000) ? (void)0 : __assert("jobj.h", 697, "!(jobj->flags & JOBJ_USE_QUATERNION)");
    jobj->rotation[1] = y;
    if (!(jobj->flags & 0x02000000)) {
        if (jobj != 0 && !HSD_JObjMtxIsDirty(jobj))
            HSD_JObjSetMtxDirtySub(jobj);
    }
}

extern void jobjCopyPos(HSD_JObj *src, HSD_JObj *dst);

void closeDisp(void)
{
    if (dOn != 0) {
        if (fogG != 0)
            HSD_FogSet(fogG);
        jobjDispAll(ending->models[0]);
        jobjDispAll(ending->models[1]);
        if (fogG != 0)
            HSD_FogSet(0);
        jobjDispAll(ending->models[2]);
        if (fogG != 0)
            HSD_FogSet(fogG);
        jobjDispAll(ending->models[3]);
        jobjDispAll(ending->models[4]);
        jobjDispAll(ending->models[5]);
        jobjCopyPos(ending->models[4], Zoid[0].joint);
        HSD_JObjSetRotationY(Zoid[0].joint, -1.5707963f);
        if (fogG != 0)
            HSD_FogSet(0);
        jobjDispAll(Zoid[0].joint);
        if (fogG != 0)
            HSD_FogSet(0);
        sWeaponAnimAll();
        sWeaponMoveAll();
        sWeaponDispAll();
        if (fogG != 0)
            HSD_FogSet(0);
    }
}
