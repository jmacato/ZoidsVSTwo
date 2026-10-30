#ifndef ZOIDS_GAME_ZOID_DATA_H
#define ZOIDS_GAME_ZOID_DATA_H

#include "types.h"

typedef struct ZoidData {
    char name[0x1F];
    char english_name[0x15];
    char model_name[8];
    s32 price;
    s32 sell_price;
    u8 reserved_0044[0xE];
    s16 health;
    s16 armor;
    s16 field_0056;
    u8 reserved_0058[8];
    s16 default_parts[10];
    s16 mount[8];
    s16 option_two;
    s16 option_one;
    s16 super_attack_slot;
    s16 super_attack_part;
    u8 reserved_008C[0xC];
    s16 weapon_permissions[10];
    u8 reserved_00AC[0x2E];
    s16 animation_sounds[175];
    u8 reserved_0238[0x15E];
    s16 animation_effects[175];
    s16 animation_effect_mounts[175];
    s16 attack_effects[12][2];
    s16 attack_effect_mounts[12][2];
    s16 attack_ranges[24];
    s16 attack_angles[24];
    u8 reserved_0712[0x30];
    s16 attack_damage[12];
    s16 attack_pull[12];
    s16 attack_hit_effects[12];
    u8 reserved_078A[4];
    s16 field_078E;
    s16 radar_range;
    s16 search_range;
    u8 reserved_0794[2];
    s16 weapon_points[10];
    s16 mount_parts[10];
    u8 reserved_07BE[0x2A];
} ZoidData;

typedef char ZoidDataSizeCheck[sizeof(ZoidData) == 0x7E8 ? 1 : -1];
extern ZoidData Tea_zoiddat[];

#endif
