#include "jobj.h"
#include "model_archive.h"

typedef struct AnimationContext {
    HSD_JObj *joint;
    JointPose *pose;
    HSD_JObjDesc *description;
    HSD_AnimJoint *animation;
    HSD_MatAnimJoint *material;
    HSD_ShapeAnimJoint *shape;
} AnimationContext;

extern s32 sGetAnimJoint(AnimationContext *context, HSD_JObj *target);
extern void HSD_JObjAddAnim(HSD_JObj *joint, HSD_AnimJoint *animation, HSD_MatAnimJoint *material, HSD_ShapeAnimJoint *shape);
extern void HSD_JObjReqAnim(HSD_JObj *joint, f32 frame);
extern void HSD_AObjSetRate(struct HSD_AObj *aobj, f32 rate);

s32 sSetAnim1(ModelArchive *archive, s32 model, HSD_JObj *joint, s32 selected)
{
    AnimationContext context;
    ModelAnimationTable *table;

    if (archive->model_count <= model)
        return 0;
    table = archive->animations;
    if (table->models == (void *)0)
        return 0;
    context.joint = archive->models[model];
    context.description = table->models[model]->joint;
    context.pose = 0;
    context.animation = 0;
    context.material = 0;
    context.shape = 0;
    if (table->models[model]->joint_animations != (void *)0)
        context.animation = table->models[model]->joint_animations[selected];
    if (table->models[model]->material_animations != (void *)0)
        context.material = table->models[model]->material_animations[selected];
    if (table->models[model]->shape_animations != (void *)0)
        context.shape = table->models[model]->shape_animations[selected];
    if (sGetAnimJoint(&context, joint) == 0)
        return 0;
    HSD_JObjAddAnim(joint, context.animation, context.material, context.shape);
    HSD_JObjReqAnim(joint, 0.0f);
    if (joint->animation != 0)
        HSD_AObjSetRate(joint->animation, 1.0f);
    return 1;
}
