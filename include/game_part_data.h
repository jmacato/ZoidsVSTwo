#ifndef ZOIDS_GAME_PART_DATA_H
#define ZOIDS_GAME_PART_DATA_H

#include "types.h"

#define PART_FLAG_CAMOUFLAGE 0x01
#define PART_FLAG_BOOSTER 0x02
#define PART_FLAG_SHIELD 0x04
#define PART_FLAG_MINE 0x10
#define PART_FLAG_SMOKE 0x20
#define PART_FLAG_NO_FOG 0x80

typedef struct PartData {
    char name[0x33];
    char english_name[0x15];
    char model_name[8];
    s32 field_50;
    s32 sell_price;
    u8 reserved_58[4];
    s32 bullets[2];
    u32 flags;
    s16 category;
    s16 armor_bonus;
    s16 power;
    s16 range;
    s16 ammo;
    s16 radar_range_bonus;
    s16 marker_count;
    s16 rate;
    s16 contact_damage;
    u8 reserved_7A[0x3A];
    s16 zoids[12];
    u8 reserved_CC[0x14];
    s16 hit_effect;
    u8 reserved_E2[2];
    s16 field_E4;
    s16 speed_bonus;
    s16 search_range_bonus;
    s16 turn_bonus;
    u8 reserved_EC[0xE];
    s16 icon;
    u8 reserved_FC[0x28];
} PartData;

typedef char PartDataSizeCheck[sizeof(PartData) == 0x124 ? 1 : -1];
extern PartData Tea_partsdat[];

#endif
