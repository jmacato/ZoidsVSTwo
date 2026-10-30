/* Reference:
 * https://github.com/doldecomp/melee/blob/master/src/sysdolphin/baselib/particle.c. */
#include "particle.h"
extern void OSPanic(const char *file, u32 line, const char *format, ...);
extern s32 _psLinkInit(s32 count);
extern u16 numActiveParticles;
extern u16 numPeakParticles;
extern void (*psCallback)(HSD_Particle *particle);
u32 *ptclref[64];
s32 psNumTexGroup[64];
HSD_PSTexGroup **psTexGroupArray[64];
HSD_PSFormGroup **psFormGroupArray[64];
s32 psNumCmdList[64];
HSD_PSCmdList **psCmdListArray[64];
HSD_JObj *psPointJObj[8];
void psInitDataBankLoad(int bank, const int *cmdBank, const int *texBank,
                        const u32 *ref, const int *formBank)
{
    u16 version;

    if (formBank != 0 && *formBank != *texBank) {
        OSPanic("particle.c", 86, "illigal form data (strange number of group)\n");
    }

    ptclref[bank] = (u32 *)ref;
    psNumTexGroup[bank] = *texBank;
    psTexGroupArray[bank] = (HSD_PSTexGroup **)(texBank + 1);

    if (formBank != 0) {
        psFormGroupArray[bank] = (HSD_PSFormGroup **)(formBank + 1);
    } else {
        psFormGroupArray[bank] = 0;
    }

    version = *(u16 *)cmdBank;
    switch (version) {
    case 0:
        psNumCmdList[bank] = cmdBank[1];
        psCmdListArray[bank] = (HSD_PSCmdList **)(cmdBank + 2);
        break;
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43: {
        s32 count = cmdBank[1];
        psNumCmdList[bank] = cmdBank[2] + count;
        psCmdListArray[bank] = (HSD_PSCmdList **)(cmdBank + 3 - count);
        break;
    }
    default:
        OSPanic("particle.c", 116, "psInitDataBanks: unknown version\n");
    }
}

/* Bank pointers are offsets from their owning blob before relocation. */
void psInitDataBankLocate(int *cmdBank, int *texBank, int *formBank)
{
    s32 num;
    s32 *ptr;
    s32 *group;
    HSD_PSFormGroup *fg;
    s32 j;
    s32 i;
    s32 num2;
    s32 *groups;
    s32 *base;
    s32 version;

    version = *(u16 *)cmdBank;
    switch (version) {
    case 0:
        num2 = ((s32 *)cmdBank)[1];
        base = (s32 *)((u8 *)cmdBank + 8);
        num = 0;
        for (i = 0; i < num2; i++) {
            ((s32 *)cmdBank)[i + 2] += (s32)cmdBank;
        }
        break;
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
        num = ((s32 *)cmdBank)[1];
        num2 = ((s32 *)cmdBank)[2] + num;
        base = (s32 *)cmdBank + 3 - num;
        ptr = (s32 *)cmdBank;
        j = 0;
        while (j < (s32)cmdBank[2]) {
            if (ptr[3] != 0) {
                ptr[3] += (s32)cmdBank;
            }
            ptr++;
            j++;
        }
        break;
    }

    ptr = base + num;
    for (i = num; i < num2; i++) {
        s32 *cmd = (s32 *)ptr[0];
        if (cmd != 0) {
            cmd[2] = cmd[2] & 0xF1FFFFFF;
            cmd = (s32 *)ptr[0];
            cmd[2] = cmd[2] | 0x08000000;
        }
        ptr++;
    }

    {
        s32 num_groups = ((s32 *)texBank)[0];
        s32 k;

        group = groups = (s32 *)texBank + 1;
        for (k = 1; k <= num_groups; k++) {
            if (group[0] != 0) {
                group[0] += (s32)texBank;
            }
            group++;
        }

        {
            group = groups;
            for (k = 0; k < num_groups; group++, k++) {
                HSD_PSTexGroup *tg = (HSD_PSTexGroup *)group[0];
                if (tg == 0) {
                    continue;
                }

                {
                    s32 ti;
                    for (ti = 0; (u32)ti < ((HSD_PSTexGroup *)group[0])->num; ti++) {
                        if (((HSD_PSTexGroup *)group[0])->texTable[ti] != 0) {
                            ((HSD_PSTexGroup *)group[0])->texTable[ti] += (u32)texBank;
                        }
                    }
                    tg = (HSD_PSTexGroup *)group[0];
                }

                {
                    u32 fmt = tg->fmt;
                    if (fmt != 8 && (fmt - 9) > 1) {
                        continue;
                    }
                }

                if (tg->palflag & 1) {

                    i = tg->num;
                    if (tg->texTable[i] != 0) {
                        tg->texTable[i] += (u32)texBank;
                    }
                } else if (tg->palnum != 0) {

                    i = tg->num;
                    for (; (u32)i < ((HSD_PSTexGroup *)group[0])->num +
                                        ((HSD_PSTexGroup *)group[0])->palnum;
                         i++) {
                        HSD_PSTexGroup *tg2 = (HSD_PSTexGroup *)group[0];
                        u8 **entry = &tg2->texTable[i];
                        if (*entry != 0) {
                            *entry += (u32)texBank;
                        }
                    }
                } else {

                    i = tg->num;
                    for (; (u32)i < ((HSD_PSTexGroup *)group[0])->num * 2; i++) {
                        HSD_PSTexGroup *tg2 = (HSD_PSTexGroup *)group[0];
                        u8 **entry = &tg2->texTable[i];
                        if (*entry != 0) {
                            *entry += (u32)texBank;
                        }
                    }
                }
            }
        }

        if (formBank == 0) {
            return;
        }
        {
            for (i = 1; i <= num_groups; i++) {
                if (formBank[i] != 0) {
                    s32 fi;
                    formBank[i] += (s32)formBank;
                    fg = (HSD_PSFormGroup *)formBank[i];
                    for (fi = 0; (u32)fi < fg->num; fi++) {
                        if (fg->formTable[fi] != 0) {
                            fg->formTable[fi] += (u32)formBank;
                        }
                    }
                }
            }
        }
    }
}

void psInitDataBank(int bank, int *cmdBank, int *texBank, u32 *ref, int *formBank)
{
    if (bank < 64) {
        psInitDataBankLocate(cmdBank, texBank, formBank);
        psInitDataBankLoad(bank, cmdBank, texBank, ref, formBank);
    }
}

void psInitParticle(int count)
{
    int i;
    _psLinkInit(count);
    numActiveParticles = 0;
    numPeakParticles = 0;
    for (i = 0; i < 64; i++) {
        psNumCmdList[i] = 0;
        psNumTexGroup[i] = 0;
        psCmdListArray[i] = 0;
        psTexGroupArray[i] = 0;
        psFormGroupArray[i] = 0;
        ptclref[i] = 0;
    }
    psCallback = 0;
    for (i = 0; i < 8; i++)
        psPointJObj[i] = 0;
}
void psRemoveParticle(void)
{
    int i;
    psKillAllParticle();
    psKillAllGenerator();
    _psListClear();
    for (i = 0; i < 64; i++) {
        psNumCmdList[i] = 0;
        psNumTexGroup[i] = 0;
        psCmdListArray[i] = 0;
        psTexGroupArray[i] = 0;
        psFormGroupArray[i] = 0;
        ptclref[i] = 0;
    }
    psClearPointJObj();
}
extern int psRemoveParticleAppSRT(HSD_Particle *particle);
extern int psAttachParticleAppSRT(HSD_Particle *particle, HSD_psAppSRT *transform);
extern HSD_Particle *psInterpretParticle0(HSD_Particle *particle,
                                          HSD_Particle *previous);
HSD_Particle *psGenerateParticle0(HSD_Particle **head, int linkNo, int bank, u32 kind,
                                  u16 texGroup, u8 *list, int life, int palflag, f32 x,
                                  f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 size,
                                  f32 grav, f32 fric, HSD_Generator *gp,
                                  int flgInterpret)
{
    HSD_Particle *pp;
    s16 cmd_wait;

    pp = _psListNew(head, linkNo);
    if (pp == 0) {
        return 0;
    }

    if (gp != 0) {
        pp->idnum = gp->idnum;
    } else {
        pp->idnum = psGetNewIDNum();
    }

    pp->appsrt = 0;
    if (gp != 0 && gp->appsrt != 0) {
        psAttachParticleAppSRT(pp, gp->appsrt);
    }

    pp->bank = bank;
    pp->linkNo = linkNo;
    pp->kind = kind;
    pp->texGroup = texGroup;
    pp->pos.x = x;
    pp->pos.y = y;
    pp->pos.z = z;
    pp->vel.x = vx;
    pp->vel.y = vy;
    pp->vel.z = vz;
    pp->size = size;
    pp->grav = grav;
    pp->fric = fric;
    pp->life = (u16)(life + 1);
    pp->cmdList = list;
    pp->cmdMarkPtr = 0;
    pp->cmdPtr = 0;

    if (palflag != 0) {
        pp->kind = pp->kind | ComTLUT;
    }

    if (list != 0) {
        cmd_wait = 1;
    } else {
        cmd_wait = 0;
    }
    pp->cmdWait = cmd_wait;
    pp->poseNum = 0;
    pp->palNum = 0xFF;
    pp->primCol.a = 0xFF;
    pp->primCol.b = 0xFF;
    pp->primCol.g = 0xFF;
    pp->primCol.r = 0xFF;
    pp->envCol.a = 0;
    pp->envCol.b = 0;
    pp->envCol.g = 0;
    pp->envCol.r = 0;
    pp->envColCount = 0;
    pp->primColCount = 0;
    pp->sizeCount = 0;
    pp->envColRemain = 0;
    pp->primColRemain = 0;
    pp->aCmpMode = 0x33;
    pp->aCmpParam1 = 1;
    pp->aCmpParam2 = 0xFF;
    pp->aCmpRemain = 0;
    pp->aCmpCount = 0;
    pp->rotateCount = 0;
    pp->rotateTarget = 0.0F;
    pp->rotate = 0.0F;
    pp->gen = gp;
    if (gp != 0) {
        gp->numChild = gp->numChild + 1;
    }

    pp->pJObjOfs = 0;
    pp->matColRemain = 0;
    pp->matColCount = 0;
    pp->matRGB = 0xFF;
    pp->matA = 0xFF;
    pp->ambColRemain = 0;
    pp->ambColCount = 0;
    pp->ambRGB = 0xFF;
    pp->ambA = 0xFF;
    pp->trail = 1.0F;
    pp->userdata = 0;

    if (gp != 0) {
        if (gp->userfunc != 0) {
            if (gp->userfunc->hookCreate != 0) {
                gp->userfunc->hookCreate(pp);
            }
        }
    }

    pp->callback = 0;
    if (flgInterpret != 0) {
        psInterpretParticle0(pp, 0);
    }
    return pp;
}

HSD_Particle *psGenerateParticleID0(HSD_Particle **head, int link, int bank,
                                    int command, int interpret)
{
    HSD_PSCmdList *list;
    HSD_PSTexGroup *textures;
    int group;
    int palette_flag;
    if (link >= 8)
        return 0;
    if (bank >= 64)
        return 0;
    if (command >= psNumCmdList[bank])
        return 0;
    list = psCmdListArray[bank][command];
    if (list == 0)
        return 0;
    group = list->texGroup;
    textures = psTexGroupArray[bank][group];
    if (textures != 0)
        palette_flag = textures->palflag;
    else
        palette_flag = 0;
    return psGenerateParticle0(head, link, bank, list->kind, group, list->cmdList,
                               list->life, palette_flag, 0.0F, 0.0F, 0.0F, list->vx,
                               list->vy, list->vz, list->size, list->grav, list->fric,
                               0, interpret);
}
HSD_Particle *psGenerateParticle(int link, int bank, u32 kind, u16 texture_group,
                                 u8 *commands, int life, int palette_flag, f32 x, f32 y,
                                 f32 z, f32 vx, f32 vy, f32 vz, f32 size, f32 gravity,
                                 f32 friction, HSD_Generator *generator)
{
    return psGenerateParticle0(0, link, bank, kind, texture_group, commands, life,
                               palette_flag, x, y, z, vx, vy, vz, size, gravity,
                               friction, generator, 1);
}
void psKillParticle(HSD_Particle *particle)
{
    HSD_Particle *previous = 0;
    HSD_Particle *current = _psListGetFirst(particle->linkNo);
    while (current != 0) {
        if (current == particle) {
            if (particle->gen != 0)
                particle->gen->numChild--;
            if (particle->appsrt != 0)
                psRemoveParticleAppSRT(particle);
            psDeletePntJObjwithParticle(current);
            _psListDelete(current, previous);
            if (particle->gen != 0 && particle->gen->userfunc != 0 &&
                particle->gen->userfunc->hookDelete != 0) {
                particle->gen->userfunc->hookDelete(particle);
            }
            break;
        }
        previous = current;
        current = current->next;
    }
}
void psKillAllParticle(void)
{
    int more_links;
    HSD_Particle *next;
    HSD_Particle *particle;
    int link;
    link = 0;
    do {
        particle = _psListGetFirst(link);
        while (particle != 0) {
            next = particle->next;
            psKillParticle(particle);
            particle = next;
        }
        link++;
        more_links = link < 16;
    } while (more_links);
}
