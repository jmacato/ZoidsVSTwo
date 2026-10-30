#ifndef ZOIDS_GAME_SAVE_H
#define ZOIDS_GAME_SAVE_H

#include "types.h"

typedef struct GameSaveRecord {
    s32 has_data;
    s16 owned_zoids[30];
    u8 owned_zoid_count;
    u8 reserved_0041;
    s16 owned_parts[450];
    s16 owned_part_count;
    s16 list_parts[450];
    s16 list_flag[450];
    s16 list_count;
    s16 field_0AD2[30];
    s16 zoid_weapons[30][28];
    s16 contract_options[30];
    s16 contract_subparts[30][3];
    u8 zoid_colors[30];
    s16 selected_zoid;
    s16 selected_zoid_slot;
    s16 previous_zoid_slot;
    s16 partner_zoid;
    s16 partner_zoid_slot;
    s16 previous_partner_zoid_slot;
    s16 selected_person;
    s16 partner_person;
    s16 selected_person_index;
    s16 partner_person_index;
    u8 reserved_12C0[8];
    s8 have_person[58];
    u8 field_1302;
    u8 field_1303;
    s16 have_zoid[100];
    s8 have_zoid_count;
    s8 have_zoid_flag[100];
    u8 field_1431;
    u8 reserved_1432;
    s8 person_list[58];
    s8 person_list_count;
    s8 shop_flags[450];
    s16 parts_num;
    u8 system_flags[0x32];
    s32 money;
    s16 map_no;
    s8 single_team;
    u8 field_166B;
    s8 stage;
    s8 field_166D;
    s8 field_166E;
    u8 reserved_166F;
    s8 game_type;
    u8 win;
    s8 country;
    s8 field_1673;
    s8 field_1674;
    u8 reserved_1675;
    s16 save_year;
    s8 save_month;
    s8 save_day;
    s8 save_hour;
    s8 save_minute;
    s8 save_second;
    u8 reserved_167D[3];
    s32 play_time;
    u8 vol_bgm;
    u8 vol_se;
    u8 vol_voice;
    u8 rumble;
    u8 vs_time;
    u8 vs_mode;
    u8 reserved_168A[2];
    /* sum of bytes 0x0000..0x1689 */
    u8 checksum;
    /* "Tears" */
    char signature[6];
    u8 reserved_1693;
} GameSaveRecord;

typedef char GameSaveRecordSizeCheck[sizeof(GameSaveRecord) == 0x1694 ? 1 : -1];
extern GameSaveRecord IveSave[9];

#endif
