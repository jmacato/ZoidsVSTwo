#ifndef ZOIDS_GROUND_COLLISION_H
#define ZOIDS_GROUND_COLLISION_H

#include "types.h"

/* no polygon; also ends each polygon index list */
#define GROUND_POLYGON_NONE 0xFFFF
/* sColPt* result when the segment crosses no polygon */
#define GROUND_NO_HIT 10000.0f
/* sColBallH3 mode bit: push the point out instead of returning the first polygon hit */
#define GROUND_BALL_PUSH_OUT 1

typedef struct GroundPolygon {
    u16 vertex_count;
    u16 material;
    u16 normal_index;
    u16 reserved_06;
    u16 vertices[4];
    Vec3 normal;
    f32 plane_offset;
} GroundPolygon;

/* zoid and missile queries test bits[0] (mask {255, 0, 0, 0}), camera queries bits[1] ({0, 255, 0, 0}) */
typedef struct GroundMask {
    u8 bits[4];
} GroundMask;

/* header of stage/stNN.xb */
typedef struct GroundColHeader {
    char name[0x20];
    u16 polygon_count;
    u16 vertex_count;
    u16 material_count;
    u16 normal_count;
    u16 div_x;
    u16 div_z;
    u8 reserved_2C[4];
    f32 start_x;
    f32 start_z;
    f32 diff_x;
    f32 diff_z;
    u32 polygon_offset;
    u32 vertex_offset;
    u32 material_offset;
    u32 normal_offset;
    /* one u32 per grid cell: h2 entry index where its polygon list starts, or 0xFFFFFFFF */
    u32 d2_offset;
    /* u16 polygon index lists, each ending with GROUND_POLYGON_NONE */
    u32 h2_offset;
    u32 h2_count;
} GroundColHeader;

typedef char GroundPolygonSizeCheck[sizeof(GroundPolygon) == 32 ? 1 : -1];
typedef char GroundMaskSizeCheck[sizeof(GroundMask) == 4 ? 1 : -1];
typedef char GroundColHeaderSizeCheck[sizeof(GroundColHeader) == 0x5C ? 1 : -1];

extern GroundColHeader *grdcCol;
extern s32 grdcPn;
extern s32 grdcVn;
extern s32 grdcMn;
extern s32 grdcNn;
extern s32 grdcDivX;
extern s32 grdcDivZ;
extern f32 grdcStartX;
extern f32 grdcStartZ;
extern f32 grdcDiffX;
extern f32 grdcDiffZ;
extern f32 grdcEndX;
extern f32 grdcEndZ;
extern GroundPolygon *grdcPolygon;
extern Vec3 *grdcVtx;
extern Vec3 *grdcNormal;
extern GroundMask *grdcMat;

extern void sColPrologue(char *name, s32 usb);
extern void sColEpilogue(void);
extern s32 sColPos(Vec3 *point);
extern u16 *sColPosTBL(Vec3 *point);
extern s32 sColPosPolygonON(Vec3 *point, u16 index);
extern f32 sColGetPolyY(u16 index, Vec3 *point);
extern f32 sColPosAndY(Vec3 *point, u16 *selector, GroundMask *mask);
extern f32 sColPosY(Vec3 *point, u16 *selector);
extern f32 sColPt00(Vec3 *start, Vec3 *end, GroundPolygon *polygon, GroundMask *mask);
extern f32 sColPt0R(Vec3 *point, Vec3 *unused, GroundPolygon *polygon, GroundMask mask, f32 radius);
extern f32 sColPt(Vec3 *start, Vec3 *end, GroundMask *mask);
extern f32 sColPtR(Vec3 *point, Vec3 *unused, GroundMask mask, f32 radius);
extern GroundPolygon *sColBallH3(Vec3 *point, Vec3 *weights, GroundMask *mask, s32 mode, f32 radius);
extern GroundPolygon *sColBallYH(Vec3 *point, GroundMask *mask, s32 mode, f32 radius);
extern void sColGetRxRz(Vec3 *normal, f32 heading, f32 *rx, f32 *rz);

#endif
