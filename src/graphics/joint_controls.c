#include "jobj.h"

#define JOBJ_HIDDEN 0x10
#define JOBJ_INSTANCE 0x1000
#define JOBJ_XLU 0x80000
#define JOBJ_ROOT_XLU 0x20000000
#define RENDER_XLU 0x40000000

extern void psKillJObjAllGenerator(HSD_JObj *joint);
extern void *malloc2(u32 size);
extern void ZeroMemory(void *memory, s32 size);
extern void free(void *memory);

#pragma dont_inline on






void HiddenParts(HSD_JObj *joint, s32 hidden);

void HiddenPartsAll(HSD_JObj *joint, s32 hidden)
{
    if (joint != 0) {
        HiddenParts(joint, hidden);
        for (joint = joint != 0 ? joint->child : 0; joint != 0;
             joint = joint != 0 ? joint->next : 0) {
            HiddenPartsAll(joint, hidden);
        }
    }
}

MaterialPose *makeDobj(void)
{
    MaterialPose *pose = malloc2(sizeof(MaterialPose));
    ZeroMemory(pose, sizeof(MaterialPose));
    return pose;
}

JointPose *makeJobj(void)
{
    JointPose *pose = malloc2(sizeof(JointPose));
    ZeroMemory(pose, sizeof(JointPose));
    return pose;
}












void stStJobjUnref(JointPose *pose)
{
    JointPose *next;
    while (pose != 0) {
        if (pose->child != 0)
            stStJobjUnref(pose->child);
        next = pose->next;
        free(pose);
        pose = next;
    }
}


