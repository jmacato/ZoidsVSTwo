#include "asm_ops.h"

/* doubles 0.0, 2^32, 2^31 */
static const unsigned long __constants[] = {0x00000000, 0x00000000, 0x41F00000, 0x00000000, 0x41E00000, 0x00000000};

void __cvt_fp2unsigned(void)
{
    ASM_INSN(stwu r1, -16(r1));
    ASM_INSN(lis r4, __constants@h);
    ASM_INSN(ori r4, r4, __constants@l);
    ASM_INSN(li r3, 0);
    ASM_INSN(lfd f0, 0(r4));
    ASM_INSN(lfd f3, 8(r4));
    ASM_INSN(lfd f4, 16(r4));
    ASM_INSN(fcmpu cr0, f1, f0);
    ASM_INSN(fcmpu cr6, f1, f3);
    ASM_INSN(blt lecc4);
    ASM_INSN(addi r3, r3, -1);
    ASM_INSN(bge cr6, lecc4);
    ASM_INSN(fcmpu cr7, f1, f4);
    ASM_INSN(fmr f2, f1);
    ASM_INSN(blt cr7, lecb0);
    ASM_INSN(fsub f2, f1, f4);
    ASM_INSN(lecb0: fctiwz f2, f2);
    ASM_INSN(stfd f2, 8(r1));
    ASM_INSN(lwz r3, 12(r1));
    ASM_INSN(blt cr7, lecc4);
    ASM_INSN(addis r3, r3, -32768);
    ASM_INSN(lecc4: addi r1, r1, 16);
}

void __div2u(void)
{
    ASM_INSN(cmpwi r3, 0);
    ASM_INSN(cntlzw r0, r3);
    ASM_INSN(cntlzw r9, r4);
    ASM_INSN(bne lece0);
    ASM_INSN(addi r0, r9, 32);
    ASM_INSN(lece0: cmpwi r5, 0);
    ASM_INSN(cntlzw r9, r5);
    ASM_INSN(cntlzw r10, r6);
    ASM_INSN(bne lecf4);
    ASM_INSN(addi r9, r10, 32);
    ASM_INSN(lecf4: cmpw r0, r9);
    ASM_INSN(subfic r10, r0, 64);
    ASM_INSN(bgt ledac);
    ASM_INSN(addi r9, r9, 1);
    ASM_INSN(subfic r9, r9, 64);
    ASM_INSN(add r0, r0, r9);
    ASM_INSN(sub r9, r10, r9);
    ASM_INSN(mtctr r9);
    ASM_INSN(cmpwi r9, 32);
    ASM_INSN(addi r7, r9, -32);
    ASM_INSN(blt led2c);
    ASM_INSN(srw r8, r3, r7);
    ASM_INSN(li r7, 0);
    ASM_INSN(b led40);
    ASM_INSN(led2c: srw r8, r4, r9);
    ASM_INSN(subfic r7, r9, 32);
    ASM_INSN(slw r7, r3, r7);
    ASM_INSN(or r8, r8, r7);
    ASM_INSN(srw r7, r3, r9);
    ASM_INSN(led40: cmpwi r0, 32);
    ASM_INSN(addic r9, r0, -32);
    ASM_INSN(blt led58);
    ASM_INSN(slw r3, r4, r9);
    ASM_INSN(li r4, 0);
    ASM_INSN(b led6c);
    ASM_INSN(led58: slw r3, r3, r0);
    ASM_INSN(subfic r9, r0, 32);
    ASM_INSN(srw r9, r4, r9);
    ASM_INSN(or r3, r3, r9);
    ASM_INSN(slw r4, r4, r0);
    ASM_INSN(led6c: li r10, -1);
    ASM_INSN(addic r7, r7, 0);
    ASM_INSN(led74: adde r4, r4, r4);
    ASM_INSN(adde r3, r3, r3);
    ASM_INSN(adde r8, r8, r8);
    ASM_INSN(adde r7, r7, r7);
    ASM_INSN(subc r0, r8, r6);
    ASM_INSN(subfe. r9, r5, r7);
    ASM_INSN(blt led9c);
    ASM_INSN(mr r8, r0);
    ASM_INSN(mr r7, r9);
    ASM_INSN(addic r0, r10, 1);
    ASM_INSN(led9c: bdnz led74);
    ASM_INSN(adde r4, r4, r4);
    ASM_INSN(adde r3, r3, r3);
    ASM_INSN(blr);
    ASM_INSN(ledac: li r4, 0);
    ASM_INSN(li r3, 0);
}

void __div2i(void)
{
    ASM_INSN(stwu r1, -16(r1));
    ASM_INSN(rlwinm. r9, r3, 0, 0, 0);
    ASM_INSN(beq ledcc);
    ASM_INSN(subfic r4, r4, 0);
    ASM_INSN(subfze r3, r3);
    ASM_INSN(ledcc: stw r9, 8(r1));
    ASM_INSN(rlwinm. r9, r5, 0, 0, 0);
    ASM_INSN(beq lede0);
    ASM_INSN(subfic r6, r6, 0);
    ASM_INSN(subfze r5, r5);
    ASM_INSN(lede0: stw r9, 12(r1));
    ASM_INSN(cmpwi r3, 0);
    ASM_INSN(cntlzw r0, r3);
    ASM_INSN(cntlzw r9, r4);
    ASM_INSN(bne ledf8);
    ASM_INSN(addi r0, r9, 32);
    ASM_INSN(ledf8: cmpwi r5, 0);
    ASM_INSN(cntlzw r9, r5);
    ASM_INSN(cntlzw r10, r6);
    ASM_INSN(bne lee0c);
    ASM_INSN(addi r9, r10, 32);
    ASM_INSN(lee0c: cmpw r0, r9);
    ASM_INSN(subfic r10, r0, 64);
    ASM_INSN(bgt leee0);
    ASM_INSN(addi r9, r9, 1);
    ASM_INSN(subfic r9, r9, 64);
    ASM_INSN(add r0, r0, r9);
    ASM_INSN(sub r9, r10, r9);
    ASM_INSN(mtctr r9);
    ASM_INSN(cmpwi r9, 32);
    ASM_INSN(addi r7, r9, -32);
    ASM_INSN(blt lee44);
    ASM_INSN(srw r8, r3, r7);
    ASM_INSN(li r7, 0);
    ASM_INSN(b lee58);
    ASM_INSN(lee44: srw r8, r4, r9);
    ASM_INSN(subfic r7, r9, 32);
    ASM_INSN(slw r7, r3, r7);
    ASM_INSN(or r8, r8, r7);
    ASM_INSN(srw r7, r3, r9);
    ASM_INSN(lee58: cmpwi r0, 32);
    ASM_INSN(addic r9, r0, -32);
    ASM_INSN(blt lee70);
    ASM_INSN(slw r3, r4, r9);
    ASM_INSN(li r4, 0);
    ASM_INSN(b lee84);
    ASM_INSN(lee70: slw r3, r3, r0);
    ASM_INSN(subfic r9, r0, 32);
    ASM_INSN(srw r9, r4, r9);
    ASM_INSN(or r3, r3, r9);
    ASM_INSN(slw r4, r4, r0);
    ASM_INSN(lee84: li r10, -1);
    ASM_INSN(addic r7, r7, 0);
    ASM_INSN(lee8c: adde r4, r4, r4);
    ASM_INSN(adde r3, r3, r3);
    ASM_INSN(adde r8, r8, r8);
    ASM_INSN(adde r7, r7, r7);
    ASM_INSN(subc r0, r8, r6);
    ASM_INSN(subfe. r9, r5, r7);
    ASM_INSN(blt leeb4);
    ASM_INSN(mr r8, r0);
    ASM_INSN(mr r7, r9);
    ASM_INSN(addic r0, r10, 1);
    ASM_INSN(leeb4: bdnz lee8c);
    ASM_INSN(adde r4, r4, r4);
    ASM_INSN(adde r3, r3, r3);
    ASM_INSN(lwz r9, 8(r1));
    ASM_INSN(lwz r10, 12(r1));
    ASM_INSN(xor. r7, r9, r10);
    ASM_INSN(beq leedc);
    ASM_INSN(cmpwi r9, 0);
    ASM_INSN(subfic r4, r4, 0);
    ASM_INSN(subfze r3, r3);
    ASM_INSN(leedc: b leee8);
    ASM_INSN(leee0: li r4, 0);
    ASM_INSN(li r3, 0);
    ASM_INSN(leee8: addi r1, r1, 16);
}

void __mod2u(void)
{
    ASM_INSN(cmpwi r3, 0);
    ASM_INSN(cntlzw r0, r3);
    ASM_INSN(cntlzw r9, r4);
    ASM_INSN(bne lef04);
    ASM_INSN(addi r0, r9, 32);
    ASM_INSN(lef04: cmpwi r5, 0);
    ASM_INSN(cntlzw r9, r5);
    ASM_INSN(cntlzw r10, r6);
    ASM_INSN(bne lef18);
    ASM_INSN(addi r9, r10, 32);
    ASM_INSN(lef18: cmpw r0, r9);
    ASM_INSN(subfic r10, r0, 64);
    ASM_INSN(bgt lefd0);
    ASM_INSN(addi r9, r9, 1);
    ASM_INSN(subfic r9, r9, 64);
    ASM_INSN(add r0, r0, r9);
    ASM_INSN(sub r9, r10, r9);
    ASM_INSN(mtctr r9);
    ASM_INSN(cmpwi r9, 32);
    ASM_INSN(addi r7, r9, -32);
    ASM_INSN(blt lef50);
    ASM_INSN(srw r8, r3, r7);
    ASM_INSN(li r7, 0);
    ASM_INSN(b lef64);
    ASM_INSN(lef50: srw r8, r4, r9);
    ASM_INSN(subfic r7, r9, 32);
    ASM_INSN(slw r7, r3, r7);
    ASM_INSN(or r8, r8, r7);
    ASM_INSN(srw r7, r3, r9);
    ASM_INSN(lef64: cmpwi r0, 32);
    ASM_INSN(addic r9, r0, -32);
    ASM_INSN(blt lef7c);
    ASM_INSN(slw r3, r4, r9);
    ASM_INSN(li r4, 0);
    ASM_INSN(b lef90);
    ASM_INSN(lef7c: slw r3, r3, r0);
    ASM_INSN(subfic r9, r0, 32);
    ASM_INSN(srw r9, r4, r9);
    ASM_INSN(or r3, r3, r9);
    ASM_INSN(slw r4, r4, r0);
    ASM_INSN(lef90: li r10, -1);
    ASM_INSN(addic r7, r7, 0);
    ASM_INSN(lef98: adde r4, r4, r4);
    ASM_INSN(adde r3, r3, r3);
    ASM_INSN(adde r8, r8, r8);
    ASM_INSN(adde r7, r7, r7);
    ASM_INSN(subc r0, r8, r6);
    ASM_INSN(subfe. r9, r5, r7);
    ASM_INSN(blt lefc0);
    ASM_INSN(mr r8, r0);
    ASM_INSN(mr r7, r9);
    ASM_INSN(addic r0, r10, 1);
    ASM_INSN(lefc0: bdnz lef98);
    ASM_INSN(mr r4, r8);
    ASM_INSN(mr r3, r7);
    ASM_INSN(blr);
    ASM_INSN(lefd0:);
}

void __mod2i(void)
{
    ASM_INSN(cmpwi cr7, r3, 0);
    ASM_INSN(bge cr7, lefe4);
    ASM_INSN(subfic r4, r4, 0);
    ASM_INSN(subfze r3, r3);
    ASM_INSN(lefe4: cmpwi r5, 0);
    ASM_INSN(bge leff4);
    ASM_INSN(subfic r6, r6, 0);
    ASM_INSN(subfze r5, r5);
    ASM_INSN(leff4: cmpwi r3, 0);
    ASM_INSN(cntlzw r0, r3);
    ASM_INSN(cntlzw r9, r4);
    ASM_INSN(bne lf008);
    ASM_INSN(addi r0, r9, 32);
    ASM_INSN(lf008: cmpwi r5, 0);
    ASM_INSN(cntlzw r9, r5);
    ASM_INSN(cntlzw r10, r6);
    ASM_INSN(bne lf01c);
    ASM_INSN(addi r9, r10, 32);
    ASM_INSN(lf01c: cmpw r0, r9);
    ASM_INSN(subfic r10, r0, 64);
    ASM_INSN(bgt lf0d0);
    ASM_INSN(addi r9, r9, 1);
    ASM_INSN(subfic r9, r9, 64);
    ASM_INSN(add r0, r0, r9);
    ASM_INSN(sub r9, r10, r9);
    ASM_INSN(mtctr r9);
    ASM_INSN(cmpwi r9, 32);
    ASM_INSN(addi r7, r9, -32);
    ASM_INSN(blt lf054);
    ASM_INSN(srw r8, r3, r7);
    ASM_INSN(li r7, 0);
    ASM_INSN(b lf068);
    ASM_INSN(lf054: srw r8, r4, r9);
    ASM_INSN(subfic r7, r9, 32);
    ASM_INSN(slw r7, r3, r7);
    ASM_INSN(or r8, r8, r7);
    ASM_INSN(srw r7, r3, r9);
    ASM_INSN(lf068: cmpwi r0, 32);
    ASM_INSN(addic r9, r0, -32);
    ASM_INSN(blt lf080);
    ASM_INSN(slw r3, r4, r9);
    ASM_INSN(li r4, 0);
    ASM_INSN(b lf094);
    ASM_INSN(lf080: slw r3, r3, r0);
    ASM_INSN(subfic r9, r0, 32);
    ASM_INSN(srw r9, r4, r9);
    ASM_INSN(or r3, r3, r9);
    ASM_INSN(slw r4, r4, r0);
    ASM_INSN(lf094: li r10, -1);
    ASM_INSN(addic r7, r7, 0);
    ASM_INSN(lf09c: adde r4, r4, r4);
    ASM_INSN(adde r3, r3, r3);
    ASM_INSN(adde r8, r8, r8);
    ASM_INSN(adde r7, r7, r7);
    ASM_INSN(subc r0, r8, r6);
    ASM_INSN(subfe. r9, r5, r7);
    ASM_INSN(blt lf0c4);
    ASM_INSN(mr r8, r0);
    ASM_INSN(mr r7, r9);
    ASM_INSN(addic r0, r10, 1);
    ASM_INSN(lf0c4: bdnz lf09c);
    ASM_INSN(mr r4, r8);
    ASM_INSN(mr r3, r7);
    ASM_INSN(lf0d0: bge cr7, lf0dc);
    ASM_INSN(subfic r4, r4, 0);
    ASM_INSN(subfze r3, r3);
    ASM_INSN(lf0dc:);
}

void __shl2i(void)
{
    ASM_INSN(subfic r8, r5, 32);
    ASM_INSN(addic r9, r5, -32);
    ASM_INSN(slw r3, r3, r5);
    ASM_INSN(srw r10, r4, r8);
    ASM_INSN(or r3, r3, r10);
    ASM_INSN(slw r10, r4, r9);
    ASM_INSN(or r3, r3, r10);
    ASM_INSN(slw r4, r4, r5);
}

void __shr2u(void)
{
    ASM_INSN(subfic r8, r5, 32);
    ASM_INSN(addic r9, r5, -32);
    ASM_INSN(srw r4, r4, r5);
    ASM_INSN(slw r10, r3, r8);
    ASM_INSN(or r4, r4, r10);
    ASM_INSN(srw r10, r3, r9);
    ASM_INSN(or r4, r4, r10);
    ASM_INSN(srw r3, r3, r5);
}

void __shr2i(void)
{
    ASM_INSN(subfic r8, r5, 32);
    ASM_INSN(addic. r9, r5, -32);
    ASM_INSN(srw r4, r4, r5);
    ASM_INSN(slw r10, r3, r8);
    ASM_INSN(or r4, r4, r10);
    ASM_INSN(sraw r10, r3, r9);
    ASM_INSN(ble lf148);
    ASM_INSN(or r4, r4, r10);
    ASM_INSN(lf148: sraw r3, r3, r5);
}

void __cvt_sll_flt(void)
{
    ASM_INSN(stwu r1, -16(r1));
    ASM_INSN(rlwinm. r5, r3, 0, 0, 0);
    ASM_INSN(beq lf164);
    ASM_INSN(subfic r4, r4, 0);
    ASM_INSN(subfze r3, r3);
    ASM_INSN(lf164: or. r7, r3, r4);
    ASM_INSN(li r6, 0);
    ASM_INSN(beq lf1ec);
    ASM_INSN(cntlzw r7, r3);
    ASM_INSN(cntlzw r8, r4);
    ASM_INSN(rlwinm r9, r7, 26, 0, 4);
    ASM_INSN(srawi r9, r9, 31);
    ASM_INSN(and r9, r9, r8);
    ASM_INSN(add r7, r7, r9);
    ASM_INSN(subfic r8, r7, 32);
    ASM_INSN(addic r9, r7, -32);
    ASM_INSN(slw r3, r3, r7);
    ASM_INSN(srw r10, r4, r8);
    ASM_INSN(or r3, r3, r10);
    ASM_INSN(slw r10, r4, r9);
    ASM_INSN(or r3, r3, r10);
    ASM_INSN(slw r4, r4, r7);
    ASM_INSN(sub r6, r6, r7);
    ASM_INSN(clrlwi r7, r4, 21);
    ASM_INSN(cmpwi r7, 1024);
    ASM_INSN(addi r6, r6, 1086);
    ASM_INSN(blt lf1d4);
    ASM_INSN(bgt lf1c8);
    ASM_INSN(rlwinm. r7, r4, 0, 20, 20);
    ASM_INSN(beq lf1d4);
    ASM_INSN(lf1c8: addic r4, r4, 2048);
    ASM_INSN(addze r3, r3);
    ASM_INSN(addze r6, r6);
    ASM_INSN(lf1d4: rotlwi r4, r4, 21);
    ASM_INSN(rlwimi r4, r3, 21, 0, 10);
    ASM_INSN(rlwinm r3, r3, 21, 12, 31);
    ASM_INSN(slwi r6, r6, 20);
    ASM_INSN(or r3, r6, r3);
    ASM_INSN(or r3, r5, r3);
    ASM_INSN(lf1ec: stw r3, 8(r1));
    ASM_INSN(stw r4, 12(r1));
    ASM_INSN(lfd f1, 8(r1));
    ASM_INSN(frsp f1, f1);
    ASM_INSN(addi r1, r1, 16);
}

void __cvt_dbl_usll(void)
{
    ASM_INSN(stwu r1, -16(r1));
    ASM_INSN(stfd f1, 8(r1));
    ASM_INSN(lwz r3, 8(r1));
    ASM_INSN(lwz r4, 12(r1));
    ASM_INSN(rlwinm r5, r3, 12, 21, 31);
    ASM_INSN(cmplwi r5, 1023);
    ASM_INSN(bge lf22c);
    ASM_INSN(li r3, 0);
    ASM_INSN(li r4, 0);
    ASM_INSN(b lf2c8);
    ASM_INSN(lf22c: mr r6, r3);
    ASM_INSN(clrlwi r3, r3, 12);
    ASM_INSN(oris r3, r3, 16);
    ASM_INSN(addi r5, r5, -1075);
    ASM_INSN(cmpwi r5, 0);
    ASM_INSN(bge lf26c);
    ASM_INSN(neg r5, r5);
    ASM_INSN(subfic r8, r5, 32);
    ASM_INSN(addic r9, r5, -32);
    ASM_INSN(srw r4, r4, r5);
    ASM_INSN(slw r10, r3, r8);
    ASM_INSN(or r4, r4, r10);
    ASM_INSN(srw r10, r3, r9);
    ASM_INSN(or r4, r4, r10);
    ASM_INSN(srw r3, r3, r5);
    ASM_INSN(b lf2b8);
    ASM_INSN(lf26c: cmpwi r5, 10);
    ASM_INSN(ble+ lf298);
    ASM_INSN(rlwinm. r6, r6, 0, 0, 0);
    ASM_INSN(beq lf288);
    ASM_INSN(lis r3, -32768);
    ASM_INSN(li r4, 0);
    ASM_INSN(b lf2c8);
    ASM_INSN(lf288: lis r3, 32767);
    ASM_INSN(ori r3, r3, 65535);
    ASM_INSN(li r4, -1);
    ASM_INSN(b lf2c8);
    ASM_INSN(lf298: subfic r8, r5, 32);
    ASM_INSN(addic r9, r5, -32);
    ASM_INSN(slw r3, r3, r5);
    ASM_INSN(srw r10, r4, r8);
    ASM_INSN(or r3, r3, r10);
    ASM_INSN(slw r10, r4, r9);
    ASM_INSN(or r3, r3, r10);
    ASM_INSN(slw r4, r4, r5);
    ASM_INSN(lf2b8: rlwinm. r6, r6, 0, 0, 0);
    ASM_INSN(beq lf2c8);
    ASM_INSN(subfic r4, r4, 0);
    ASM_INSN(subfze r3, r3);
    ASM_INSN(lf2c8: addi r1, r1, 16);
}

