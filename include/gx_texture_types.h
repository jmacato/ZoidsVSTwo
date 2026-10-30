/* Types follow
 * https://github.com/doldecomp/dolsdk2001/blob/main/include/dolphin/gx/GXEnum.h. */
#ifndef ZOIDS_GX_TEXTURE_TYPES_H
#define ZOIDS_GX_TEXTURE_TYPES_H
#include "gx_tev.h"
#define _GX_TF_CTF 0x20
#define _GX_TF_ZTF 0x10
typedef u8 GXBool;
typedef enum GXTexFmt {
    GX_TF_I4 = 0x0,
    GX_TF_I8 = 0x1,
    GX_TF_IA4 = 0x2,
    GX_TF_IA8 = 0x3,
    GX_TF_RGB565 = 0x4,
    GX_TF_RGB5A3 = 0x5,
    GX_TF_RGBA8 = 0x6,
    GX_TF_CMPR = 0xE,

    GX_CTF_R4 = 0x0 | _GX_TF_CTF,
    GX_CTF_RA4 = 0x2 | _GX_TF_CTF,
    GX_CTF_RA8 = 0x3 | _GX_TF_CTF,
    GX_CTF_YUVA8 = 0x6 | _GX_TF_CTF,
    GX_CTF_A8 = 0x7 | _GX_TF_CTF,
    GX_CTF_R8 = 0x8 | _GX_TF_CTF,
    GX_CTF_G8 = 0x9 | _GX_TF_CTF,
    GX_CTF_B8 = 0xA | _GX_TF_CTF,
    GX_CTF_RG8 = 0xB | _GX_TF_CTF,
    GX_CTF_GB8 = 0xC | _GX_TF_CTF,

    GX_TF_Z8 = 0x1 | _GX_TF_ZTF,
    GX_TF_Z16 = 0x3 | _GX_TF_ZTF,
    GX_TF_Z24X8 = 0x6 | _GX_TF_ZTF,

    GX_CTF_Z4 = 0x0 | _GX_TF_ZTF | _GX_TF_CTF,
    GX_CTF_Z8M = 0x9 | _GX_TF_ZTF | _GX_TF_CTF,
    GX_CTF_Z8L = 0xA | _GX_TF_ZTF | _GX_TF_CTF,
    GX_CTF_Z16L = 0xC | _GX_TF_ZTF | _GX_TF_CTF,

    GX_TF_A8 = GX_CTF_A8,
} GXTexFmt;
typedef enum _GXCITexFmt {
    GX_TF_C4 = 0x8,
    GX_TF_C8 = 0x9,
    GX_TF_C14X2 = 0xA,
} GXCITexFmt;
typedef s32 GXTexWrapMode;
enum {
    GX_CLAMP,
    GX_REPEAT,
    GX_MIRROR,
    GX_MAX_TEXWRAPMODE,
};
typedef s32 GXTexFilter;
enum {
    GX_NEAR,
    GX_LINEAR,
    GX_NEAR_MIP_NEAR,
    GX_LIN_MIP_NEAR,
    GX_NEAR_MIP_LIN,
    GX_LIN_MIP_LIN,
};
typedef s32 GXAnisotropy;
enum {
    GX_ANISO_1,
    GX_ANISO_2,
    GX_ANISO_4,
    GX_MAX_ANISOTROPY,
};
typedef s32 GXTlutFmt;
enum {
    GX_TL_IA8,
    GX_TL_RGB565,
    GX_TL_RGB5A3,
    GX_MAX_TLUTFMT,
};
typedef enum _GXTexCacheSize {
    GX_TEXCACHE_32K,
    GX_TEXCACHE_128K,
    GX_TEXCACHE_512K,
    GX_TEXCACHE_NONE,
} GXTexCacheSize;
typedef enum _GXTlutSize {
    GX_TLUT_16 = 1,
    GX_TLUT_32 = 2,
    GX_TLUT_64 = 4,
    GX_TLUT_128 = 8,
    GX_TLUT_256 = 16,
    GX_TLUT_512 = 32,
    GX_TLUT_1K = 64,
    GX_TLUT_2K = 128,
    GX_TLUT_4K = 256,
    GX_TLUT_8K = 512,
    GX_TLUT_16K = 1024,
} GXTlutSize;
typedef struct GXTexObj {
    u32 dummy[8];
} GXTexObj;
typedef struct GXTexRegion {
    u32 dummy[4];
} GXTexRegion;
typedef struct GXTlutObj {
    u32 dummy[3];
} GXTlutObj;
typedef struct GXTlutRegion {
    u32 dummy[4];
} GXTlutRegion;
typedef GXTexRegion *(*GXTexRegionCallback)(GXTexObj *, GXTexMapID);
typedef GXTlutRegion *(*GXTlutRegionCallback)(u32);
#endif
