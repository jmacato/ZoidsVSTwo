#ifndef ZOIDS_GAME_CAMERA_H
#define ZOIDS_GAME_CAMERA_H

#include "types.h"

enum CameraMode {
    CAMERA_MODE_NORMAL = 1,
    CAMERA_MODE_HALF = 2
};

/* sCameraIn2 result bits, screen y grows downward */
enum CameraScreenFlag {
    CAMERA_OUT_TOP = 1,
    CAMERA_OUT_LEFT = 2,
    CAMERA_OUT_BOTTOM = 4,
    CAMERA_OUT_RIGHT = 8,
    CAMERA_IN_CENTER = 0x10
};

typedef struct CameraRange {
    f32 heading;
    f32 radius;
    f32 height;
    s32 visible;
    s32 target_visible;
    Vec3 origin;
    u32 score;
} CameraRange;

typedef struct CameraState {
    s32 mode;
    s32 half;
    struct HSD_LObj *lights;
    struct HSD_Shadow *shadow;
    f32 focus_heading;
    u8 reserved_014[4];
    f32 field_018;
    f32 field_01C;
    u8 reserved_020[4];
    f32 focus_elevation;
    u8 reserved_028[4];
    f32 focus_distance;
    s32 mount_index;
    s32 mount_timer;
    u8 reserved_038[0x14];
    s8 mount_hidden[20];
    s8 mount_blocked[20];
    Vec3 mount_positions[20];
    s32 field_164;
    u8 reserved_168[4];
    s32 linier_timer;
    struct HSD_JObj *field_170;
    struct ZoidState *zoid;
    s32 target_index;
    struct PadState *pad;
    f32 focus_y_offset;
    Vec3 relative_origin;
    Vec3 focus;
    Vec3 field_19C;
    Vec3 eye;
    Vec3 interest;
    Vec3 saved_eye;
    Vec3 saved_interest;
    struct HSD_CObj *cobj;
    Vec3 direction;
    f32 planar[2];
    f32 planar_length;
    f32 yaw;
    f32 projection[7];
    f32 viewport[6];
    f32 view[3][4];
    f32 inverse_view[3][4];
    s8 substick_held;
    u8 reserved_28D[3];
    f32 zoid_distance[16];
    CameraRange range[16];
    s32 range_index;
    s32 target_marker_count;
    s32 range_timer;
} CameraState;

typedef char CameraStateSizeCheck[sizeof(CameraState) == 0x51C ? 1 : -1];

extern CameraState sCamera[4];
extern struct HSD_CObj *sCamNormal;
extern s32 sCamNN;
extern s32 sCamNS;

void sCameraInit(void);
void sCameraReset(CameraState *camera);
void sCameraPurge(void);
void sCameraUn(void);
void sCameraSetNormal(CameraState *camera);
void sCameraSetHalf(CameraState *camera, s32 first);
void sCameraSetHalf2(CameraState *camera, s32 first);
void sCameraTargetZoid(CameraState *camera, struct ZoidState *zoid);
void sCameraLenZoid(CameraState *camera);
void camPara(CameraState *camera);
void camCockpit(CameraState *camera, struct ZoidState *zoid);
void sCameraRela(CameraState *camera, Vec3 *position, Vec3 *relative);
s32 sCameraIn2(CameraState *camera, struct HSD_JObj *joint);
s32 sCameraPt2d(CameraState *camera, Vec3 *position, s32 *screen_point);
s32 sCameraJobjPt2d(CameraState *camera, struct HSD_JObj *joint, s32 *screen_point);
s32 camZdLimit(CameraState *camera, struct ZoidState *zoid);
s32 camZdLimit2(CameraState *camera, struct ZoidState *zoid);
void setCamTgtLockON(CameraState *camera);
s32 linierCamLimitXZ(CameraState *camera, f32 radius);

#endif
