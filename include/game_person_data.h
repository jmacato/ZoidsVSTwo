#ifndef ZOIDS_GAME_PERSON_DATA_H
#define ZOIDS_GAME_PERSON_DATA_H

#include "types.h"

/* One record of etc/n_Person.dat. */
typedef struct PersonData {
    char name[0x20];
    s16 melee_power_bonus;
    s16 bullet_power_bonus;
    s16 turn_bonus;
    u8 reserved_26[0x42];
} PersonData;

typedef char PersonDataSizeCheck[sizeof(PersonData) == 0x68 ? 1 : -1];
extern PersonData Tea_persondat[];

#endif
