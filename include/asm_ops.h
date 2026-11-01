#ifndef ASM_OPS_H
#define ASM_OPS_H

#ifdef __clang__
#define ASM_MFMSR(v) __asm__ volatile("mfmsr %0" : "=r"(v))
#define ASM_MTMSR(v) __asm__ volatile("mtmsr %0" : : "r"(v))
#define ASM_MFSPR(v, spr) __asm__ volatile("mfspr %0, " #spr : "=r"(v))
#define ASM_MTSPR(spr, v) __asm__ volatile("mtspr " #spr ", %0" : : "r"(v))
#define ASM_MFTBU(v) __asm__ volatile("mftbu %0" : "=r"(v))
#define ASM_MFTB(v) __asm__ volatile("mftb %0" : "=r"(v))
#define ASM_MFSR(v, sr) __asm__ volatile("mfsr %0, " #sr : "=r"(v))
#define ASM_MTSR(sr, v) __asm__ volatile("mtsr " #sr ", %0" : : "r"(v))
#define ASM_SC() __asm__ volatile("sc" : : : "memory")
#define ASM_MTFSB0(bit) __asm__ volatile("mtfsb0 " #bit)
#define ASM_MTFSB1(bit) __asm__ volatile("mtfsb1 " #bit)
#else
#define ASM_MFMSR(v) asm { mfmsr v }
#define ASM_MTMSR(v) asm { mtmsr v }
#define ASM_MFSPR(v, spr) asm { mfspr v, spr }
#define ASM_MTSPR(spr, v) asm { mtspr spr, v }
#define ASM_MFTBU(v) asm { mftbu v }
#define ASM_MFTB(v) asm { mftb v }
#define ASM_MFSR(v, sr) asm { mfsr v, sr }
#define ASM_MTSR(sr, v) asm { mtsr sr, v }
#define ASM_SC() asm { sc }
#define ASM_MTFSB0(bit) asm { mtfsb0 bit }
#define ASM_MTFSB1(bit) asm { mtfsb1 bit }
#endif
#define ASM_ICBI(a, b) asm { icbi a, b }
#define ASM_DCBI(a, b) asm { dcbi a, b }
#define ASM_MFFS(f) asm { mffs f }
#define ASM_MTFSF(mask, f) asm { mtfsf mask, f }
#define ASM_INSN(...) asm { __VA_ARGS__ }

#endif
