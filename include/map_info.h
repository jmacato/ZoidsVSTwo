#ifndef ZOIDS_MAP_INFO_H
#define ZOIDS_MAP_INFO_H

#include "types.h"

enum MapUnitKind {
    MAP_UNIT_MAP = 0,
    MAP_UNIT_ZOID = 1,
    MAP_UNIT_PLAYER = 3,
    MAP_UNIT_PARTNER = 4
};

typedef struct MapInfoUnit {
    char name[0x20];
    s32 kind;
    char picture[0x28];
    char text[0x64];
    s32 x;
    s32 y;
    s32 z;
    s8 enabled;
    u8 reserved_BD[3];
    s32 zoid; /* first member of the ZoidInfo record that infoLoadZoid0/1 read (infoLoadZoid_group.c) */
    u8 reserved_C4[0x4C];
    s8 chara;
    u8 field_111;
    u8 reserved_112[0x13A];
} MapInfoUnit;

/* values of MapInfo.clear_condition tested by calcEndMission; 1 and 4 behave like DESTROY_ALL */
enum MissionClear {
    MISSION_CLEAR_DESTROY_ALL = 0,
    MISSION_CLEAR_TARGET_HEALTH = 2,
    MISSION_CLEAR_AREA = 3,
    MISSION_CLEAR_KILLS = 99
};

/* menu/map_info_dat/B%03d.mg and M%03d.mg, loaded by rdMapInfo */
typedef struct MapInfo {
    char farm[0x20];
    u8 reserved_0020[8];
    char title[0x64];
    char victory_text[0x50];
    char defeat_text[0x50];
    MapInfoUnit units[17];
    u8 reserved_2838[0x24C];
    char label[0x20];
    s32 clear_condition;
    u8 reserved_2AA8[4];
    s32 new_zoids[2];
    s32 new_parts[8];
    s32 map_no;
    u8 reserved_2AD8[0x5A4];
    s16 target_type;
    s16 target_health;
    s16 required_kills;
    s16 area_x;
    s16 area_z;
    s16 area_radius;
    u8 reserved_3088[0x190];
} MapInfo;

typedef char MapInfoUnitSizeCheck[sizeof(MapInfoUnit) == 0x24C ? 1 : -1];
typedef char MapInfoSizeCheck[sizeof(MapInfo) == 12824 ? 1 : -1];

#endif
