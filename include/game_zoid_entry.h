#ifndef ZOIDS_GAME_ZOID_ENTRY_H
#define ZOIDS_GAME_ZOID_ENTRY_H

#include "types.h"

/* GameZoid[] record: the zoid setup ttl_main fills from the save before a game and ZoidInit1 loads */
typedef struct GameZoidEntry {
    s16 type;
    s16 weapons[28];
    s8 color;
    u8 reserved_3B[0x1D];
    s16 person;
    u8 reserved_5A[2];
    s16 option;
    s16 additional_parts[3];
    s8 field_64;
    s8 field_65;
    u8 reserved_66[2];
} GameZoidEntry;

typedef char GameZoidEntrySizeCheck[sizeof(GameZoidEntry) == 0x68 ? 1 : -1];
extern GameZoidEntry GameZoid[16];

#endif
