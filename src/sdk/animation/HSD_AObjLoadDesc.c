/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/aobj.c. */
#include "aobj.h"
#include "id.h"

extern HSD_AObj *HSD_AObjAlloc(void);
extern void HSD_AObjSetRewindFrame(HSD_AObj *animation, f32 frame);
extern void HSD_AObjSetEndFrame(HSD_AObj *animation, f32 frame);
extern struct HSD_FObj *HSD_FObjLoadDesc(struct HSD_FObjDesc *description);
extern void HSD_FObjRemoveAll(struct HSD_FObj *functions);
extern HSD_JObj *HSD_JObjLoadJoint(HSD_JObjDesc *description);
extern void HSD_JObjUnref(HSD_JObj *joint);
extern void __assert(const char *file, u32 line, const char *condition);

typedef struct ObjectHeader {
    void *class_info;
    u16 reference_count;
    u16 individual_reference_count;
} ObjectHeader;

static inline void reference_object(void *object)
{
    if (object != 0) {
        u16 *reference_count = &((ObjectHeader *)object)->reference_count;

        (*reference_count)++;
        if (*reference_count == 0xFFFF) {
            __assert("object.h", 93, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
        }
    }
}

static inline void set_flags(HSD_AObj *animation, u32 flags)
{
    if (animation != 0) {
        flags &= AOBJ_LOOP | AOBJ_NO_UPDATE;
        animation->flags |= flags;
    }
}

HSD_AObj *HSD_AObjLoadDesc(HSD_AObjDesc *description)
{
    HSD_AObj *animation;
    struct HSD_FObj *functions;
    HSD_JObj *joint;

    if (description != 0) {
        animation = HSD_AObjAlloc();
        set_flags(animation, description->flags);
        HSD_AObjSetRewindFrame(animation, 0.0f);
        HSD_AObjSetEndFrame(animation, description->end_frame);
        functions = HSD_FObjLoadDesc(description->functions);
        if (animation != 0) {
            if (animation->functions != 0) {
                HSD_FObjRemoveAll(animation->functions);
            }
            animation->functions = functions;
        }
        if (description->joint != 0) {
            void *object = HSD_IDGetDataFromTable(0, (u32)description->joint, 0);
            joint = object;
            if (object != 0) {
                reference_object(object);
            } else {
                joint = HSD_JObjLoadJoint(description->joint);
            }
            if (animation != 0) {
                if (animation->joint != 0) {
                    HSD_JObjUnref(animation->joint);
                }
                animation->joint = joint;
            }
        }
        return animation;
    }
    return 0;
}
