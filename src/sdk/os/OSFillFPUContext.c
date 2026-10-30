asm void OSFillFPUContext(void)
{
    nofralloc
    mfmsr r5
    ori r5, r5, 0x2000
    mtmsr r5
    isync
    stfd f0, 0x90(r3)
    stfd f1, 0x98(r3)
    stfd f2, 0xa0(r3)
    stfd f3, 0xa8(r3)
    stfd f4, 0xb0(r3)
    stfd f5, 0xb8(r3)
    stfd f6, 0xc0(r3)
    stfd f7, 0xc8(r3)
    stfd f8, 0xd0(r3)
    stfd f9, 0xd8(r3)
    stfd f10, 0xe0(r3)
    stfd f11, 0xe8(r3)
    stfd f12, 0xf0(r3)
    stfd f13, 0xf8(r3)
    stfd f14, 0x100(r3)
    stfd f15, 0x108(r3)
    stfd f16, 0x110(r3)
    stfd f17, 0x118(r3)
    stfd f18, 0x120(r3)
    stfd f19, 0x128(r3)
    stfd f20, 0x130(r3)
    stfd f21, 0x138(r3)
    stfd f22, 0x140(r3)
    stfd f23, 0x148(r3)
    stfd f24, 0x150(r3)
    stfd f25, 0x158(r3)
    stfd f26, 0x160(r3)
    stfd f27, 0x168(r3)
    stfd f28, 0x170(r3)
    stfd f29, 0x178(r3)
    stfd f30, 0x180(r3)
    stfd f31, 0x188(r3)
    mffs f0
    stfd f0, 0x190(r3)
    lfd f0, 0x90(r3)
    mfspr r5, 920
    rlwinm. r5, r5, 3, 0x1f, 0x1f
    beq lab_128
    psq_st f0, 0x1c8(r3), 0, 0
    psq_st f1, 0x1d0(r3), 0, 0
    psq_st f2, 0x1d8(r3), 0, 0
    psq_st f3, 0x1e0(r3), 0, 0
    psq_st f4, 0x1e8(r3), 0, 0
    psq_st f5, 0x1f0(r3), 0, 0
    psq_st f6, 0x1f8(r3), 0, 0
    psq_st f7, 0x200(r3), 0, 0
    psq_st f8, 0x208(r3), 0, 0
    psq_st f9, 0x210(r3), 0, 0
    psq_st f10, 0x218(r3), 0, 0
    psq_st f11, 0x220(r3), 0, 0
    psq_st f12, 0x228(r3), 0, 0
    psq_st f13, 0x230(r3), 0, 0
    psq_st f14, 0x238(r3), 0, 0
    psq_st f15, 0x240(r3), 0, 0
    psq_st f16, 0x248(r3), 0, 0
    psq_st f17, 0x250(r3), 0, 0
    psq_st f18, 0x258(r3), 0, 0
    psq_st f19, 0x260(r3), 0, 0
    psq_st f20, 0x268(r3), 0, 0
    psq_st f21, 0x270(r3), 0, 0
    psq_st f22, 0x278(r3), 0, 0
    psq_st f23, 0x280(r3), 0, 0
    psq_st f24, 0x288(r3), 0, 0
    psq_st f25, 0x290(r3), 0, 0
    psq_st f26, 0x298(r3), 0, 0
    psq_st f27, 0x2a0(r3), 0, 0
    psq_st f28, 0x2a8(r3), 0, 0
    psq_st f29, 0x2b0(r3), 0, 0
    psq_st f30, 0x2b8(r3), 0, 0
    psq_st f31, 0x2c0(r3), 0, 0
lab_128:
    blr
}
