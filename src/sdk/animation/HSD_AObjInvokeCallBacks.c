#include "aobj.h"

extern s32 nb_deadanim;
extern s32 nb_activeanim;
extern AnimationEndCallback *endcallback_list;

void HSD_AObjInvokeCallBacks(void)
{
    AnimationEndCallback *entry;

    if (nb_deadanim != 0 && nb_activeanim == 0) {
        for (entry = endcallback_list; entry != 0; entry = entry->next) {
            void (*callback)(void) = entry->callback;

            callback();
        }
    }
}
