#ifndef ZOIDS_GAME_MODE_H
#define ZOIDS_GAME_MODE_H

#include "types.h"

/* values of GameModeState.game_type, printed as "type" */
enum GameType {
    GAME_TYPE_BATTLE = 1,
    GAME_TYPE_VS = 2,
    GAME_TYPE_3 = 3,
    GAME_TYPE_MISSION = 4
};

/* results of calcEnd and gameWin, kept in winF; WIN and LOSE are from Zoid[0]'s side */
enum GameResult {
    GAME_RESULT_NONE = 0,
    GAME_RESULT_WIN = 1,
    GAME_RESULT_DRAW = 2,
    GAME_RESULT_LOSE = 3
};

typedef struct GameModeState {
    u8 reserved_00[4];
    s32 game_type;
    s32 win;
    u8 reserved_0C[8];
    s32 map;
    s32 map_variant;
    s32 map_no;
    s32 country;
    s32 gametime;
    s32 bp_earned;
    s32 field_2C;
} GameModeState;

typedef char GameModeStateSizeCheck[sizeof(GameModeState) == 48 ? 1 : -1];
extern GameModeState GameMode;

#endif
