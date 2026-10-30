#ifndef ZOIDS_RANDOM_H
#define ZOIDS_RANDOM_H
#include "types.h"
extern u32 __hsd_rand_seed_default;
extern u32 *__hsd_rand_seed_hold;
extern s32 HSD_Rand(void);
extern f32 HSD_Randf(void);
extern s32 HSD_Randi(s32 maximum);
extern void _HSD_RandForgetMemory(void *low, void *high);
#endif
