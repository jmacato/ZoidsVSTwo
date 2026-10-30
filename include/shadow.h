#ifndef ZOIDS_SHADOW_H
#define ZOIDS_SHADOW_H
/* Layout follows
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/shadow.h. */
#include "jobj.h"
#include "tobj.h"
#include "cobj.h"
typedef struct HSD_Shadow {
    HSD_SList *objects;
    HSD_CObj *camera;
    HSD_TObj *texture;
    f32 scaleS;
    f32 scaleT;
    f32 transS;
    f32 transT;
    s32 active;
    u8 intensity;
    void *user_data;
} HSD_Shadow;

typedef struct HSD_ViewingRect {
    Vec3 origin;
    Vec3 up_v;
    Vec3 right_v;
    Vec3 eye_v;
    Vec3 eye_vn;
    f32 distance;
    f32 top;
    f32 bottom;
    f32 left;
    f32 right;
    int perspective;
} HSD_ViewingRect;

HSD_ObjAllocData *HSD_ShadowGetAllocData(void);
void HSD_ShadowInitAllocData(void);
HSD_Shadow *HSD_ShadowAlloc(void);
void HSD_ShadowAddObject(HSD_Shadow *shadow, HSD_JObj *jobj);
void HSD_ShadowDeleteObject(HSD_Shadow *shadow, HSD_JObj *jobj);

void HSD_ShadowStartRender(HSD_Shadow *shadow);
void HSD_ShadowEndRender(HSD_Shadow *shadow);
void HSD_ShadowSetActive(HSD_Shadow *shadow, int active);

void HSD_ShadowRemove(HSD_Shadow *shadow);
void HSD_ShadowInit(HSD_Shadow *shadow);
void HSD_ShadowSetSize(HSD_Shadow *shadow, u16 width, u16 height);

void HSD_ShadowSetViewingRect(HSD_Shadow *shadow, float top, float bottom, float left,
                              float right);
void HSD_ViewingRectInit(HSD_ViewingRect *rect, Vec3 *position, Vec3 *interest,
                         Vec3 *upvector, int perspective);
int HSD_ViewingRectCheck(HSD_ViewingRect *rect);
void HSD_ViewingRectAddRect(HSD_ViewingRect *rect, Vec3 *position, float top,
                            float bottom, float left, float right);

#endif
