#include "missile.h"

#define NULL ((void *)0)

extern void sLinier3d(f32 *current, f32 *target, f32 fraction);
extern void sGlobal2Local(HSD_JObj *joint);

void sEffectTrace(Effect *effect)
{
    s32 i;
    s32 n;
    s32 type;
    f32 rate;
    HSD_JObj *previous;
    HSD_JObj *joint;
    f32 *last;
    f32 *current;

    if (effect->source == NULL)
        return;
    if (effect->joints[0].joint == NULL)
        return;
    type = effect->type;
    if (type <= 0)
        return;
    rate = Tea_effectdat[type].trace_rate / 100.0;
    for (n = 0; n < 3; n++)
        effect->joints[0].position[n] = effect->joints[0].joint->matrix[n][3];
    for (i = 17; i > 0; i--) {
        previous = effect->joints[i - 1].joint;
        joint = effect->joints[i].joint;
        if (previous != NULL && joint != NULL) {
            last = effect->joints[i - 1].position;
            current = effect->joints[i].position;
            sLinier3d(current, last, rate);
        }
    }
    for (i = 1; i < 18; i++) {
        previous = effect->joints[i - 1].joint;
        joint = effect->joints[i].joint;
        if (previous != NULL && joint != NULL) {
            last = effect->joints[i - 1].position;
            current = effect->joints[i].position;
            for (n = 0; n < 3; n++) {
                if (last[n] != current[n])
                    break;
            }
            if (n == 3)
                current[0] += 0.01;
            for (n = 0; n < 3; n++) {
                joint->matrix[n][3] = current[n];
            }
            sGlobal2Local(joint);
        }
    }
}
