/* Reference:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/os/__start.c. */
#include "runtime_string.h"
#include "os_reset.h"
#include "asm_ops.h"
#pragma peephole off
typedef struct RomCopyInfo {
    char *rom;
    char *addr;
    unsigned int size;
} RomCopyInfo;
typedef struct BssInitInfo {
    char *addr;
    unsigned int size;
} BssInitInfo;
__declspec(section ".init") extern RomCopyInfo _rom_copy_info[];
__declspec(section ".init") extern BssInitInfo _bss_init_info[];
extern void __flush_cache(void *, unsigned long);
extern void __OSPSInit(void);
extern void __OSCacheInit(void);
extern char _stack_addr[];
extern char _SDA_BASE_[];
extern char _SDA2_BASE_[];
__declspec(section ".init") static inline void
copy_rom_section(void *dst, const void *src, unsigned long size)
{
    if (size && dst != src) {
        memcpy(dst, src, size);
        __flush_cache(dst, size);
    }
}
__declspec(section ".init") static inline void init_bss_section(void *dst,
                                                                unsigned long size)
{
    if (size)
        memset(dst, 0, size);
}
__declspec(section ".init") static void __init_registers(void)
{
    ASM_INSN(lis r1, _stack_addr@h);
    ASM_INSN(ori r1, r1, _stack_addr@l);
    ASM_INSN(lis r2, _SDA2_BASE_@h);
    ASM_INSN(ori r2, r2, _SDA2_BASE_@l);
    ASM_INSN(lis r13, _SDA_BASE_@h);
    ASM_INSN(ori r13, r13, _SDA_BASE_@l);
}
__declspec(section ".init") static void __init_data(void)
{
    RomCopyInfo *dci;
    BssInitInfo *bii;
    dci = _rom_copy_info;
    while (1) {
        if (dci->size == 0)
            break;
        copy_rom_section(dci->addr, dci->rom, dci->size);
        dci++;
    }
    bii = _bss_init_info;
    while (1) {
        if (bii->size == 0)
            break;
        init_bss_section(bii->addr, bii->size);
        bii++;
    }
}
#pragma peephole on
__declspec(section ".init") static void __check_pad3(void)
{
    if ((*(unsigned short *)0x800030E4 & 0x0EEF) == 0x0EEF)
        OSResetSystem(0, 0, 0);
}
__declspec(section ".init") void __flush_cache(void *addr, unsigned long size)
{
    ASM_INSN(lis r5, -1);
    ASM_INSN(ori r5, r5, 65521);
    ASM_INSN(and r5, r5, r3);
    ASM_INSN(sub r3, r3, r5);
    ASM_INSN(add r4, r4, r3);
    ASM_INSN(l5444: dcbst r0, r5);
    ASM_INSN(sync);
    ASM_INSN(icbi r0, r5);
    ASM_INSN(addic r5, r5, 8);
    ASM_INSN(addic. r4, r4, -8);
    ASM_INSN(bge l5444);
    ASM_INSN(isync);
}
__declspec(section ".init") asm void __init_hardware(void)
{
    nofralloc
    mfmsr r0
    ori r0, r0, 0x2000
    mtmsr r0
    mfspr r31, 8
    bl __OSPSInit
    bl __OSCacheInit
    mtspr 8, r31
    blr
}

extern void DBInit(void);
extern void OSInit(void);
extern void __init_user(void);
extern void InitMetroTRK(void);
extern void main(void);
extern void exit(int status);

__declspec(section ".init") asm void __start(void)
{
    nofralloc
    bl __init_registers
    bl __init_hardware
    li r0, -1
    stwu r1, -8(r1)
    stw r0, 4(r1)
    stw r0, 0(r1)
    bl __init_data
    li r0, 0
    lis r6, -0x8000
    addi r6, r6, 0x44
    stw r0, 0(r6)
    lis r6, -0x8000
    addi r6, r6, 0xf4
    lwz r6, 0(r6)
    cmplwi r6, 0
    beq lab_48
    lwz r7, 0xc(r6)
    b lab_68
lab_48:
    lis r5, -0x8000
    addi r5, r5, 0x34
    lwz r5, 0(r5)
    cmplwi r5, 0
    beq lab_90
    lis r7, -0x8000
    addi r7, r7, 0x30e8
    lwz r7, 0(r7)
lab_68:
    li r5, 0
    cmplwi r7, 2
    beq lab_80
    cmplwi r7, 3
    bne lab_90
    li r5, 1
lab_80:
    lis r6, InitMetroTRK@ha
    addi r6, r6, InitMetroTRK@l
    mtspr 8, r6
    blrl
lab_90:
    lis r6, -0x8000
    addi r6, r6, 0xf4
    lwz r5, 0(r6)
    cmplwi r5, 0
    bc 13, 2, lab_f0
    lwz r6, 8(r5)
    cmplwi r6, 0
    bc 13, 2, lab_f0
    add r6, r5, r6
    lwz r14, 0(r6)
    cmplwi r14, 0
    beq lab_f0
    addi r15, r6, 4
    mtspr 9, r14
lab_c8:
    addi r6, r6, 4
    lwz r7, 0(r6)
    add r7, r7, r5
    stw r7, 0(r6)
    bdnz lab_c8
    lis r5, -0x8000
    addi r5, r5, 0x34
    rlwinm r7, r15, 0, 0, 0x1a
    stw r7, 0(r5)
    b lab_f8
lab_f0:
    li r14, 0
    li r15, 0
lab_f8:
    bl DBInit
    bl OSInit
    lis r4, -0x8000
    addi r4, r4, 0x30e6
    lhz r3, 0(r4)
    andi. r5, r3, 0x8000
    beq lab_120
    andi. r3, r3, 0x7fff
    cmplwi r3, 1
    bne lab_124
lab_120:
    bl __check_pad3
lab_124:
    bl __init_user
    mr r3, r14
    mr r4, r15
    bl main
    b exit
}
