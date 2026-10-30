#ifndef ASM_OPS_H
#define ASM_OPS_H

#define ASM_MFMSR(v) asm { mfmsr v }
#define ASM_MTMSR(v) asm { mtmsr v }
#define ASM_MFSPR(v, spr) asm { mfspr v, spr }
#define ASM_MTSPR(spr, v) asm { mtspr spr, v }
#define ASM_MFTBU(v) asm { mftbu v }
#define ASM_MFTB(v) asm { mftb v }
#define ASM_MFSR(v, sr) asm { mfsr v, sr }
#define ASM_MTSR(sr, v) asm { mtsr sr, v }
#define ASM_ICBI(a, b) asm { icbi a, b }
#define ASM_DCBI(a, b) asm { dcbi a, b }
#define ASM_SC() asm { sc }
#define ASM_MTFSB0(bit) asm { mtfsb0 bit }
#define ASM_MTFSB1(bit) asm { mtfsb1 bit }
#define ASM_MFFS(f) asm { mffs f }
#define ASM_MTFSF(mask, f) asm { mtfsf mask, f }
#define ASM_INSN(...) asm { __VA_ARGS__ }

#endif
