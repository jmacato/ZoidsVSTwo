/* Types from
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/psstructs.h. */
#ifndef ZOIDS_PARTICLE_H
#define ZOIDS_PARTICLE_H
#include "types.h"
#include "gx_tev.h"
#include "joint.h"
typedef struct HSD_Particle HSD_Particle;
typedef struct HSD_Generator HSD_Generator;
typedef struct HSD_psAppSRT HSD_psAppSRT;
/* HSD_Particle.kind and HSD_Generator.kind bits */
#define ApplyGrav (1 << 0)
#define ApplyFric (1 << 1)
#define Tornado (1 << 2)
#define TexEdge (1 << 3)
#define ComTLUT (1 << 4)
#define MirrorS (1 << 5)
#define MirrorT (1 << 6)
#define PrimEnv (1 << 7)
#define TexInterpNear (1 << 9)
#define DispTexture (1 << 10)
#define TexFlipS (1 << 18)
#define TexFlipT (1 << 19)
#define Trail (1 << 20)
#define DirVec (1 << 21)
#define DispFog (1 << 24)
#define DispPoint (1 << 30)
#define DispLighting (1 << 31)
/* HSD_Generator.type */
#define PS_GEN_TYPE_MASK 0xF
#define PS_GEN_DISC 0
#define PS_GEN_LINE 1
#define PS_GEN_TORNADO 2
#define PS_GEN_DISC_3 3
#define PS_GEN_DISC_4 4
#define PS_GEN_RECT 5
#define PS_GEN_CONE 6
#define PS_GEN_CYLINDER 7
#define PS_GEN_SPHERE 8
#define PS_GEN_APPSRT 0x100
#define PS_GEN_JOBJ 0x200
struct HSD_Particle {
    HSD_Particle *next;
    u32 kind;
    u8 bank;
    u8 texGroup;
    u8 poseNum;
    u8 palNum;
    u16 sizeCount;
    u16 primColCount;
    u16 envColCount;
    GXColor primCol;
    GXColor envCol;
    u16 cmdWait;
    u8 loopCount;
    u8 linkNo;
    u16 idnum;
    u8 *cmdList;
    u16 cmdPtr;
    u16 cmdMarkPtr;
    u16 cmdLoopPtr;
    u16 life;
    Vec3 vel;
    float grav;
    float fric;
    Vec3 pos;
    float size;
    float rotate;
    u16 aCmpCount;
    u8 aCmpMode;
    u8 aCmpParam1;
    u8 aCmpParam2;
    u8 pJObjOfs;
    u16 matColCount;
    u16 ambColCount;
    u16 rotateCount;
    float sizeTarget;
    float rotateTarget;
    u16 primColRemain;
    u16 envColRemain;
    GXColor primColTarget;
    GXColor envColTarget;
    u16 matColRemain;
    u16 ambColRemain;
    u16 aCmpRemain;
    u8 aCmpParam1Target;
    u8 aCmpParam2Target;
    u8 matRGB;
    u8 matA;
    u8 ambRGB;
    u8 ambA;
    u8 matRGBTarget;
    u8 matATarget;
    u8 ambRGBTarget;
    u8 ambATarget;
    float trail;
    struct HSD_Generator *gen;
    struct HSD_psAppSRT *appsrt;
    float *userdata;
    int (*callback)(HSD_Particle *part);
};

typedef struct _PSUserFunc {
    int (*hookCreate)(HSD_Particle *part);
    int (*hookDelete)(HSD_Particle *part);
    int (*setUserData)(HSD_Particle *part, u8 unknown1, float unknown2);
} HSD_PSUserFunc;

typedef struct _auxDisc {
    f32 minAngle;
    f32 maxAngle;
} auxDisc;

typedef struct _auxLine {
    f32 x2;
    f32 y2;
    f32 z2;
} auxLine;

typedef struct _auxTornado {
    f32 vel;
} auxTornado;

typedef struct _auxRect {
    f32 x;
    f32 y;
    f32 z;
    f32 xx;
    f32 xy;
    f32 xz;
    f32 yx;
    f32 yy;
    f32 yz;
    f32 zx;
    f32 zy;
    f32 zz;
    u16 flag;
} auxRect;

typedef struct _auxCone {
    f32 minAngle;
    f32 maxAngle;
    f32 height;
} auxCone;

typedef struct _auxSphere {
    f32 speed;
    f32 latMid;
    f32 latRange;
    f32 lonMid;
    f32 lonRange;
} auxSphere;

struct HSD_Generator {
    HSD_Generator *next;
    u32 kind;
    f32 random;
    f32 count;
    HSD_JObj *jobj;
    u16 genLife;
    u16 type;
    u8 bank;
    u8 linkNo;
    u8 texGroup;
    u8 dummy;
    u16 idnum;
    u16 life;
    u8 *cmdList;
    Vec3 pos;
    Vec3 vel;
    f32 grav;
    f32 fric;
    f32 size;
    f32 radius;
    f32 angle;
    u32 numChild;
    HSD_psAppSRT *appsrt;
    HSD_PSUserFunc *userfunc;
    int (*callback)(HSD_Generator *part);
    union {
        auxDisc disc;
        auxLine line;
        auxTornado tornado;
        auxRect rect;
        auxCone cone;
        auxSphere sphere;
    } aux;
};

typedef struct HSD_PSTexGroup {
    u32 num;
    u32 fmt;
    u32 tlutfmt;
    u32 width;
    u32 height;
    u16 palnum;
    u16 palflag;
    u8 *texTable[1];
} HSD_PSTexGroup;
typedef struct HSD_PSFormGroup {
    u32 num;
    u8 *formTable[1];
} HSD_PSFormGroup;
typedef struct HSD_PSCmdList {
    u16 type;
    u16 texGroup;
    u16 genLife;
    u16 life;
    u32 kind;
    f32 grav;
    f32 fric;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 radius;
    f32 angle;
    f32 random;
    f32 size;
    f32 param1;
    f32 param2;
    f32 param3;
    u8 cmdList[1];
} HSD_PSCmdList;
extern u32 *ptclref[64];
extern s32 psNumTexGroup[64];
extern HSD_PSTexGroup **psTexGroupArray[64];
extern HSD_PSFormGroup **psFormGroupArray[64];
extern s32 psNumCmdList[64];
extern HSD_PSCmdList **psCmdListArray[64];
extern HSD_JObj *psPointJObj[8];
extern void psInitDataBankLoad(s32 bank, const s32 *commands, const s32 *textures,
                               const u32 *references, const s32 *forms);
extern void psInitDataBankLocate(s32 *commands, s32 *textures, s32 *forms);
extern void psInitDataBank(s32 bank, s32 *commands, s32 *textures, u32 *references,
                           s32 *forms);
extern void psInitParticle(s32 count);
extern void psRemoveParticle(void);
extern HSD_Particle *psGenerateParticle0(HSD_Particle **head, int link, int bank,
                                         u32 kind, u16 texture_group, u8 *commands,
                                         int life, int palette_flag, f32 x, f32 y,
                                         f32 z, f32 vx, f32 vy, f32 vz, f32 size,
                                         f32 gravity, f32 friction,
                                         HSD_Generator *generator, int interpret);
extern HSD_Particle *psGenerateParticleID0(HSD_Particle **head, int link, int bank,
                                           int command, int interpret);
extern HSD_Particle *psGenerateParticle(int link, int bank, u32 kind, u16 texture_group,
                                        u8 *commands, int life, int palette_flag, f32 x,
                                        f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 size,
                                        f32 gravity, f32 friction,
                                        HSD_Generator *generator);
extern void psKillParticle(HSD_Particle *particle);
extern void psKillAllParticle(void);
extern HSD_Particle *psInterpretParticle0(HSD_Particle *particle,
                                         HSD_Particle *previous);
extern void psInterpretParticles(u32 mask);
extern void psSetPointJObj(int index, HSD_JObj *joint);
extern void psSetPointJObjNodup(HSD_JObj *joint, int index);
extern void psClearPointJObj(void);
extern void psDeletePntJObjwithParticle(HSD_Particle *particle);
extern int _psLinkInit(int count);
extern void _psListClear(void);
extern HSD_Particle *_psListNew(HSD_Particle **head, int link);
extern void _psListDelete(HSD_Particle *particle, HSD_Particle *previous);
extern HSD_Particle *_psListGetFirst(int link);
extern HSD_Particle *particleSort(int link, HSD_Particle **opaque,
                                  HSD_Particle **translucent);
extern u16 psGetNewIDNum(void);
extern void psInitGenerator(int count);
extern void psRemoveGenerator(void);
extern HSD_Generator *psKillGeneratorSub(HSD_Generator *generator,
                                         HSD_Generator *previous);
extern void psKillGenerator(HSD_Generator *generator);
extern void psKillAllGenerator(void);
extern void psKillJObjAllGenerator(HSD_JObj *joint);
extern HSD_Generator *psCreateGenerator(void);
extern HSD_Generator *psCreateGeneratorID(s32 link, s32 bank, s32 command);
extern HSD_Generator *psCreateGeneratorIDJObjD(s32 link, s32 bank, s32 command,
                                             HSD_JObj *joint);
extern HSD_Generator *psCreateGeneratorIDJObjS(s32 link, s32 bank, s32 command,
                                             HSD_JObj *joint);
extern void psExecGenerator(u32 mask);
struct HSD_CObj;
extern void psSetBillboardCamera(struct HSD_CObj *camera);
extern void psSetupTevCommon(void);
extern void psSetupTevInvalidState(void);
extern void psSetupTev(HSD_Particle *particle);
#endif
