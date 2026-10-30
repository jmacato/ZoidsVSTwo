#include "jobj.h"
#include "model_archive.h"

void sAddAnimAll(HSD_JObj *joint, ModelAnimationTable *table, s32 selected)
{
    if (joint != (void *)0 && table != (void *)0) {
        HSD_MatAnimJoint **materials;
        HSD_ShapeAnimJoint **shapes;
        HSD_AnimJoint **animations;
        s32 i = 0;
        animations = table->models[0]->joint_animations;
        materials = table->models[0]->material_animations;
        shapes = table->models[0]->shape_animations;
        for (; i < selected; i++) {
            if (animations != 0 && animations[selected] == (void *)0)
                break;
            if (materials != 0 && materials[selected] == (void *)0)
                break;
            if (shapes != 0 && shapes[selected] == (void *)0)
                break;
        }
        selected = i;
        HSD_JObjAddAnimAll(joint, animations != 0 ? animations[selected] : 0,
                           materials != 0 ? materials[selected] : 0,
                           shapes != 0 ? shapes[selected] : 0);
    }
}
