#pragma readonly_strings on
#include "game_camera.h"
#define efctFile efctFile_decl
#include "missile.h"
#undef efctFile

extern s32 efctNum;
extern s32 SE_TableList[];
extern s32 gameEndF;
extern CameraState *camTmp;
extern s32 usbForce;
extern void *efctBinB2;
extern s32 sprintf(char *buffer, const char *format, ...);
extern HSD_JObj *CopyJoint(void *archive);
extern void LoadArcAnimModel(void *archive, HSD_JObj *joint, s32 model, s32 selected);
extern HSD_JObj *searchJobjJoint(void *archive, HSD_JObj *joint, char *name);
extern void sLsrSet(HSD_JObj *source, ModelArchive *archive);
extern void sGlobal2Local(HSD_JObj *joint);
extern void sMakeMatrix(HSD_JObj *joint);
extern s32 camSe(CameraState *camera, s16 sound, f32 *point);
extern void HSD_JObjSetMtxDirtySub(HSD_JObj *joint);
extern void __assert(const char *file, s32 line, const char *expr);
extern void *sReadFileU(char *name, s32 force);
extern void *binB2(void *bin, s32 index, s32 *size);
extern ModelArchive *LoadArc2B(void *data, s32 size, EffectDat *dat);
extern void ZeroMemory(void *dst, u32 size);
extern void free(void *ptr);

#define JOBJ_MTX_INDEP_SRT 0x02000000
#define JOBJ_USER_DEF_MTX 0x800000
#define JOBJ_MTX_DIRTY 0x40
#pragma auto_inline off
static inline s32 HSD_JObjMtxIsDirty(HSD_JObj *jobj)
{
    jobj ? (void)0 : __assert("jobj.h", 605, "jobj");
    return !(jobj->flags & JOBJ_USER_DEF_MTX) && (jobj->flags & JOBJ_MTX_DIRTY);
}
#define HSD_JObjSetMtxDirty(jobj) \
    do { \
        if ((jobj) != 0 && !HSD_JObjMtxIsDirty(jobj)) \
            HSD_JObjSetMtxDirtySub(jobj); \
    } while (0)
static inline void HSD_JObjSetTranslateX(HSD_JObj *jobj, f32 value)
{
    jobj ? (void)0 : __assert("jobj.h", 952, "jobj");
    jobj->translation.x = value;
    if (!(jobj->flags & JOBJ_MTX_INDEP_SRT))
        HSD_JObjSetMtxDirty(jobj);
}

static ModelArchive *efctFile[136];
static Effect sEffect[100];

void sEffectPrologueU(s32 force)
{
    s32 i;
    Effect *effect;
    void *data;
    s32 size;

    if (usbForce != 0) {
        force = 1;
    }
    efctBinB2 = sReadFileU("effect/ef.b", force);
    if (efctBinB2 == 0) {
        return;
    }
    for (i = 0; i < 136; i++) {
        data = binB2(efctBinB2, i, &size);
        if (data == 0) {
            continue;
        }
        efctFile[i] = LoadArc2B(data, size, &Tea_effectdat[i]);
        if (efctFile[i] != (void *)0 && efctFile[i]->model_data != 0 && efctFile[i]->models == (void *)0) {
            PurgeArc2(efctFile[i]);
            efctFile[i] = 0;
        }
    }
    for (i = 0; i < 100; i++) {
        effect = &sEffect[i];
        ZeroMemory(effect, sizeof(Effect));
        effect->root = 0;
    }
    sEfctLight = LoadArc2("effect/light.dat");
}

void sEffectEpilogue(void)
{
    Effect *effect;
    s32 i;

    PurgeArc2(sEfctLight);
    sEfctLight = 0;
    for (i = 0; i < 100; i++) {
        effect = &sEffect[i];
        sEffectPurge(effect);
        effect->root = 0;
    }
    for (i = 0; i < 136; i++) {
        if (efctFile[i] != 0) {
            PurgeArc2(efctFile[i]);
        }
        efctFile[i] = 0;
    }
    if (efctBinB2 != 0)
        free(efctBinB2);
    efctBinB2 = 0;
}

void sEffectSet(s32 type, s32 mode, f32 *position, HSD_JObj *source, struct ZoidState *zoid)
{
    HSD_JObj *root;
    ModelArchive *archive;
    HSD_JObj *start;
    HSD_JObj *end;
    Effect *effect;
    s32 i;
    s32 j;
    s32 se;
    s32 sound;
    char name[100];

    if (type <= 0)
        return;
    if (type >= 136)
        return;
    archive = efctFile[type];
    if (archive == (void *)0)
        return;
    if (archive->model_data == (void *)0)
        return;
    start = searchJobjJoint(archive, archive->models[0], "start_joint");
    end = searchJobjJoint(archive, archive->models[0], "end_joint");
    if (end == (void *)0)
        start = 0;
    if (start != 0) {
        sLsrSet(source, archive);
        return;
    }
    efctNum++;
    for (i = 0; i < 100; i++) {
        if (efctNum >= 100)
            efctNum = 0;
        effect = &sEffect[efctNum];
        if (effect->root == (void *)0)
            break;
        efctNum++;
    }
    if (i == 100)
        return;
    root = CopyJoint(archive);
    if (root == (void *)0)
        return;
    effect->a1_joint = searchJobjJoint(archive, archive->models[0], "a_1_joint");
    if (source != 0) {
        for (i = 0; i < 18; i++) {
            sprintf(name, "te_%02d_joint", i);
            effect->joints[i].joint = searchJobjJoint(archive, root, name);
            for (j = 0; j < 3; j++)
                effect->joints[i].position[j] = 0.01 + (source->matrix[j][3] + 0.01 * i);
        }
        for (i = 0; i < 60; i++) {
            for (j = 0; j < 3; j++)
                effect->history[i][j] = 0.01 + (source->matrix[j][3] + 0.01 * i);
        }
    }
    effect->root = root;
    effect->type = type;
    effect->archive = archive;
    effect->mode = mode;
    effect->source = source;
    effect->zoid = zoid;
    if (mode == 0) {
        for (i = 0; i < 3; i++)
            effect->position[i] = position[i];
    }
    if (mode == 1 || mode == 2 || mode == 3) {
        for (i = 0; i < 3; i++) {
            effect->position[i] = source->matrix[i][3];
            for (j = 0; j < 4; j++)
                effect->root->matrix[i][j] = source->matrix[i][j];
        }
        HSD_JObjSetTranslateX(effect->root, effect->position[0]);
        sGlobal2Local(effect->root);
        sMakeMatrix(effect->root);
    }
    LoadArcAnimModel(archive, root, 0, 0);
    sound = Tea_effectdat[type].se;
    if (sound < 0)
        return;
    se = SE_TableList[sound];
    if (se < 0)
        return;
    if (gameEndF == 0) {
        effect->se_voice = camSe(camTmp, sound, effect->position);
        effect->se_playing = 1;
        effect->se = se;
    }
}
