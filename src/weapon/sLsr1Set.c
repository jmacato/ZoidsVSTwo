#pragma readonly_strings on
#include "types.h"
#include "joint.h"

typedef struct Lsr3 {
    HSD_JObj *source;
    f32 start[3][4];
    f32 end[3][4];
    void *archive;
    s32 effect;
} Lsr3;

typedef struct Lsr1_3 {
    f32 start[3][4];
    f32 end[3][4];
    HSD_JObj *joint;
    HSD_JObj *start_joint;
    HSD_JObj *end_joint;
} Lsr1_3;

extern Lsr1_3 sLsr1[400];
extern HSD_JObj *CopyJoint(void *archive);
extern void LoadArcAnimModel(void *archive, HSD_JObj *joint, s32 model, s32 selected);
extern HSD_JObj *searchJobjJoint(void *archive, HSD_JObj *joint, char *name);
extern void HSD_JObjReqAnimAll(HSD_JObj *joint, f32 frame);
extern void setAnimRate(HSD_JObj *joint, f32 rate);

void sLsr1Set(Lsr3 *lsr)
{
    s32 i;
    s32 r;
    s32 c;
    Lsr1_3 *beam;

    if (lsr == (void *)0)
        return;
    if (lsr->archive == (void *)0)
        return;
    if (lsr->source == (void *)0)
        return;
    for (i = 0; i < 400; i++) {
        beam = &sLsr1[i];
        if (beam->joint == 0) {
            for (r = 0; r < 3; r++) {
                for (c = 0; c < 4; c++) {
                    beam->start[r][c] = lsr->start[r][c];
                    beam->end[r][c] = lsr->end[r][c];
                }
            }
            beam->joint = CopyJoint(lsr->archive);
            LoadArcAnimModel(lsr->archive, beam->joint, 0, 0);
            beam->start_joint = searchJobjJoint(lsr->archive, beam->joint, "start_joint");
            beam->end_joint = searchJobjJoint(lsr->archive, beam->joint, "end_joint");
            HSD_JObjReqAnimAll(beam->joint, 0.0f);
            setAnimRate(beam->joint, 1.0f);
            return;
        }
    }
}
