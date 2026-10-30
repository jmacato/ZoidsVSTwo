#include "cobj.h"
#include "types.h"

typedef struct CameraState3 {
    u8 pad_000[0x214];
    Vec3 eye;
    Vec3 interest;
    u8 pad_22C[0x18];
    HSD_CObj *cobj;
    Vec3 direction;
    f32 planar[2];
    f32 planar_length;
    f32 yaw;
} CameraState3;

extern f64 __frsqrte(f64);
extern f32 atan2f(f32 y, f32 x);

static inline f32 sqrtf(f32 x, volatile f32 *y)
{
    static const f64 _half = 0.5;
    static const f64 _three = 3.0;

    if (x > 0.0f) {
        f64 guess = __frsqrte((f64)x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        guess = _half * guess * (_three - guess * guess * x);
        *y = (f32)(x * guess);
        return *y;
    }
    return x;
}

void sCameraCalc(CameraState3 *camera)
{
    volatile f32 root;
    s32 axis;
    f32 x;
    f32 z;

    if (camera != (void *)0 && camera->cobj != (void *)0) {
        for (axis = 0; axis < 3; axis++)
            (&camera->direction.x)[axis] = (&camera->interest.x)[axis] - (&camera->eye.x)[axis];
        camera->planar[0] = camera->direction.z;
        camera->planar[1] = camera->direction.x;
        camera->planar_length = sqrtf(camera->planar[0] * camera->planar[0] + camera->planar[1] * camera->planar[1], &root);
        if (0.0f == camera->planar_length) {
            camera->planar[0] = 1.0f;
            camera->planar[1] = 0.0f;
        } else {
            camera->planar[0] /= camera->planar_length;
            camera->planar[1] /= camera->planar_length;
        }
        z = camera->direction.z;
        x = camera->direction.x;
        camera->yaw = atan2f(x, z);
    }
}
