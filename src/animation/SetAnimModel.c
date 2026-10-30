#include "jobj.h"
#include "model_archive.h"

extern void setAnimRate(HSD_JObj *joint, f32 rate);

#pragma dont_inline on
void SetAnimModel(ModelAnimation *model, HSD_JObj *joint, s32 selected)
{
    HSD_AnimJoint *animation = 0;
    HSD_MatAnimJoint *material = 0;
    HSD_ShapeAnimJoint *shape = 0;
    s32 i;
    HSD_AnimJoint **joint_list = model->joint_animations;
    HSD_MatAnimJoint **material_list = model->material_animations;
    HSD_ShapeAnimJoint **shape_list = model->shape_animations;

    if (joint_list != 0) {
        for (i = 0; i < selected + 1; i++) {
            animation = joint_list[i];
            if (animation == (void *)0)
                break;
        }
    }
    if (material_list != 0) {
        for (i = 0; i < selected + 1; i++) {
            material = material_list[i];
            if (material == (void *)0)
                break;
        }
    }
    if (shape_list != 0) {
        for (i = 0; i < selected + 1; i++) {
            shape = shape_list[i];
            if (shape == (void *)0)
                break;
        }
    }
    HSD_JObjAddAnimAll(joint, animation, material, shape);
    HSD_JObjReqAnimAll(joint, 0.0F);
    setAnimRate(joint, 1.0F);
}
