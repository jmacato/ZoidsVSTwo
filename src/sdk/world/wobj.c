/* Based on
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/wobj.c. */
#include "wobj.h"
#include "joint_matrix.h"
#include "object.h"
void WObjInfoInit(void);
HSD_WObjInfo hsdWObj = {{{WObjInfoInit}}};
extern HSD_ClassInfo *default_class;
extern void splArcLengthPoint(Vec3 *point, struct HSD_Spline *spline, f32 distance);

void HSD_WObjReqAnim(HSD_WObj *object, f32 frame)
{
    if (object != 0) {
        HSD_AObjReqAnim(object->animation, frame);
        HSD_RObjReqAnimAll(object->render_object, frame);
    }
}

void HSD_WObjAddAnim(HSD_WObj *object, HSD_WObjAnim *animation)
{
    if (object != 0 && animation != 0) {
        if (object->animation != 0) {
            HSD_AObjRemove(object->animation);
        }
        object->animation = HSD_AObjLoadDesc(animation->animation);
        HSD_RObjAddAnimAll(object->render_object, animation->render_animation);
    }
}

void WObjUpdateFunc(void *object, s32 type, AnimationValue *value)
{
    HSD_WObj *world_object;
    Vec3 position;
    HSD_JObj *joint;

    world_object = object;
    if (world_object != 0) {
        switch (type) {
        case 4: {
            if (value->scalar < 0.0) {
                value->scalar = 0.0;
            }
            if (1.0 < value->scalar) {
                value->scalar = 1.0;
            }

            if (world_object->animation == 0) {
                __assert("wobj.c", 152, "wobj->aobj");
            }
            joint = (HSD_JObj *)world_object->animation->joint;
            if (joint == 0) {
                __assert("wobj.c", 154, "jp");
            }
            if (joint->display_object == 0) {
                __assert("wobj.c", 155, "jp->u.spline");
            }

            splArcLengthPoint(&position, joint->display_object, value->scalar);
            HSD_WObjSetPosition(world_object, &position);
            world_object->flags |= WOBJ_PATH_LOCAL;
        } break;

        case 5:
            HSD_WObjSetPositionX(world_object, value->scalar);
            break;

        case 6:
            HSD_WObjSetPositionY(world_object, value->scalar);
            break;

        case 7:
            HSD_WObjSetPositionZ(world_object, value->scalar);
            break;
        }
    }
}

void HSD_WObjInterpretAnim(HSD_WObj *object)
{
    if (object != 0) {
        HSD_AObjInterpretAnim(object->animation, object, object->class_info->update);
        HSD_RObjAnimAll(object->render_object);
    }
}

s32 WObjLoad(HSD_WObj *object, HSD_WObjDesc *description)
{
    HSD_WObjSetPosition(object, &description->position);
    if (object->render_object != 0) {
        HSD_RObjRemoveAll(object->render_object);
    }
    object->render_object = HSD_RObjLoadDesc(description->render_description);
    HSD_RObjResolveRefsAll(object->render_object, description->render_description);
    return 0;
}

void HSD_WObjInit(HSD_WObj *object, HSD_WObjDesc *description)
{
    if (object == 0 || description == 0) {
        return;
    }

    HSD_WObjSetPosition(object, &description->position);
    if (object->render_object != 0) {
        HSD_RObjRemoveAll(object->render_object);
    }
    object->render_object = HSD_RObjLoadDesc(description->render_description);
    HSD_RObjResolveRefsAll(object->render_object, description->render_description);
}

HSD_WObj *HSD_WObjLoadDesc(HSD_WObjDesc *description)
{
    if (description != 0) {
        HSD_WObj *world_object;
        HSD_ClassInfo *info;
        if (description->class_name == 0 ||
            !(info = hsdSearchClassInfo(description->class_name))) {
            world_object = HSD_WObjAlloc();
        } else {
            world_object = hsdNew(info);
            if (world_object == 0) {
                __assert("wobj.c", 260, "wobj");
            }
        }
        world_object->class_info->load(world_object, description);
        return world_object;
    }
    return 0;
}

/* Retain the SDK's unused class assertion. */
#pragma push
#pragma force_active on
static char world_class_check[] = "hsdIsDescendantOf(info, &hsdWObj)";
#pragma pop

void HSD_WObjSetPosition(HSD_WObj *object, Vec3 *position)
{
    if (object == 0 || position == 0) {
        return;
    }

    object->position = *position;
    object->flags |= WOBJ_POSITION_DIRTY;
    object->flags &= ~WOBJ_PATH_LOCAL;
}

void HSD_WObjSetPositionX(HSD_WObj *object, f32 value)
{
    HSD_JObj *joint;

    if (object != 0) {
        if ((object->flags & WOBJ_PATH_LOCAL) != 0) {
            if (object->animation != 0 && object->animation->joint != 0) {
                joint = (HSD_JObj *)object->animation->joint;
                setup_matrix((HSD_JObj *)object->animation->joint);
                PSMTXMultVec(joint->matrix, &object->position, &object->position);
            }
            object->flags &= ~WOBJ_PATH_LOCAL;
        }
        object->position.x = value;
        object->flags |= WOBJ_POSITION_DIRTY;
    }
}

void HSD_WObjSetPositionY(HSD_WObj *object, f32 value)
{
    HSD_JObj *joint;

    if (object != 0) {
        if ((object->flags & WOBJ_PATH_LOCAL) != 0) {
            if (object->animation != 0 && object->animation->joint != 0) {
                joint = (HSD_JObj *)object->animation->joint;
                setup_matrix((HSD_JObj *)object->animation->joint);
                PSMTXMultVec(joint->matrix, &object->position, &object->position);
            }
            object->flags &= ~WOBJ_PATH_LOCAL;
        }
        object->position.y = value;
        object->flags |= WOBJ_POSITION_DIRTY;
    }
}

void HSD_WObjSetPositionZ(HSD_WObj *object, f32 value)
{
    HSD_JObj *joint;

    if (object != 0) {
        if ((object->flags & WOBJ_PATH_LOCAL) != 0) {
            if (object->animation != 0 && object->animation->joint != 0) {
                joint = (HSD_JObj *)object->animation->joint;
                setup_matrix((HSD_JObj *)object->animation->joint);
                PSMTXMultVec(joint->matrix, &object->position, &object->position);
            }
            object->flags &= ~WOBJ_PATH_LOCAL;
        }
        object->position.z = value;
        object->flags |= WOBJ_POSITION_DIRTY;
    }
}

void HSD_WObjGetPosition(HSD_WObj *object, Vec3 *position)
{
    HSD_JObj *joint;

    if (object == 0 || position == 0) {
        return;
    }
    if ((object->flags & WOBJ_PATH_LOCAL) != 0) {
        if (object->animation != 0 && object->animation->joint != 0) {
            joint = (HSD_JObj *)object->animation->joint;
            setup_matrix((HSD_JObj *)object->animation->joint);
            PSMTXMultVec(joint->matrix, &object->position, &object->position);
        }
        object->flags &= ~WOBJ_PATH_LOCAL;
    }
    *position = object->position;
}

void WObjRelease(HSD_Class *object)
{
    HSD_WObj *world_object = (HSD_WObj *)object;

    HSD_RObjRemoveAll(world_object->render_object);
    HSD_AObjRemove(world_object->animation);
    hsdWObj.parent.head.parent->release(object);
}

void WObjAmnesia(HSD_ClassInfo *info)
{
    if (info == default_class) {
        default_class = 0;
    }
    hsdWObj.parent.head.parent->forget(info);
}

void WObjInfoInit(void)
{
    hsdInitClassInfo(&hsdWObj.parent, &hsdObj, "sysdolphin_base_library", "had_wobj",
                     sizeof(HSD_WObjInfo), sizeof(HSD_WObj));
    hsdWObj.parent.release = WObjRelease;
    hsdWObj.parent.forget = WObjAmnesia;
    hsdWObj.load = WObjLoad;
    hsdWObj.update = WObjUpdateFunc;
}
