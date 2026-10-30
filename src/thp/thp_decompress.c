/* Source and layouts:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/thp/THPDec.c */
#include "types.h"
#include "asm_ops.h"

typedef int BOOL;
typedef u8 THPSample;
typedef s16 THPCoeff;
typedef f32 THPQuantTab[64];

typedef struct _THPHuffmanTab {
    u8 quick[32];
    u8 increment[32];
    u8 *Vij;
    s32 maxCode[18];
    s32 valPtr[18];
    u8 Vij1;
    u8 pad[11];
} THPHuffmanTab;

typedef struct _THPComponent {
    u8 quantizationTableSelector;
    u8 DCTableSelector;
    u8 ACTableSelector;
    THPCoeff predDC;
} THPComponent;

typedef struct _THPFileInfo {
    THPQuantTab quantTabs[3];
    THPHuffmanTab huffmanTabs[4];
    THPComponent components[3];
    u16 xPixelSize;
    u16 yPixelSize;
    u16 MCUsPerRow;
    u16 decompressedY;
    u8 *c;
    u32 currByte;
    u32 cnt;
    u8 validHuffmanTabs;
    u8 RST;
    u16 nMCU;
    u16 currMCU;
    u8 *dLC[3];
} THPFileInfo;

extern const u8 __THPJpegNaturalOrder[80];
extern THPHuffmanTab *Ydchuff, *Udchuff, *Vdchuff, *Yachuff, *Uachuff, *Vachuff;
extern THPSample *Gbase;
extern u32 Gwid;
extern f32 *Gq;
extern u32 __THPOldGQR5;
extern u32 __THPOldGQR6;
extern THPFileInfo *__THPInfo;
extern BOOL __THPInitFlag;

static f32 __THPIDCTWorkspace[64];
static u8 *__THPLCWork512[3];
static u8 *__THPLCWork640[3];
static THPCoeff *__THPMCUBuffer[6];

extern void LCQueueWait(u32 len);
extern u32 LCStoreData(void *dest, void *src, u32 len);
extern u32 PPCMfhid2(void);
extern void __THPPrepBitStream(void);

static void __THPDecompressiMCURow512x448(void);
static void __THPDecompressiMCURow640x480(void);
static void __THPDecompressiMCURowNxN(void);
static void __THPHuffDecodeDCTCompY(THPFileInfo *info, THPCoeff *block);
static void __THPHuffDecodeDCTCompU(THPFileInfo *info, THPCoeff *block);
static void __THPHuffDecodeDCTCompV(THPFileInfo *info, THPCoeff *block);

#define THP_GQR_SETUP() { \
    register u32 tmp2; \
    register u32 tmp1; \
    ASM_INSN(mfspr tmp1, GQR5); \
    ASM_INSN(mfspr tmp2, GQR6); \
    __THPOldGQR5 = tmp1; \
    __THPOldGQR6 = tmp2; \
    ASM_INSN(li r3, 0x0007); \
    ASM_INSN(oris r3, r3, 0x0007); \
    ASM_INSN(mtspr GQR5, r3); \
    ASM_INSN(li r3, 0x3D04); \
    ASM_INSN(oris r3, r3, 0x3D04); \
    ASM_INSN(mtspr GQR6, r3); \
}

#define THP_GQR_RESTORE() { \
    register u32 tmp2; \
    register u32 tmp1; \
    tmp1 = __THPOldGQR5; \
    tmp2 = __THPOldGQR6; \
    ASM_INSN(mtspr GQR5, tmp1); \
    ASM_INSN(mtspr GQR6, tmp2); \
}

#define THP_IDCT_NOYPOS_VARS(ID) \
    register THPSample *out1_##ID; \
    register THPSample *out0_##ID; \
    register u32 off1_##ID; \
    register u32 off0_##ID; \
    register u32 itmp0_2_##ID; \
    register u32 wid_##ID; \
    register THPSample *obase_##ID; \
    register u32 itmp3_##ID; \
    register u32 itmp2_##ID; \
    register u32 itmp1_##ID; \
    register u32 itmp0_##ID; \
    register f32 bias_##ID; \
    register f32 cc2c6a_##ID; \
    register f32 cc2c6s_##ID; \
    register f32 cc2_##ID; \
    register f32 cc4_##ID; \
    register f32 tmp23_##ID; \
    register f32 tmp22_##ID; \
    register f32 tmp21_##ID; \
    register f32 tmp20_##ID; \
    register f32 tmp13_##ID; \
    register f32 tmp12_##ID; \
    register f32 tmp11_##ID; \
    register f32 tmp10_##ID; \
    register f32 tmp9_##ID; \
    register f32 tmp8_##ID; \
    register f32 tmp7_##ID; \
    register f32 tmp6_##ID; \
    register f32 tmp5_##ID; \
    register f32 tmp4_##ID; \
    register f32 tmp3_##ID; \
    register f32 tmp2_##ID; \
    register f32 tmp1_##ID; \
    register f32 tmp0_##ID; \
    register f32 *ws_##ID; \
    register f32 *q_##ID; \
    register THPCoeff *in_##ID; \
    register u32 xPos_##ID;

#define THP_IDCT_NOYPOS(ID, IN_, XPOS_) { \
    xPos_##ID = (XPOS_); \
    in_##ID = (IN_); \
    cc4_##ID = 1.414213562F; \
    cc2_##ID = 1.847759065F; \
    cc2c6s_##ID = 1.082392200F; \
    cc2c6a_##ID = -2.613125930F; \
    bias_##ID = 1024.0F; \
    q_##ID = Gq; \
    ws_##ID = &__THPIDCTWorkspace[0] - 2; \
    { \
        ASM_INSN(li itmp2_##ID, 8); \
        ASM_INSN(mtctr itmp2_##ID); \
        ASM_INSN(l_loophead0_##ID: psq_l tmp10_##ID, 0(in_##ID), 0, 5); \
        ASM_INSN(psq_l tmp11_##ID, 0(q_##ID), 0, 0); \
        ASM_INSN(lwz itmp0_##ID, 12(in_##ID)); \
        ASM_INSN(lwz itmp3_##ID, 8(in_##ID)); \
        ASM_INSN(ps_mul tmp10_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(lwz itmp1_##ID, 4(in_##ID)); \
        ASM_INSN(lhz itmp2_##ID, 2(in_##ID)); \
        ASM_INSN(or. itmp0_##ID, itmp0_##ID, itmp3_##ID); \
        ASM_INSN(l_loophead1_##ID: cmpwi itmp0_##ID, 0); \
        ASM_INSN(bne l_regularidct_##ID); \
        ASM_INSN(ps_merge00 tmp0_##ID, tmp10_##ID, tmp10_##ID); \
        ASM_INSN(cmpwi itmp1_##ID, 0); \
        ASM_INSN(psq_st tmp0_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(bne l_halfidct_##ID); \
        ASM_INSN(psq_st tmp0_##ID, 16(ws_##ID), 0, 0); \
        ASM_INSN(cmpwi itmp2_##ID, 0); \
        ASM_INSN(psq_st tmp0_##ID, 24(ws_##ID), 0, 0); \
        ASM_INSN(bne l_quarteridct_##ID); \
        ASM_INSN(addi q_##ID, q_##ID, 8*sizeof(f32)); \
        ASM_INSN(psq_stu tmp0_##ID, 32(ws_##ID), 0, 0); \
        ASM_INSN(addi in_##ID, in_##ID, 8*sizeof(THPCoeff)); \
        ASM_INSN(bdnz l_loophead0_##ID); \
        ASM_INSN(b l_loopend_##ID); \
        ASM_INSN(l_quarteridct_##ID: addi in_##ID, in_##ID, 8*sizeof(THPCoeff)); \
        ASM_INSN(ps_msub tmp2_##ID, tmp10_##ID, cc2_##ID, tmp10_##ID); \
        ASM_INSN(addi q_##ID, q_##ID, 8*sizeof(f32)); \
        ASM_INSN(ps_merge00 tmp9_##ID, tmp10_##ID, tmp10_##ID); \
        ASM_INSN(lwz itmp1_##ID, 4(in_##ID)); \
        ASM_INSN(ps_sub tmp1_##ID, cc2_##ID, cc2c6s_##ID); \
        ASM_INSN(ps_msub tmp3_##ID, tmp10_##ID, cc4_##ID, tmp2_##ID); \
        ASM_INSN(lhz itmp2_##ID, 2(in_##ID)); \
        ASM_INSN(ps_merge11 tmp5_##ID, tmp10_##ID, tmp2_##ID); \
        ASM_INSN(psq_l tmp11_##ID, 0(q_##ID), 0, 0); \
        ASM_INSN(ps_nmsub tmp4_##ID, tmp10_##ID, tmp1_##ID, tmp3_##ID); \
        ASM_INSN(ps_add tmp7_##ID, tmp9_##ID, tmp5_##ID); \
        ASM_INSN(psq_l tmp10_##ID, 0(in_##ID), 0, 5); \
        ASM_INSN(ps_merge11 tmp6_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(ps_sub tmp5_##ID, tmp9_##ID, tmp5_##ID); \
        ASM_INSN(lwz itmp0_##ID, 12(in_##ID)); \
        ASM_INSN(ps_add tmp8_##ID, tmp9_##ID, tmp6_##ID); \
        ASM_INSN(lwz itmp3_##ID, 8(in_##ID)); \
        ASM_INSN(ps_sub tmp6_##ID, tmp9_##ID, tmp6_##ID); \
        ASM_INSN(psq_stu tmp7_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_merge10 tmp6_##ID, tmp6_##ID, tmp6_##ID); \
        ASM_INSN(psq_stu tmp8_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_merge10 tmp5_##ID, tmp5_##ID, tmp5_##ID); \
        ASM_INSN(or itmp0_##ID, itmp0_##ID, itmp3_##ID); \
        ASM_INSN(psq_stu tmp6_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_mul tmp10_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(psq_stu tmp5_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(bdnz l_loophead1_##ID); \
        ASM_INSN(b l_loopend_##ID); \
        ASM_INSN(l_halfidct_##ID: psq_l tmp1_##ID, 4(in_##ID), 0, 5); \
        ASM_INSN(psq_l tmp9_##ID, 8(q_##ID), 0, 0); \
        ASM_INSN(addi in_##ID, in_##ID, 8*sizeof(THPCoeff)); \
        ASM_INSN(ps_mul tmp1_##ID, tmp1_##ID, tmp9_##ID); \
        ASM_INSN(addi q_##ID, q_##ID, 8*sizeof(f32)); \
        ASM_INSN(ps_sub tmp3_##ID, tmp10_##ID, tmp1_##ID); \
        ASM_INSN(ps_add tmp2_##ID, tmp10_##ID, tmp1_##ID); \
        ASM_INSN(lwz itmp0_##ID, 12(in_##ID)); \
        ASM_INSN(ps_madd tmp4_##ID, tmp1_##ID, cc4_##ID, tmp3_##ID); \
        ASM_INSN(ps_nmsub tmp5_##ID, tmp1_##ID, cc4_##ID, tmp2_##ID); \
        ASM_INSN(ps_mul tmp8_##ID, tmp3_##ID, cc2_##ID); \
        ASM_INSN(ps_merge00 tmp4_##ID, tmp2_##ID, tmp4_##ID); \
        ASM_INSN(lwz itmp3_##ID, 8(in_##ID)); \
        ASM_INSN(ps_nmsub tmp6_##ID, tmp1_##ID, cc2c6a_##ID, tmp8_##ID); \
        ASM_INSN(ps_merge00 tmp5_##ID, tmp5_##ID, tmp3_##ID); \
        ASM_INSN(lwz itmp1_##ID, 4(in_##ID)); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp2_##ID); \
        ASM_INSN(ps_msub tmp7_##ID, tmp10_##ID, cc2c6s_##ID, tmp8_##ID); \
        ASM_INSN(lhz itmp2_##ID, 2(in_##ID)); \
        ASM_INSN(ps_merge11 tmp2_##ID, tmp2_##ID, tmp6_##ID); \
        ASM_INSN(ps_msub tmp8_##ID, tmp3_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(psq_l tmp10_##ID, 0(in_##ID), 0, 5); \
        ASM_INSN(ps_add tmp9_##ID, tmp4_##ID, tmp2_##ID); \
        ASM_INSN(ps_add tmp7_##ID, tmp8_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp11_##ID, 0(q_##ID), 0, 0); \
        ASM_INSN(ps_merge11 tmp3_##ID, tmp8_##ID, tmp7_##ID); \
        ASM_INSN(ps_sub tmp4_##ID, tmp4_##ID, tmp2_##ID); \
        ASM_INSN(psq_stu tmp9_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp0_##ID, tmp5_##ID, tmp3_##ID); \
        ASM_INSN(ps_sub tmp1_##ID, tmp5_##ID, tmp3_##ID); \
        ASM_INSN(or itmp0_##ID, itmp0_##ID, itmp3_##ID); \
        ASM_INSN(psq_stu tmp0_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_merge10 tmp1_##ID, tmp1_##ID, tmp1_##ID); \
        ASM_INSN(ps_merge10 tmp4_##ID, tmp4_##ID, tmp4_##ID); \
        ASM_INSN(psq_stu tmp1_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_mul tmp10_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(psq_stu tmp4_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(bdnz l_loophead1_##ID); \
        ASM_INSN(b l_loopend_##ID); \
        ASM_INSN(l_regularidct_##ID: psq_l tmp9_##ID, 4(in_##ID), 0, 5); \
        ASM_INSN(psq_l tmp5_##ID, 8(q_##ID), 0, 0); \
        ASM_INSN(ps_mul tmp9_##ID, tmp9_##ID, tmp5_##ID); \
        ASM_INSN(psq_l tmp2_##ID, 8(in_##ID), 0, 5); \
        ASM_INSN(psq_l tmp6_##ID, 16(q_##ID), 0, 0); \
        ASM_INSN(ps_merge01 tmp0_##ID, tmp10_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp3_##ID, 12(in_##ID), 0, 5); \
        ASM_INSN(ps_merge01 tmp1_##ID, tmp9_##ID, tmp10_##ID); \
        ASM_INSN(psq_l tmp7_##ID, 24(q_##ID), 0, 0); \
        ASM_INSN(addi in_##ID, in_##ID, 8*sizeof(THPCoeff)); \
        ASM_INSN(ps_madd tmp4_##ID, tmp2_##ID, tmp6_##ID, tmp0_##ID); \
        ASM_INSN(ps_nmsub tmp5_##ID, tmp2_##ID, tmp6_##ID, tmp0_##ID); \
        ASM_INSN(ps_madd tmp6_##ID, tmp3_##ID, tmp7_##ID, tmp1_##ID); \
        ASM_INSN(ps_nmsub tmp7_##ID, tmp3_##ID, tmp7_##ID, tmp1_##ID); \
        ASM_INSN(addi q_##ID, q_##ID, 8*sizeof(f32)); \
        ASM_INSN(ps_add tmp0_##ID, tmp4_##ID, tmp6_##ID); \
        ASM_INSN(ps_sub tmp3_##ID, tmp4_##ID, tmp6_##ID); \
        ASM_INSN(ps_msub tmp2_##ID, tmp7_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(lwz itmp0_##ID, 12(in_##ID)); \
        ASM_INSN(ps_sub tmp8_##ID, tmp7_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp1_##ID, tmp5_##ID, tmp2_##ID); \
        ASM_INSN(ps_sub tmp2_##ID, tmp5_##ID, tmp2_##ID); \
        ASM_INSN(ps_mul tmp8_##ID, tmp8_##ID, cc2_##ID); \
        ASM_INSN(lwz itmp3_##ID, 8(in_##ID)); \
        ASM_INSN(ps_merge00 tmp1_##ID, tmp0_##ID, tmp1_##ID); \
        ASM_INSN(ps_nmsub tmp6_##ID, tmp5_##ID, cc2c6a_##ID, tmp8_##ID); \
        ASM_INSN(ps_msub tmp4_##ID, tmp7_##ID, cc2c6s_##ID, tmp8_##ID); \
        ASM_INSN(lwz itmp1_##ID, 4(in_##ID)); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp0_##ID); \
        ASM_INSN(ps_merge00 tmp2_##ID, tmp2_##ID, tmp3_##ID); \
        ASM_INSN(lhz itmp2_##ID, 2(in_##ID)); \
        ASM_INSN(ps_madd tmp5_##ID, tmp3_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(ps_merge11 tmp7_##ID, tmp0_##ID, tmp6_##ID); \
        ASM_INSN(psq_l tmp10_##ID, 0(in_##ID), 0, 5); \
        ASM_INSN(ps_sub tmp4_##ID, tmp4_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp3_##ID, tmp1_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp11_##ID, 0(q_##ID), 0, 0); \
        ASM_INSN(ps_merge11 tmp4_##ID, tmp5_##ID, tmp4_##ID); \
        ASM_INSN(ps_sub tmp0_##ID, tmp1_##ID, tmp7_##ID); \
        ASM_INSN(ps_mul tmp10_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(ps_add tmp5_##ID, tmp2_##ID, tmp4_##ID); \
        ASM_INSN(ps_sub tmp6_##ID, tmp2_##ID, tmp4_##ID); \
        ASM_INSN(ps_merge10 tmp5_##ID, tmp5_##ID, tmp5_##ID); \
        ASM_INSN(psq_stu tmp3_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_merge10 tmp0_##ID, tmp0_##ID, tmp0_##ID); \
        ASM_INSN(psq_stu tmp6_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(psq_stu tmp5_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(or itmp0_##ID, itmp0_##ID, itmp3_##ID); \
        ASM_INSN(psq_stu tmp0_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(bdnz l_loophead1_##ID); \
        l_loopend_##ID:; \
    } \
    ws_##ID = &__THPIDCTWorkspace[0]; \
    { \
        obase_##ID = Gbase; \
        wid_##ID = Gwid; \
        ASM_INSN(psq_l tmp10_##ID, 8*0*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(slwi xPos_##ID, xPos_##ID, 2); \
        ASM_INSN(psq_l tmp11_##ID, 8*4*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(slwi off1_##ID, wid_##ID, 2); \
        ASM_INSN(psq_l tmp12_##ID, 8*2*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(mr off0_##ID, xPos_##ID); \
        ASM_INSN(ps_add tmp6_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(psq_l tmp13_##ID, 8*6*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp8_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(add off1_##ID, off0_##ID, off1_##ID); \
        ASM_INSN(ps_add tmp6_##ID, tmp6_##ID, bias_##ID); \
        ASM_INSN(li itmp0_2_##ID, 3); \
        ASM_INSN(ps_add tmp7_##ID, tmp12_##ID, tmp13_##ID); \
        ASM_INSN(add out0_##ID, obase_##ID, off0_##ID); \
        ASM_INSN(ps_sub tmp9_##ID, tmp12_##ID, tmp13_##ID); \
        ASM_INSN(ps_add tmp0_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(add out1_##ID, obase_##ID, off1_##ID); \
        ASM_INSN(ps_add tmp8_##ID, tmp8_##ID, bias_##ID); \
        ASM_INSN(mtctr itmp0_2_##ID); \
        ASM_INSN(l_loophead10_##ID: psq_l tmp4_##ID, 8*1*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_msub tmp9_##ID, tmp9_##ID, cc4_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp5_##ID, 8*3*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp3_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(ps_add tmp1_##ID, tmp8_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp6_##ID, 8*5*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp2_##ID, tmp8_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp7_##ID, 8*7*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp8_##ID, tmp6_##ID, tmp5_##ID); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp5_##ID); \
        ASM_INSN(addi ws_##ID, ws_##ID, 2*sizeof(f32)); \
        ASM_INSN(ps_add tmp9_##ID, tmp4_##ID, tmp7_##ID); \
        ASM_INSN(ps_sub tmp4_##ID, tmp4_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp10_##ID, 8*0*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp7_##ID, tmp9_##ID, tmp8_##ID); \
        ASM_INSN(ps_sub tmp5_##ID, tmp9_##ID, tmp8_##ID); \
        ASM_INSN(ps_add tmp8_##ID, tmp6_##ID, tmp4_##ID); \
        ASM_INSN(psq_l tmp11_##ID, 8*4*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp9_##ID, tmp0_##ID, tmp7_##ID); \
        ASM_INSN(ps_mul tmp8_##ID, tmp8_##ID, cc2_##ID); \
        ASM_INSN(psq_l tmp12_##ID, 8*2*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp23_##ID, tmp0_##ID, tmp7_##ID); \
        ASM_INSN(ps_madd tmp6_##ID, tmp6_##ID, cc2c6a_##ID, tmp8_##ID); \
        ASM_INSN(psq_l tmp13_##ID, 8*6*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(addi off0_##ID, off0_##ID, 2*sizeof(THPSample)); \
        ASM_INSN(psq_st tmp9_##ID, 0(out0_##ID), 0, 6); \
        ASM_INSN(ps_msub tmp4_##ID, tmp4_##ID, cc2c6s_##ID, tmp8_##ID); \
        ASM_INSN(ps_add tmp9_##ID, tmp1_##ID, tmp6_##ID); \
        ASM_INSN(ps_msub tmp5_##ID, tmp5_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(ps_sub tmp22_##ID, tmp1_##ID, tmp6_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 8(out0_##ID), 0, 6); \
        ASM_INSN(ps_add tmp8_##ID, tmp2_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp4_##ID, tmp4_##ID, tmp5_##ID); \
        ASM_INSN(psq_st tmp8_##ID, 16(out0_##ID), 0, 6); \
        ASM_INSN(addi off1_##ID, off1_##ID, 2*sizeof(THPSample)); \
        ASM_INSN(ps_sub tmp9_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(ps_add tmp20_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 24(out0_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp21_##ID, tmp2_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp6_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(psq_st tmp20_##ID, 0(out1_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp8_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(ps_add tmp6_##ID, tmp6_##ID, bias_##ID); \
        ASM_INSN(psq_st tmp21_##ID, 8(out1_##ID), 0, 6); \
        ASM_INSN(ps_add tmp7_##ID, tmp12_##ID, tmp13_##ID); \
        ASM_INSN(ps_sub tmp9_##ID, tmp12_##ID, tmp13_##ID); \
        ASM_INSN(psq_st tmp22_##ID, 16(out1_##ID), 0, 6); \
        ASM_INSN(add out0_##ID, obase_##ID, off0_##ID); \
        ASM_INSN(ps_add tmp0_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(psq_st tmp23_##ID, 24(out1_##ID), 0, 6); \
        ASM_INSN(ps_add tmp8_##ID, tmp8_##ID, bias_##ID); \
        ASM_INSN(add out1_##ID, obase_##ID, off1_##ID); \
        ASM_INSN(bdnz l_loophead10_##ID); \
        ASM_INSN(psq_l tmp4_##ID, 8*1*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_msub tmp9_##ID, tmp9_##ID, cc4_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp5_##ID, 8*3*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp3_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(ps_add tmp1_##ID, tmp8_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp6_##ID, 8*5*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp2_##ID, tmp8_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp7_##ID, 8*7*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp8_##ID, tmp6_##ID, tmp5_##ID); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp9_##ID, tmp4_##ID, tmp7_##ID); \
        ASM_INSN(ps_sub tmp4_##ID, tmp4_##ID, tmp7_##ID); \
        ASM_INSN(ps_add tmp7_##ID, tmp9_##ID, tmp8_##ID); \
        ASM_INSN(ps_sub tmp5_##ID, tmp9_##ID, tmp8_##ID); \
        ASM_INSN(ps_add tmp8_##ID, tmp6_##ID, tmp4_##ID); \
        ASM_INSN(ps_add tmp9_##ID, tmp0_##ID, tmp7_##ID); \
        ASM_INSN(ps_mul tmp8_##ID, tmp8_##ID, cc2_##ID); \
        ASM_INSN(ps_sub tmp23_##ID, tmp0_##ID, tmp7_##ID); \
        ASM_INSN(ps_madd tmp6_##ID, tmp6_##ID, cc2c6a_##ID, tmp8_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 0(out0_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(ps_msub tmp4_##ID, tmp4_##ID, cc2c6s_##ID, tmp8_##ID); \
        ASM_INSN(psq_st tmp23_##ID, 24(out1_##ID), 0, 6); \
        ASM_INSN(ps_add tmp9_##ID, tmp1_##ID, tmp6_##ID); \
        ASM_INSN(ps_msub tmp5_##ID, tmp5_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(ps_sub tmp22_##ID, tmp1_##ID, tmp6_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 8(out0_##ID), 0, 6); \
        ASM_INSN(ps_add tmp8_##ID, tmp2_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp4_##ID, tmp4_##ID, tmp5_##ID); \
        ASM_INSN(psq_st tmp22_##ID, 16(out1_##ID), 0, 6); \
        ASM_INSN(psq_st tmp8_##ID, 16(out0_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp9_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(ps_add tmp20_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 24(out0_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp21_##ID, tmp2_##ID, tmp5_##ID); \
        ASM_INSN(psq_st tmp20_##ID, 0(out1_##ID), 0, 6); \
        ASM_INSN(psq_st tmp21_##ID, 8(out1_##ID), 0, 6); \
    } \
}

#define THP_IDCT_Y8_VARS(ID) \
    register THPSample *out1_##ID; \
    register THPSample *out0_##ID; \
    register u32 off1_##ID; \
    register u32 off0_##ID; \
    register u32 itmp0_2_##ID; \
    register u32 wid_##ID; \
    register THPSample *obase_##ID; \
    register u32 itmp3_##ID; \
    register u32 itmp2_##ID; \
    register u32 itmp1_##ID; \
    register u32 itmp0_##ID; \
    register f32 bias_##ID; \
    register f32 cc2c6a_##ID; \
    register f32 cc2c6s_##ID; \
    register f32 cc2_##ID; \
    register f32 cc4_##ID; \
    register f32 tmp23_##ID; \
    register f32 tmp22_##ID; \
    register f32 tmp21_##ID; \
    register f32 tmp20_##ID; \
    register f32 tmp13_##ID; \
    register f32 tmp12_##ID; \
    register f32 tmp11_##ID; \
    register f32 tmp10_##ID; \
    register f32 tmp9_##ID; \
    register f32 tmp8_##ID; \
    register f32 tmp7_##ID; \
    register f32 tmp6_##ID; \
    register f32 tmp5_##ID; \
    register f32 tmp4_##ID; \
    register f32 tmp3_##ID; \
    register f32 tmp2_##ID; \
    register f32 tmp1_##ID; \
    register f32 tmp0_##ID; \
    register f32 *ws_##ID; \
    register f32 *q_##ID; \
    register THPCoeff *in_##ID; \
    register u32 xPos_##ID;

#define THP_IDCT_Y8(ID, IN_, XPOS_) { \
    xPos_##ID = (XPOS_); \
    in_##ID = (IN_); \
    cc4_##ID = 1.414213562F; \
    cc2_##ID = 1.847759065F; \
    cc2c6s_##ID = 1.082392200F; \
    cc2c6a_##ID = -2.613125930F; \
    bias_##ID = 1024.0F; \
    q_##ID = Gq; \
    ws_##ID = &__THPIDCTWorkspace[0] - 2; \
    { \
        ASM_INSN(li itmp2_##ID, 8); \
        ASM_INSN(mtctr itmp2_##ID); \
        ASM_INSN(l_loophead0_##ID: psq_l tmp10_##ID, 0(in_##ID), 0, 5); \
        ASM_INSN(psq_l tmp11_##ID, 0(q_##ID), 0, 0); \
        ASM_INSN(lwz itmp0_##ID, 12(in_##ID)); \
        ASM_INSN(lwz itmp3_##ID, 8(in_##ID)); \
        ASM_INSN(ps_mul tmp10_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(lwz itmp1_##ID, 4(in_##ID)); \
        ASM_INSN(lhz itmp2_##ID, 2(in_##ID)); \
        ASM_INSN(or itmp0_##ID, itmp0_##ID, itmp3_##ID); \
        ASM_INSN(l_loophead1_##ID: cmpwi itmp0_##ID, 0); \
        ASM_INSN(bne l_regularidct_##ID); \
        ASM_INSN(ps_merge00 tmp0_##ID, tmp10_##ID, tmp10_##ID); \
        ASM_INSN(cmpwi itmp1_##ID, 0); \
        ASM_INSN(psq_st tmp0_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(bne l_halfidct_##ID); \
        ASM_INSN(psq_st tmp0_##ID, 16(ws_##ID), 0, 0); \
        ASM_INSN(cmpwi itmp2_##ID, 0); \
        ASM_INSN(psq_st tmp0_##ID, 24(ws_##ID), 0, 0); \
        ASM_INSN(bne l_quarteridct_##ID); \
        ASM_INSN(addi q_##ID, q_##ID, 8*sizeof(f32)); \
        ASM_INSN(psq_stu tmp0_##ID, 32(ws_##ID), 0, 0); \
        ASM_INSN(addi in_##ID, in_##ID, 8*sizeof(THPCoeff)); \
        ASM_INSN(bdnz l_loophead0_##ID); \
        ASM_INSN(b l_loopend_##ID); \
        ASM_INSN(l_quarteridct_##ID: ps_msub tmp2_##ID, tmp10_##ID, cc2_##ID, tmp10_##ID); \
        ASM_INSN(addi in_##ID, in_##ID, 8*sizeof(THPCoeff)); \
        ASM_INSN(ps_merge00 tmp9_##ID, tmp10_##ID, tmp10_##ID); \
        ASM_INSN(addi q_##ID, q_##ID, 8*sizeof(f32)); \
        ASM_INSN(ps_sub tmp1_##ID, cc2_##ID, cc2c6s_##ID); \
        ASM_INSN(lwz itmp1_##ID, 4(in_##ID)); \
        ASM_INSN(ps_msub tmp3_##ID, tmp10_##ID, cc4_##ID, tmp2_##ID); \
        ASM_INSN(lhz itmp2_##ID, 2(in_##ID)); \
        ASM_INSN(ps_merge11 tmp5_##ID, tmp10_##ID, tmp2_##ID); \
        ASM_INSN(psq_l tmp11_##ID, 0(q_##ID), 0, 0); \
        ASM_INSN(ps_nmsub tmp4_##ID, tmp10_##ID, tmp1_##ID, tmp3_##ID); \
        ASM_INSN(ps_add tmp7_##ID, tmp9_##ID, tmp5_##ID); \
        ASM_INSN(psq_l tmp10_##ID, 0(in_##ID), 0, 5); \
        ASM_INSN(ps_merge11 tmp6_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(ps_sub tmp5_##ID, tmp9_##ID, tmp5_##ID); \
        ASM_INSN(lwz itmp0_##ID, 12(in_##ID)); \
        ASM_INSN(ps_add tmp8_##ID, tmp9_##ID, tmp6_##ID); \
        ASM_INSN(lwz itmp3_##ID, 8(in_##ID)); \
        ASM_INSN(ps_sub tmp6_##ID, tmp9_##ID, tmp6_##ID); \
        ASM_INSN(psq_stu tmp7_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_merge10 tmp6_##ID, tmp6_##ID, tmp6_##ID); \
        ASM_INSN(psq_stu tmp8_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_merge10 tmp5_##ID, tmp5_##ID, tmp5_##ID); \
        ASM_INSN(or itmp0_##ID, itmp0_##ID, itmp3_##ID); \
        ASM_INSN(psq_stu tmp6_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_mul tmp10_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(psq_stu tmp5_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(bdnz l_loophead1_##ID); \
        ASM_INSN(b l_loopend_##ID); \
        ASM_INSN(l_halfidct_##ID: psq_l tmp1_##ID, 4(in_##ID), 0, 5); \
        ASM_INSN(psq_l tmp9_##ID, 8(q_##ID), 0, 0); \
        ASM_INSN(addi in_##ID, in_##ID, 8*sizeof(THPCoeff)); \
        ASM_INSN(ps_mul tmp1_##ID, tmp1_##ID, tmp9_##ID); \
        ASM_INSN(addi q_##ID, q_##ID, 8*sizeof(f32)); \
        ASM_INSN(ps_sub tmp3_##ID, tmp10_##ID, tmp1_##ID); \
        ASM_INSN(ps_add tmp2_##ID, tmp10_##ID, tmp1_##ID); \
        ASM_INSN(lwz itmp0_##ID, 12(in_##ID)); \
        ASM_INSN(ps_madd tmp4_##ID, tmp1_##ID, cc4_##ID, tmp3_##ID); \
        ASM_INSN(ps_nmsub tmp5_##ID, tmp1_##ID, cc4_##ID, tmp2_##ID); \
        ASM_INSN(ps_mul tmp8_##ID, tmp3_##ID, cc2_##ID); \
        ASM_INSN(ps_merge00 tmp4_##ID, tmp2_##ID, tmp4_##ID); \
        ASM_INSN(lwz itmp3_##ID, 8(in_##ID)); \
        ASM_INSN(ps_nmsub tmp6_##ID, tmp1_##ID, cc2c6a_##ID, tmp8_##ID); \
        ASM_INSN(ps_merge00 tmp5_##ID, tmp5_##ID, tmp3_##ID); \
        ASM_INSN(lwz itmp1_##ID, 4(in_##ID)); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp2_##ID); \
        ASM_INSN(ps_msub tmp7_##ID, tmp10_##ID, cc2c6s_##ID, tmp8_##ID); \
        ASM_INSN(lhz itmp2_##ID, 2(in_##ID)); \
        ASM_INSN(ps_merge11 tmp2_##ID, tmp2_##ID, tmp6_##ID); \
        ASM_INSN(ps_msub tmp8_##ID, tmp3_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(psq_l tmp10_##ID, 0(in_##ID), 0, 5); \
        ASM_INSN(ps_add tmp9_##ID, tmp4_##ID, tmp2_##ID); \
        ASM_INSN(ps_add tmp7_##ID, tmp8_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp11_##ID, 0(q_##ID), 0, 0); \
        ASM_INSN(ps_merge11 tmp3_##ID, tmp8_##ID, tmp7_##ID); \
        ASM_INSN(ps_sub tmp4_##ID, tmp4_##ID, tmp2_##ID); \
        ASM_INSN(psq_stu tmp9_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp0_##ID, tmp5_##ID, tmp3_##ID); \
        ASM_INSN(ps_sub tmp1_##ID, tmp5_##ID, tmp3_##ID); \
        ASM_INSN(or itmp0_##ID, itmp0_##ID, itmp3_##ID); \
        ASM_INSN(psq_stu tmp0_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_merge10 tmp1_##ID, tmp1_##ID, tmp1_##ID); \
        ASM_INSN(ps_merge10 tmp4_##ID, tmp4_##ID, tmp4_##ID); \
        ASM_INSN(psq_stu tmp1_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_mul tmp10_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(psq_stu tmp4_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(bdnz l_loophead1_##ID); \
        ASM_INSN(b l_loopend_##ID); \
        ASM_INSN(l_regularidct_##ID: psq_l tmp9_##ID, 4(in_##ID), 0, 5); \
        ASM_INSN(psq_l tmp5_##ID, 8(q_##ID), 0, 0); \
        ASM_INSN(ps_mul tmp9_##ID, tmp9_##ID, tmp5_##ID); \
        ASM_INSN(psq_l tmp2_##ID, 8(in_##ID), 0, 5); \
        ASM_INSN(psq_l tmp6_##ID, 16(q_##ID), 0, 0); \
        ASM_INSN(ps_merge01 tmp0_##ID, tmp10_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp3_##ID, 12(in_##ID), 0, 5); \
        ASM_INSN(ps_merge01 tmp1_##ID, tmp9_##ID, tmp10_##ID); \
        ASM_INSN(psq_l tmp7_##ID, 24(q_##ID), 0, 0); \
        ASM_INSN(addi in_##ID, in_##ID, 8*sizeof(THPCoeff)); \
        ASM_INSN(ps_madd tmp4_##ID, tmp2_##ID, tmp6_##ID, tmp0_##ID); \
        ASM_INSN(ps_nmsub tmp5_##ID, tmp2_##ID, tmp6_##ID, tmp0_##ID); \
        ASM_INSN(ps_madd tmp6_##ID, tmp3_##ID, tmp7_##ID, tmp1_##ID); \
        ASM_INSN(ps_nmsub tmp7_##ID, tmp3_##ID, tmp7_##ID, tmp1_##ID); \
        ASM_INSN(addi q_##ID, q_##ID, 8*sizeof(f32)); \
        ASM_INSN(ps_add tmp0_##ID, tmp4_##ID, tmp6_##ID); \
        ASM_INSN(ps_sub tmp3_##ID, tmp4_##ID, tmp6_##ID); \
        ASM_INSN(ps_msub tmp2_##ID, tmp7_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(lwz itmp0_##ID, 12(in_##ID)); \
        ASM_INSN(ps_sub tmp8_##ID, tmp7_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp1_##ID, tmp5_##ID, tmp2_##ID); \
        ASM_INSN(ps_sub tmp2_##ID, tmp5_##ID, tmp2_##ID); \
        ASM_INSN(ps_mul tmp8_##ID, tmp8_##ID, cc2_##ID); \
        ASM_INSN(lwz itmp3_##ID, 8(in_##ID)); \
        ASM_INSN(ps_merge00 tmp1_##ID, tmp0_##ID, tmp1_##ID); \
        ASM_INSN(ps_nmsub tmp6_##ID, tmp5_##ID, cc2c6a_##ID, tmp8_##ID); \
        ASM_INSN(ps_msub tmp4_##ID, tmp7_##ID, cc2c6s_##ID, tmp8_##ID); \
        ASM_INSN(lwz itmp1_##ID, 4(in_##ID)); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp0_##ID); \
        ASM_INSN(ps_merge00 tmp2_##ID, tmp2_##ID, tmp3_##ID); \
        ASM_INSN(lhz itmp2_##ID, 2(in_##ID)); \
        ASM_INSN(ps_madd tmp5_##ID, tmp3_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(ps_merge11 tmp7_##ID, tmp0_##ID, tmp6_##ID); \
        ASM_INSN(psq_l tmp10_##ID, 0(in_##ID), 0, 5); \
        ASM_INSN(ps_sub tmp4_##ID, tmp4_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp3_##ID, tmp1_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp11_##ID, 0(q_##ID), 0, 0); \
        ASM_INSN(ps_merge11 tmp4_##ID, tmp5_##ID, tmp4_##ID); \
        ASM_INSN(ps_sub tmp0_##ID, tmp1_##ID, tmp7_##ID); \
        ASM_INSN(ps_mul tmp10_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(ps_add tmp5_##ID, tmp2_##ID, tmp4_##ID); \
        ASM_INSN(ps_sub tmp6_##ID, tmp2_##ID, tmp4_##ID); \
        ASM_INSN(ps_merge10 tmp5_##ID, tmp5_##ID, tmp5_##ID); \
        ASM_INSN(psq_stu tmp3_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(ps_merge10 tmp0_##ID, tmp0_##ID, tmp0_##ID); \
        ASM_INSN(psq_stu tmp6_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(psq_stu tmp5_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(or itmp0_##ID, itmp0_##ID, itmp3_##ID); \
        ASM_INSN(psq_stu tmp0_##ID, 8(ws_##ID), 0, 0); \
        ASM_INSN(bdnz l_loophead1_##ID); \
        l_loopend_##ID:; \
    } \
    ws_##ID = &__THPIDCTWorkspace[0]; \
    { \
        obase_##ID = Gbase; \
        wid_##ID = Gwid; \
        ASM_INSN(psq_l tmp10_##ID, 8*0*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(slwi off0_##ID, wid_##ID, 3); \
        ASM_INSN(psq_l tmp11_##ID, 8*4*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(slwi xPos_##ID, xPos_##ID, 2); \
        ASM_INSN(psq_l tmp12_##ID, 8*2*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(slwi off1_##ID, wid_##ID, 2); \
        ASM_INSN(ps_add tmp6_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(add off0_##ID, off0_##ID, xPos_##ID); \
        ASM_INSN(psq_l tmp13_##ID, 8*6*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp8_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(add off1_##ID, off0_##ID, off1_##ID); \
        ASM_INSN(ps_add tmp6_##ID, tmp6_##ID, bias_##ID); \
        ASM_INSN(li itmp0_2_##ID, 3); \
        ASM_INSN(ps_add tmp7_##ID, tmp12_##ID, tmp13_##ID); \
        ASM_INSN(add out0_##ID, obase_##ID, off0_##ID); \
        ASM_INSN(ps_sub tmp9_##ID, tmp12_##ID, tmp13_##ID); \
        ASM_INSN(ps_add tmp0_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(add out1_##ID, obase_##ID, off1_##ID); \
        ASM_INSN(ps_add tmp8_##ID, tmp8_##ID, bias_##ID); \
        ASM_INSN(mtctr itmp0_2_##ID); \
        ASM_INSN(l_loophead10_##ID: psq_l tmp4_##ID, 8*1*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_msub tmp9_##ID, tmp9_##ID, cc4_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp5_##ID, 8*3*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp3_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(ps_add tmp1_##ID, tmp8_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp6_##ID, 8*5*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp2_##ID, tmp8_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp7_##ID, 8*7*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp8_##ID, tmp6_##ID, tmp5_##ID); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp5_##ID); \
        ASM_INSN(addi ws_##ID, ws_##ID, 2*sizeof(f32)); \
        ASM_INSN(ps_add tmp9_##ID, tmp4_##ID, tmp7_##ID); \
        ASM_INSN(ps_sub tmp4_##ID, tmp4_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp10_##ID, 8*0*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp7_##ID, tmp9_##ID, tmp8_##ID); \
        ASM_INSN(ps_sub tmp5_##ID, tmp9_##ID, tmp8_##ID); \
        ASM_INSN(ps_add tmp8_##ID, tmp6_##ID, tmp4_##ID); \
        ASM_INSN(psq_l tmp11_##ID, 8*4*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp9_##ID, tmp0_##ID, tmp7_##ID); \
        ASM_INSN(ps_mul tmp8_##ID, tmp8_##ID, cc2_##ID); \
        ASM_INSN(psq_l tmp12_##ID, 8*2*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp23_##ID, tmp0_##ID, tmp7_##ID); \
        ASM_INSN(ps_madd tmp6_##ID, tmp6_##ID, cc2c6a_##ID, tmp8_##ID); \
        ASM_INSN(psq_l tmp13_##ID, 8*6*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(addi off0_##ID, off0_##ID, 2*sizeof(THPSample)); \
        ASM_INSN(psq_st tmp9_##ID, 0(out0_##ID), 0, 6); \
        ASM_INSN(ps_msub tmp4_##ID, tmp4_##ID, cc2c6s_##ID, tmp8_##ID); \
        ASM_INSN(ps_add tmp9_##ID, tmp1_##ID, tmp6_##ID); \
        ASM_INSN(ps_msub tmp5_##ID, tmp5_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(ps_sub tmp22_##ID, tmp1_##ID, tmp6_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 8(out0_##ID), 0, 6); \
        ASM_INSN(ps_add tmp8_##ID, tmp2_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp4_##ID, tmp4_##ID, tmp5_##ID); \
        ASM_INSN(psq_st tmp8_##ID, 16(out0_##ID), 0, 6); \
        ASM_INSN(addi off1_##ID, off1_##ID, 2*sizeof(THPSample)); \
        ASM_INSN(ps_sub tmp9_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(ps_add tmp20_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 24(out0_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp21_##ID, tmp2_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp6_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(psq_st tmp20_##ID, 0(out1_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp8_##ID, tmp10_##ID, tmp11_##ID); \
        ASM_INSN(ps_add tmp6_##ID, tmp6_##ID, bias_##ID); \
        ASM_INSN(psq_st tmp21_##ID, 8(out1_##ID), 0, 6); \
        ASM_INSN(ps_add tmp7_##ID, tmp12_##ID, tmp13_##ID); \
        ASM_INSN(ps_sub tmp9_##ID, tmp12_##ID, tmp13_##ID); \
        ASM_INSN(psq_st tmp22_##ID, 16(out1_##ID), 0, 6); \
        ASM_INSN(add out0_##ID, obase_##ID, off0_##ID); \
        ASM_INSN(ps_add tmp0_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(psq_st tmp23_##ID, 24(out1_##ID), 0, 6); \
        ASM_INSN(ps_add tmp8_##ID, tmp8_##ID, bias_##ID); \
        ASM_INSN(add out1_##ID, obase_##ID, off1_##ID); \
        ASM_INSN(bdnz l_loophead10_##ID); \
        ASM_INSN(psq_l tmp4_##ID, 8*1*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_msub tmp9_##ID, tmp9_##ID, cc4_##ID, tmp7_##ID); \
        ASM_INSN(psq_l tmp5_##ID, 8*3*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp3_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(ps_add tmp1_##ID, tmp8_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp6_##ID, 8*5*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_sub tmp2_##ID, tmp8_##ID, tmp9_##ID); \
        ASM_INSN(psq_l tmp7_##ID, 8*7*sizeof(f32)(ws_##ID), 0, 0); \
        ASM_INSN(ps_add tmp8_##ID, tmp6_##ID, tmp5_##ID); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp9_##ID, tmp4_##ID, tmp7_##ID); \
        ASM_INSN(ps_sub tmp4_##ID, tmp4_##ID, tmp7_##ID); \
        ASM_INSN(ps_add tmp7_##ID, tmp9_##ID, tmp8_##ID); \
        ASM_INSN(ps_sub tmp5_##ID, tmp9_##ID, tmp8_##ID); \
        ASM_INSN(ps_add tmp8_##ID, tmp6_##ID, tmp4_##ID); \
        ASM_INSN(ps_add tmp9_##ID, tmp0_##ID, tmp7_##ID); \
        ASM_INSN(ps_mul tmp8_##ID, tmp8_##ID, cc2_##ID); \
        ASM_INSN(ps_sub tmp23_##ID, tmp0_##ID, tmp7_##ID); \
        ASM_INSN(ps_madd tmp6_##ID, tmp6_##ID, cc2c6a_##ID, tmp8_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 0(out0_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp6_##ID, tmp6_##ID, tmp7_##ID); \
        ASM_INSN(ps_msub tmp4_##ID, tmp4_##ID, cc2c6s_##ID, tmp8_##ID); \
        ASM_INSN(psq_st tmp23_##ID, 24(out1_##ID), 0, 6); \
        ASM_INSN(ps_add tmp9_##ID, tmp1_##ID, tmp6_##ID); \
        ASM_INSN(ps_msub tmp5_##ID, tmp5_##ID, cc4_##ID, tmp6_##ID); \
        ASM_INSN(ps_sub tmp22_##ID, tmp1_##ID, tmp6_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 8(out0_##ID), 0, 6); \
        ASM_INSN(ps_add tmp8_##ID, tmp2_##ID, tmp5_##ID); \
        ASM_INSN(ps_add tmp4_##ID, tmp4_##ID, tmp5_##ID); \
        ASM_INSN(psq_st tmp8_##ID, 16(out0_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp9_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(psq_st tmp22_##ID, 16(out1_##ID), 0, 6); \
        ASM_INSN(ps_add tmp20_##ID, tmp3_##ID, tmp4_##ID); \
        ASM_INSN(psq_st tmp9_##ID, 24(out0_##ID), 0, 6); \
        ASM_INSN(ps_sub tmp21_##ID, tmp2_##ID, tmp5_##ID); \
        ASM_INSN(psq_st tmp20_##ID, 0(out1_##ID), 0, 6); \
        ASM_INSN(psq_st tmp21_##ID, 8(out1_##ID), 0, 6); \
    } \
}

#define THP_HUFF_DECODE_TAB_VARS(ID) \
    register u32 tmp2_3_##ID; \
    register u32 mask_##ID; \
    register u32 tmp2_2_##ID; \
    register u32 maxcodebase_2_##ID; \
    register u32 tmp2_##ID; \
    register u32 maxcodebase_##ID; \
    register s32 tmp_##ID; \
    register u32 increment_##ID; \
    register s32 cb_##ID; \
    register u32 cnt_##ID; \
    register s32 code_##ID; \
    register THPFileInfo *hinfo_##ID; \
    register THPHuffmanTab *htab_##ID;

#define THP_HUFF_DECODE_TAB(ID, INFO_, H_, RES_) { \
    htab_##ID = (H_); \
    hinfo_##ID = (INFO_); \
    ASM_INSN(lwz cnt_##ID, hinfo_##ID->cnt); \
    ASM_INSN(addi increment_##ID, htab_##ID, 32); \
    ASM_INSN(lwz cb_##ID, hinfo_##ID->currByte); \
    ASM_INSN(addi code_##ID, cnt_##ID, 4); \
    ASM_INSN(cmpwi cnt_##ID, 28); \
    ASM_INSN(rlwnm tmp_##ID, cb_##ID, code_##ID, 27, 31); \
    ASM_INSN(bgt l_notenoughbits_##ID); \
    ASM_INSN(lbzx code_##ID, htab_##ID, tmp_##ID); \
    ASM_INSN(lbzx increment_##ID, increment_##ID, tmp_##ID); \
    ASM_INSN(cmpwi code_##ID, 0xFF); \
    ASM_INSN(beq l_failedcheckenoughbits_##ID); \
    ASM_INSN(add cnt_##ID, cnt_##ID, increment_##ID); \
    ASM_INSN(stw cnt_##ID, hinfo_##ID->cnt); \
    l_done_##ID: goto l_hdone_##ID; \
    { \
    l_failedcheckenoughbits_##ID: \
        maxcodebase_##ID = (u32) & (htab_##ID->maxCode); \
        cnt_##ID += 5; \
        ASM_INSN(li tmp2_##ID, sizeof(s32)*(5)); \
        ASM_INSN(li code_##ID, 5); \
        ASM_INSN(add maxcodebase_##ID, maxcodebase_##ID, tmp2_##ID); \
        ASM_INSN(l_while_start_##ID: cmpwi cnt_##ID, 33); \
        ASM_INSN(slwi tmp_##ID, tmp_##ID, 1); \
        ASM_INSN(beq l_fceb_faster_##ID); \
        ASM_INSN(rlwnm increment_##ID, cb_##ID, cnt_##ID, 31, 31); \
        ASM_INSN(lwzu tmp2_##ID, 4(maxcodebase_##ID)); \
        ASM_INSN(or tmp_##ID, tmp_##ID, increment_##ID); \
        ASM_INSN(addi cnt_##ID, cnt_##ID, 1); \
        ASM_INSN(b l_while_check_##ID); \
        ASM_INSN(l_fceb_faster_##ID: lwz increment_##ID, hinfo_##ID->c); \
        ASM_INSN(li cnt_##ID, 1); \
        ASM_INSN(lwzu cb_##ID, 4(increment_##ID)); \
        ASM_INSN(lwzu tmp2_##ID, 4(maxcodebase_##ID)); \
        ASM_INSN(stw increment_##ID, hinfo_##ID->c); \
        ASM_INSN(rlwimi tmp_##ID, cb_##ID, 1, 31, 31); \
        ASM_INSN(stw cb_##ID, hinfo_##ID->currByte); \
        ASM_INSN(b l_fl_while_check_##ID); \
        ASM_INSN(l_fl_while_start_##ID: slwi tmp_##ID, tmp_##ID, 1); \
        ASM_INSN(rlwnm increment_##ID, cb_##ID, cnt_##ID, 31, 31); \
        ASM_INSN(lwzu tmp2_##ID, 4(maxcodebase_##ID)); \
        ASM_INSN(or tmp_##ID, tmp_##ID, increment_##ID); \
        ASM_INSN(l_fl_while_check_##ID: cmpw tmp_##ID, tmp2_##ID); \
        ASM_INSN(addi cnt_##ID, cnt_##ID, 1); \
        ASM_INSN(addi code_##ID, code_##ID, 1); \
        ASM_INSN(bgt l_fl_while_start_##ID); \
        ASM_INSN(b l_fceb_done_##ID); \
        ASM_INSN(l_while_check_##ID: cmpw tmp_##ID, tmp2_##ID); \
        ASM_INSN(addi code_##ID, code_##ID, 1); \
        ASM_INSN(bgt l_while_start_##ID); \
    } \
l_fceb_done_##ID: \
    hinfo_##ID->cnt = cnt_##ID; \
    code_##ID = (htab_##ID->Vij[(s32)(tmp_##ID + htab_##ID->valPtr[code_##ID])]); goto l_hdone_##ID; \
    ASM_INSN(l_notenoughbits_##ID: cmpwi cnt_##ID, 33); \
    ASM_INSN(lwz tmp_##ID, hinfo_##ID->c); \
    ASM_INSN(beq l_getfullword_##ID); \
    ASM_INSN(cmpwi cnt_##ID, 32); \
    ASM_INSN(rlwnm code_##ID, cb_##ID, code_##ID, 27, 31); \
    ASM_INSN(beq l_1bitleft_##ID); \
    ASM_INSN(lbzx tmp_##ID, htab_##ID, code_##ID); \
    ASM_INSN(lbzx increment_##ID, increment_##ID, code_##ID); \
    ASM_INSN(cmpwi tmp_##ID, 0xFF); \
    ASM_INSN(add code_##ID, cnt_##ID, increment_##ID); \
    ASM_INSN(beq l_failedchecknobits0_##ID); \
    ASM_INSN(cmpwi code_##ID, 33); \
    ASM_INSN(stw code_##ID, hinfo_##ID->cnt); \
    ASM_INSN(bgt l_failedchecknobits1_##ID); \
    code_##ID = tmp_##ID; goto l_hdone_##ID; \
    ASM_INSN(l_1bitleft_##ID: lwzu cb_##ID, 4(tmp_##ID)); \
    ASM_INSN(stw tmp_##ID, hinfo_##ID->c); \
    ASM_INSN(rlwimi code_##ID, cb_##ID, 4, 28, 31); \
    ASM_INSN(lbzx tmp_##ID, htab_##ID, code_##ID); \
    ASM_INSN(lbzx increment_##ID, increment_##ID, code_##ID); \
    ASM_INSN(stw cb_##ID, hinfo_##ID->currByte); \
    ASM_INSN(cmpwi tmp_##ID, 0xFF); \
    ASM_INSN(stw increment_##ID, hinfo_##ID->cnt); \
    ASM_INSN(beq l_dammitread4_##ID); \
    code_##ID = tmp_##ID; goto l_hdone_##ID; \
l_dammitread4_##ID: { \
    maxcodebase_2_##ID = (u32) & (htab_##ID->maxCode); \
    ASM_INSN(li cnt_##ID, sizeof(s32)*5); \
    ASM_INSN(add maxcodebase_2_##ID, maxcodebase_2_##ID, cnt_##ID); \
    ASM_INSN(slwi tmp_##ID, code_##ID, 32-5); \
    ASM_INSN(li cnt_##ID, 5); \
    ASM_INSN(rlwimi tmp_##ID, cb_##ID, 32-1, 1, 31); \
    ASM_INSN(l_dr4_while_start_##ID: subfic cb_##ID, cnt_##ID, 31); \
    ASM_INSN(lwzu tmp2_2_##ID, 4(maxcodebase_2_##ID)); \
    ASM_INSN(srw code_##ID, tmp_##ID, cb_##ID); \
    ASM_INSN(l_dr4_while_check_##ID: cmpw code_##ID, tmp2_2_##ID); \
    ASM_INSN(addi cnt_##ID, cnt_##ID, 1); \
    ASM_INSN(bgt l_dr4_while_start_##ID); \
} \
    hinfo_##ID->cnt = cnt_##ID; \
l_code_plus_vp_cnt_##ID: \
    code_##ID = (htab_##ID->Vij[(s32)(code_##ID + htab_##ID->valPtr[cnt_##ID])]); goto l_hdone_##ID; \
l_getfullword_##ID: \
    ASM_INSN(lwzu cb_##ID, 4(tmp_##ID)); \
    ASM_INSN(rlwinm code_##ID, cb_##ID, 5, 27, 31); \
    ASM_INSN(stw tmp_##ID, hinfo_##ID->c); \
    ASM_INSN(lbzx cnt_##ID, htab_##ID, code_##ID); \
    ASM_INSN(lbzx increment_##ID, increment_##ID, code_##ID); \
    ASM_INSN(cmpwi cnt_##ID, 0xFF); \
    ASM_INSN(stw cb_##ID, hinfo_##ID->currByte); \
    ASM_INSN(addi increment_##ID, increment_##ID, 1); \
    ASM_INSN(beq l_failedcheckenoughbits_updated_##ID); \
    ASM_INSN(stw increment_##ID, hinfo_##ID->cnt); \
    code_##ID = (s32)cnt_##ID; goto l_hdone_##ID; \
l_failedcheckenoughbits_updated_##ID: \
    cnt_##ID = 5; \
    do { \
        ASM_INSN(subfic tmp_##ID, cnt_##ID, 31); \
        ASM_INSN(addi cnt_##ID, cnt_##ID, 1); \
        ASM_INSN(srw code_##ID, cb_##ID, tmp_##ID); \
    } while (code_##ID > htab_##ID->maxCode[cnt_##ID]); \
    hinfo_##ID->cnt = cnt_##ID + 1; \
    goto l_code_plus_vp_cnt_##ID; \
l_failedchecknobits0_##ID: \
l_failedchecknobits1_##ID: \
{ \
    mask_##ID = 0xFFFFFFFF << (33 - cnt_##ID); \
    code_##ID = (s32)(cb_##ID & (~mask_##ID)); \
    mask_##ID = (u32) & (htab_##ID->maxCode); \
    ASM_INSN(lwz tmp_##ID, hinfo_##ID->c); \
    ASM_INSN(subfic tmp2_3_##ID, cnt_##ID, 33); \
    ASM_INSN(addi cnt_##ID, tmp2_3_##ID, 1); \
    ASM_INSN(slwi tmp2_3_##ID, tmp2_3_##ID, 2); \
    ASM_INSN(lwzu cb_##ID, 4(tmp_##ID)); \
    ASM_INSN(add mask_##ID, mask_##ID, tmp2_3_##ID); \
    ASM_INSN(stw tmp_##ID, hinfo_##ID->c); \
    ASM_INSN(slwi code_##ID, code_##ID, 1); \
    ASM_INSN(stw cb_##ID, hinfo_##ID->currByte); \
    ASM_INSN(rlwimi code_##ID, cb_##ID, 1, 31, 31); \
    ASM_INSN(lwzu tmp2_3_##ID, 4(mask_##ID)); \
    ASM_INSN(li tmp_##ID, 2); \
    ASM_INSN(b l_fcnb1_while_check_##ID); \
    ASM_INSN(l_fcnb1_while_start_##ID: slwi code_##ID, code_##ID, 1); \
    ASM_INSN(addi cnt_##ID, cnt_##ID, 1); \
    ASM_INSN(lwzu tmp2_3_##ID, 4(mask_##ID)); \
    ASM_INSN(add code_##ID, code_##ID, increment_##ID); \
    ASM_INSN(addi tmp_##ID, tmp_##ID, 1); \
    ASM_INSN(l_fcnb1_while_check_##ID: cmpw code_##ID, tmp2_3_##ID); \
    ASM_INSN(rlwnm increment_##ID, cb_##ID, tmp_##ID, 31, 31); \
    ASM_INSN(bgt l_fcnb1_while_start_##ID); \
} \
    hinfo_##ID->cnt = (u32)tmp_##ID; \
    code_##ID = (htab_##ID->Vij[(s32)(code_##ID + htab_##ID->valPtr[cnt_##ID])]); goto l_hdone_##ID; \
l_hdone_##ID: \
RES_ = code_##ID; \
}

void __THPDecompressYUV(void* tileY, void* tileU, void* tileV)
{
    u16 currentY, targetY;
    __THPInfo->dLC[0] = tileY;
    __THPInfo->dLC[1] = tileU;
    __THPInfo->dLC[2] = tileV;

    currentY = __THPInfo->decompressedY;
    targetY = __THPInfo->yPixelSize;

    THP_GQR_SETUP();
    __THPPrepBitStream();

    if (__THPInfo->xPixelSize == 512 && targetY == 448) {
        while (currentY < targetY) {
            __THPDecompressiMCURow512x448();
            currentY += 16;
        }
    } else if (__THPInfo->xPixelSize == 640 && targetY == 480) {
        while (currentY < targetY) {
            __THPDecompressiMCURow640x480();
            currentY += 16;
        }
    } else {
        while (currentY < targetY) {
            __THPDecompressiMCURowNxN();
            currentY += 16;
        }
    }

    THP_GQR_RESTORE();
}

static void __THPDecompressiMCURow512x448(void)
{
    THP_IDCT_NOYPOS_VARS(c1)
    THP_IDCT_NOYPOS_VARS(c2)
    THP_IDCT_Y8_VARS(c5)
    THP_IDCT_Y8_VARS(c6)
    THP_IDCT_NOYPOS_VARS(c3)
    THP_IDCT_NOYPOS_VARS(c4)
    u8 cl_num;
    u32 x_pos;
    THPComponent* comp;

    LCQueueWait(3);

    for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[0]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
        __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
        __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

        comp = &__THPInfo->components[0];
        Gbase = __THPLCWork512[0];
        Gwid = 512;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos = (u32)(cl_num * 16);
        THP_IDCT_NOYPOS(c1, __THPMCUBuffer[0], x_pos);
        THP_IDCT_NOYPOS(c2, __THPMCUBuffer[1], x_pos + 8);
        THP_IDCT_Y8(c5, __THPMCUBuffer[2], x_pos);
        THP_IDCT_Y8(c6, __THPMCUBuffer[3], x_pos + 8);

        comp = &__THPInfo->components[1];
        Gbase = __THPLCWork512[1];
        Gwid = 256;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos /= 2;
        THP_IDCT_NOYPOS(c3, __THPMCUBuffer[4], x_pos);
        comp = &__THPInfo->components[2];
        Gbase = __THPLCWork512[2];
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        THP_IDCT_NOYPOS(c4, __THPMCUBuffer[5], x_pos);

        if (__THPInfo->RST != 0) {
            if ((--__THPInfo->currMCU) == 0) {
                __THPInfo->currMCU = __THPInfo->nMCU;
                __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                if (__THPInfo->cnt > 33) {
                    __THPInfo->cnt = 33;
                }

                __THPInfo->components[0].predDC = 0;
                __THPInfo->components[1].predDC = 0;
                __THPInfo->components[2].predDC = 0;
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork512[0], 0x2000);
    LCStoreData(__THPInfo->dLC[1], __THPLCWork512[1], 0x800);
    LCStoreData(__THPInfo->dLC[2], __THPLCWork512[2], 0x800);

    __THPInfo->dLC[0] += 0x2000;
    __THPInfo->dLC[1] += 0x800;
    __THPInfo->dLC[2] += 0x800;
}

static void __THPDecompressiMCURow640x480(void)
{
    THP_IDCT_NOYPOS_VARS(c1)
    THP_IDCT_NOYPOS_VARS(c2)
    THP_IDCT_Y8_VARS(c5)
    THP_IDCT_Y8_VARS(c6)
    THP_IDCT_NOYPOS_VARS(c3)
    THP_IDCT_NOYPOS_VARS(c4)
    u8 cl_num;
    u32 x_pos;
    THPComponent* comp;

    LCQueueWait(3);

    {
        for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
            THPFileInfo* um = __THPInfo;
            __THPHuffDecodeDCTCompY(um, __THPMCUBuffer[0]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
            __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
            __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
            __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

            comp = &__THPInfo->components[0];
            Gbase = __THPLCWork640[0];
            Gwid = 640;
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            x_pos = (u32)(cl_num * 16);
            THP_IDCT_NOYPOS(c1, __THPMCUBuffer[0], x_pos);
            THP_IDCT_NOYPOS(c2, __THPMCUBuffer[1], x_pos + 8);
            THP_IDCT_Y8(c5, __THPMCUBuffer[2], x_pos);
            THP_IDCT_Y8(c6, __THPMCUBuffer[3], x_pos + 8);

            comp = &__THPInfo->components[1];
            Gbase = __THPLCWork640[1];
            Gwid = 320;
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            x_pos /= 2;
            THP_IDCT_NOYPOS(c3, __THPMCUBuffer[4], x_pos);

            comp = &__THPInfo->components[2];
            Gbase = __THPLCWork640[2];
            Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
            THP_IDCT_NOYPOS(c4, __THPMCUBuffer[5], x_pos);

            if (__THPInfo->RST != 0) {
                __THPInfo->currMCU--;
                if (__THPInfo->currMCU == 0) {
                    __THPInfo->currMCU = __THPInfo->nMCU;

                    __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                    if (__THPInfo->cnt > 32) {
                        __THPInfo->cnt = 33;
                    }

                    __THPInfo->components[0].predDC = 0;
                    __THPInfo->components[1].predDC = 0;
                    __THPInfo->components[2].predDC = 0;
                }
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork640[0], 0x2800);
    LCStoreData(__THPInfo->dLC[1], __THPLCWork640[1], 0xA00);
    LCStoreData(__THPInfo->dLC[2], __THPLCWork640[2], 0xA00);

    __THPInfo->dLC[0] += 0x2800;
    __THPInfo->dLC[1] += 0xA00;
    __THPInfo->dLC[2] += 0xA00;
}

static void __THPDecompressiMCURowNxN(void)
{
    THP_IDCT_NOYPOS_VARS(c1)
    THP_IDCT_NOYPOS_VARS(c2)
    THP_IDCT_Y8_VARS(c5)
    THP_IDCT_Y8_VARS(c6)
    THP_IDCT_NOYPOS_VARS(c3)
    THP_IDCT_NOYPOS_VARS(c4)
    u8 cl_num;
    u32 x_pos, x;
    THPComponent* comp;

    x = __THPInfo->xPixelSize;

    LCQueueWait(3);

    for (cl_num = 0; cl_num < __THPInfo->MCUsPerRow; cl_num++) {
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[0]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[1]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[2]);
        __THPHuffDecodeDCTCompY(__THPInfo, __THPMCUBuffer[3]);
        __THPHuffDecodeDCTCompU(__THPInfo, __THPMCUBuffer[4]);
        __THPHuffDecodeDCTCompV(__THPInfo, __THPMCUBuffer[5]);

        comp = &__THPInfo->components[0];
        Gbase = __THPLCWork640[0];
        Gwid = x;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos = (u32)(cl_num * 16);
        THP_IDCT_NOYPOS(c1, __THPMCUBuffer[0], x_pos);
        THP_IDCT_NOYPOS(c2, __THPMCUBuffer[1], x_pos + 8);
        THP_IDCT_Y8(c5, __THPMCUBuffer[2], x_pos);
        THP_IDCT_Y8(c6, __THPMCUBuffer[3], x_pos + 8);

        comp = &__THPInfo->components[1];
        Gbase = __THPLCWork640[1];
        Gwid = x / 2;
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        x_pos /= 2;
        THP_IDCT_NOYPOS(c3, __THPMCUBuffer[4], x_pos);

        comp = &__THPInfo->components[2];
        Gbase = __THPLCWork640[2];
        Gq = __THPInfo->quantTabs[comp->quantizationTableSelector];
        THP_IDCT_NOYPOS(c4, __THPMCUBuffer[5], x_pos);

        if (__THPInfo->RST != 0) {
            __THPInfo->currMCU--;
            if (__THPInfo->currMCU == 0) {
                __THPInfo->currMCU = __THPInfo->nMCU;
                __THPInfo->cnt = 1 + ((__THPInfo->cnt + 6) & 0xFFFFFFF8);

                if (__THPInfo->cnt > 32) {
                    __THPInfo->cnt = 33;
                }

                __THPInfo->components[0].predDC = 0;
                __THPInfo->components[1].predDC = 0;
                __THPInfo->components[2].predDC = 0;
            }
        }
    }

    LCStoreData(__THPInfo->dLC[0], __THPLCWork640[0],
                ((4 * sizeof(u8) * 64) * (x / 16)));
    LCStoreData(__THPInfo->dLC[1], __THPLCWork640[1],
                ((sizeof(u8) * 64) * (x / 16)));
    LCStoreData(__THPInfo->dLC[2], __THPLCWork640[2],
                ((sizeof(u8) * 64) * (x / 16)));
    __THPInfo->dLC[0] += ((4 * sizeof(u8) * 64) * (x / 16));
    __THPInfo->dLC[1] += ((sizeof(u8) * 64) * (x / 16));
    __THPInfo->dLC[2] += ((sizeof(u8) * 64) * (x / 16));
}

static void __THPHuffDecodeDCTCompY(register THPFileInfo* info, THPCoeff* block)
{
    THP_HUFF_DECODE_TAB_VARS(c1)
    {
        register s32 t;
        THPCoeff dc;
        register THPCoeff diff;

        __dcbz((void*)block, 0);
        THP_HUFF_DECODE_TAB(c1, info, Ydchuff, t);
        __dcbz((void*)block, 32);
        diff = 0;
        __dcbz((void*)block, 64);

        if (t) {
            {
                register s32 v;
                register u32 cb;
                register u32 cnt;
                register u32 cnt33;
                register u32 tmp;
                register u32 cnt1;
                register u32 tmp1;
                ASM_INSN(lwz cnt, info->cnt);
                ASM_INSN(subfic cnt33, cnt, 33);
                ASM_INSN(lwz cb, info->currByte);
                ASM_INSN(subfc. tmp, cnt33, t);
                ASM_INSN(subi cnt1, cnt, 1);
                ASM_INSN(bgt l_notenoughbitsdiff);
                ASM_INSN(add v, cnt, t);
                ASM_INSN(slw cnt, cb, cnt1);
                ASM_INSN(stw v, info->cnt);
                ASM_INSN(subfic v, t, 32);
                ASM_INSN(srw diff, cnt, v);

                ASM_INSN(b l_donediff);
                ASM_INSN(l_notenoughbitsdiff: lwz tmp1, info->c);
                ASM_INSN(slw v, cb, cnt1);
                ASM_INSN(lwzu cb, 4(tmp1));
                ASM_INSN(addi tmp, tmp, 1);
                ASM_INSN(stw cb, info->currByte);
                ASM_INSN(srw cb, cb, cnt33);
                ASM_INSN(stw tmp1, info->c);
                ASM_INSN(add v, cb, v);
                ASM_INSN(stw tmp, info->cnt);
                ASM_INSN(subfic tmp, t, 32);
                ASM_INSN(srw diff, v, tmp);
                l_donediff:;
            }

            if (__cntlzw((u32)diff) > 32 - t) {
                diff += ((0xFFFFFFFF << t) + 1);
            }
        };

        __dcbz((void*)block, 96);
        dc = (s16)(info->components[0].predDC + diff);
        block[0] = info->components[0].predDC = dc;
    }

    {
        register s32 k;
        register s32 code;
        register u32 cnt;
        register u32 cb;
        register u32 increment;
        register s32 tmp;
        register THPHuffmanTab* h = Yachuff;

#define cnt4 code
        ASM_INSN(lwz cnt, info->cnt);
        ASM_INSN(addi increment, h, 32);
        ASM_INSN(lwz cb, info->currByte);

        for (k = 1; k < 64; k++)
        {
            register s32 ssss;
            register s32 rrrr;

            ASM_INSN(addi cnt4, cnt, 4);
            ASM_INSN(cmpwi cnt, 28);
            ASM_INSN(rlwnm tmp, cb, cnt4, 27, 31);
            ASM_INSN(bgt l_notenoughbits);
            ASM_INSN(lbzx ssss, h, tmp);
            ASM_INSN(lbzx code, increment, tmp);
            ASM_INSN(cmpwi ssss, 0xFF);
            ASM_INSN(beq l_failedcheckenoughbits);
            ASM_INSN(add cnt, cnt, code);
            ASM_INSN(b l_donedecodetab);

            {
                register u32 maxcodebase;
                register u32 tmp2;

            l_failedcheckenoughbits:
                cnt += 5;
                maxcodebase = (u32) & (h->maxCode);
                ASM_INSN(li tmp2, sizeof(s32)*(5));
                ASM_INSN(li code, 5);
                ASM_INSN(add maxcodebase, maxcodebase, tmp2);
                ASM_INSN(l_while_start: cmpwi cnt, 33);
                ASM_INSN(slwi tmp, tmp, 1);
                ASM_INSN(beq l_fceb_faster);
                ASM_INSN(rlwnm ssss, cb, cnt, 31, 31);
                ASM_INSN(lwzu tmp2, 4(maxcodebase));
                ASM_INSN(or tmp, tmp, ssss);
                ASM_INSN(addi cnt, cnt, 1);
                ASM_INSN(b l_while_check);
                ASM_INSN(l_fceb_faster: lwz ssss, info->c);
                ASM_INSN(li cnt, 1);
                ASM_INSN(lwzu cb, 4(ssss));
                ASM_INSN(lwzu tmp2, 4(maxcodebase));
                ASM_INSN(stw ssss, info->c);
                ASM_INSN(rlwimi tmp, cb, 1, 31, 31);
                ASM_INSN(b l_fl_while_check);
                ASM_INSN(l_fl_while_start: slwi tmp, tmp, 1);
                ASM_INSN(rlwnm ssss, cb, cnt, 31, 31);
                ASM_INSN(lwzu tmp2, 4(maxcodebase));
                ASM_INSN(or tmp, tmp, ssss);
                ASM_INSN(l_fl_while_check: cmpw tmp, tmp2);
                ASM_INSN(addi cnt, cnt, 1);
                ASM_INSN(addi code, code, 1);
                ASM_INSN(bgt l_fl_while_start);
                ASM_INSN(b l_fceb_done);
                ASM_INSN(l_while_check: cmpw tmp, tmp2);
                ASM_INSN(addi code, code, 1);
                ASM_INSN(bgt l_while_start);
            }
        l_fceb_done:
            ssss = (h->Vij[(s32)(tmp + h->valPtr[code])]);
            goto l_donedecodetab;

        l_notenoughbits:
            ASM_INSN(cmpwi cnt, 33);
            ASM_INSN(lwz tmp, info->c);
            ASM_INSN(beq l_getfullword);
            ASM_INSN(cmpwi cnt, 32);
            ASM_INSN(rlwnm code, cb, cnt4, 27, 31);
            ASM_INSN(beq l_1bitleft);
            ASM_INSN(lbzx ssss, h, code);
            ASM_INSN(lbzx rrrr, increment, code);
            ASM_INSN(cmpwi ssss, 0xFF);
            ASM_INSN(add code, cnt, rrrr);
            ASM_INSN(beq l_failedchecknobits0);
            ASM_INSN(cmpwi code, 33);
            ASM_INSN(bgt l_failedchecknobits1);
            cnt = (u32)code;
            goto l_donedecodetab;

        l_getfullword: {
            ASM_INSN(lwzu cb, 4(tmp));
            ASM_INSN(rlwinm code, cb, 5, 27, 31);
            ASM_INSN(stw tmp, info->c);
            ASM_INSN(lbzx ssss, h, code);
            ASM_INSN(lbzx tmp, increment, code);
            ASM_INSN(cmpwi ssss, 0xFF);
            ASM_INSN(addi cnt, tmp, 1);
            ASM_INSN(beq l_failedcheckenoughbits_updated);
        }
            goto l_donedecodetab;

        l_failedcheckenoughbits_updated:
            ssss = 5;
            do {
                ASM_INSN(subfic tmp, ssss, 31);
                ASM_INSN(addi ssss, ssss, 1);
                ASM_INSN(srw code, cb, tmp);
            } while (code > h->maxCode[ssss]);

            cnt = (u32)(ssss + 1);
            ssss = (h->Vij[(s32)(code + h->valPtr[ssss])]);

            goto l_donedecodetab;

        l_1bitleft:
            ASM_INSN(lwzu cb, 4(tmp));
            ASM_INSN(stw tmp, info->c);
            ASM_INSN(rlwimi code, cb, 4, 28, 31);
            ASM_INSN(lbzx ssss, h, code);
            ASM_INSN(lbzx cnt, increment, code);
            ASM_INSN(cmpwi ssss, 0xFF);
            ASM_INSN(beq l_dammitread4);

            goto l_donedecodetab;

        l_dammitread4: {
            register u32 maxcodebase = (u32) & (h->maxCode);
            register u32 tmp2;

            ASM_INSN(li cnt, sizeof(s32)*5);
            ASM_INSN(add maxcodebase, maxcodebase, cnt);
            ASM_INSN(slwi tmp, code, 32-5);
            ASM_INSN(li cnt, 5);
            ASM_INSN(rlwimi tmp, cb, 32-1, 1, 31);
            ASM_INSN(l_dr4_while_start: subfic ssss, cnt, 31);
            ASM_INSN(lwzu tmp2, 4(maxcodebase));
            ASM_INSN(srw code, tmp, ssss);
            ASM_INSN(l_dr4_while_check: cmpw code, tmp2);
            ASM_INSN(addi cnt, cnt, 1);
            ASM_INSN(bgt l_dr4_while_start);
        }
            ssss = (h->Vij[(s32)(code + h->valPtr[cnt])]);
            goto l_donedecodetab;

        l_failedchecknobits0:
        l_failedchecknobits1:
        l_realfailedchecknobits: {
            register u32 mask = 0xFFFFFFFF << (33 - cnt);
            register u32 tmp2;
            register u32 tmp3;
            code = (s32)(cb & (~mask));
            mask = (u32) & (h->maxCode);
            ASM_INSN(lwz tmp, info->c);
            ASM_INSN(subfic tmp2, cnt, 33);
            ASM_INSN(addi tmp3, tmp2, 1);
            ASM_INSN(slwi tmp2, tmp2, 2);
            ASM_INSN(lwzu cb, 4(tmp));
            ASM_INSN(add mask, mask, tmp2);
            ASM_INSN(stw tmp, info->c);
            ASM_INSN(slwi code, code, 1);
            ASM_INSN(rlwimi code, cb, 1, 31, 31);
            ASM_INSN(lwzu tmp2, 4(mask));
            ASM_INSN(li cnt, 2);
            ASM_INSN(b l_fcnb1_while_check);
            ASM_INSN(l_fcnb1_while_start: slwi code, code, 1);
            ASM_INSN(addi tmp3, tmp3, 1);
            ASM_INSN(lwzu tmp2, 4(mask));
            ASM_INSN(add code, code, rrrr);
            ASM_INSN(addi cnt, cnt, 1);
            ASM_INSN(l_fcnb1_while_check: cmpw code, tmp2);
            ASM_INSN(rlwnm rrrr, cb, cnt, 31, 31);
            ASM_INSN(bgt l_fcnb1_while_start);
            ssss = (h->Vij[(s32)(code + h->valPtr[tmp3])]);
        }

            goto l_donedecodetab;

        l_donedecodetab:
            ASM_INSN(andi. rrrr, ssss, 15);
            ASM_INSN(srawi ssss, ssss, 4);
            ASM_INSN(beq l_recv_ssss_zero);

            {
                k += ssss;
                {
                    register s32 v;
#define cnt33 code
                    register u32 cnt1;
                    register u32 tmp1;
                    ASM_INSN(subfic cnt33, cnt, 33);
                    ASM_INSN(subfc. tmp, cnt33, rrrr);
                    ASM_INSN(subi cnt1, cnt, 1);
                    ASM_INSN(bgt l_recvnotenoughbits);
                    ASM_INSN(add cnt, cnt, rrrr);
                    ASM_INSN(slw tmp1, cb, cnt1);
                    ASM_INSN(subfic v, rrrr, 32);
                    ASM_INSN(srw ssss, tmp1, v);

                    ASM_INSN(b l_recvdone);
                    ASM_INSN(l_recvnotenoughbits: lwz tmp1, info->c);
                    ASM_INSN(slw v, cb, cnt1);
                    ASM_INSN(lwzu cb, 4(tmp1));
                    ASM_INSN(addi cnt, tmp, 1);
                    ASM_INSN(stw tmp1, info->c);
                    ASM_INSN(srw tmp1, cb, cnt33);
                    ASM_INSN(add v, tmp1, v);
                    ASM_INSN(subfic tmp, rrrr, 32);
                    ASM_INSN(srw ssss, v, tmp);
                    l_recvdone:;
                }

#undef cnt33

                if (__cntlzw((u32)ssss) > 32 - rrrr) {
                    ssss += ((0xFFFFFFFF << rrrr) + 1);
                }

                block[__THPJpegNaturalOrder[k]] = (s16)ssss;
                goto l_recv_end;
            }

            {
            l_recv_ssss_zero:
                if (ssss != 15) {
                    break;
                }

                k += 15;
            };

        l_recv_end:
            (void)0;
        }
        info->cnt = cnt;
        info->currByte = cb;
    }
#undef cnt4
}

static void __THPHuffDecodeDCTCompU(register THPFileInfo* info, THPCoeff* block)
{
    THP_HUFF_DECODE_TAB_VARS(c1)
    THP_HUFF_DECODE_TAB_VARS(c2)
    register s32 t;
    register THPCoeff diff;
    THPCoeff dc;
    register s32 v;
    register u32 cb;
    register u32 cnt;
    register u32 cnt33;
    register u32 tmp;
    register u32 cnt1;
    register u32 tmp1;
    register s32 k;
    register s32 ssss;
    register s32 rrrr;

    __dcbz((void*)block, 0);
    THP_HUFF_DECODE_TAB(c1, info, Udchuff, t);
    __dcbz((void*)block, 32);
    diff = 0;
    __dcbz((void*)block, 64);

    if (t) {
        ASM_INSN(lwz cnt, info->cnt);
        ASM_INSN(subfic cnt33, cnt, 33);
        ASM_INSN(lwz cb, info->currByte);
        ASM_INSN(subfc. tmp, cnt33, t);
        ASM_INSN(subi cnt1, cnt, 1);
        ASM_INSN(bgt l_notenoughbitsdiff);
        ASM_INSN(add v, cnt, t);
        ASM_INSN(slw cnt, cb, cnt1);
        ASM_INSN(stw v, info->cnt);
        ASM_INSN(subfic v, t, 32);
        ASM_INSN(srw diff, cnt, v);

        ASM_INSN(b l_donediff);
        ASM_INSN(l_notenoughbitsdiff: lwz tmp1, info->c);
        ASM_INSN(slw v, cb, cnt1);
        ASM_INSN(lwzu cb, 4(tmp1));
        ASM_INSN(addi tmp, tmp, 1);
        ASM_INSN(stw cb, info->currByte);
        ASM_INSN(srw cb, cb, cnt33);
        ASM_INSN(stw tmp1, info->c);
        ASM_INSN(add v, cb, v);
        ASM_INSN(stw tmp, info->cnt);
        ASM_INSN(subfic tmp, t, 32);
        ASM_INSN(srw diff, v, tmp);
        l_donediff:;

        if (__cntlzw((u32)diff) > 32 - t) {
            diff += ((0xFFFFFFFF << t) + 1);
        }
    }

    __dcbz((void*)block, 96);
    dc = (s16)(info->components[1].predDC + diff);
    block[0] = info->components[1].predDC = dc;

    for (k = 1; k < 64; k++) {
        THP_HUFF_DECODE_TAB(c2, info, Uachuff, ssss);
        rrrr = ssss >> 4;
        ssss &= 15;

        if (ssss) {
            k += rrrr;

            ASM_INSN(lwz cnt, info->cnt);
            ASM_INSN(subfic cnt33, cnt, 33);
            ASM_INSN(lwz cb, info->currByte);
            ASM_INSN(subf. tmp, cnt33, ssss);
            ASM_INSN(subi cnt1, cnt, 1);
            ASM_INSN(bgt l_notenoughbits);
            ASM_INSN(add v, cnt, ssss);
            ASM_INSN(slw cnt, cb, cnt1);
            ASM_INSN(stw v, info->cnt);
            ASM_INSN(subfic v, ssss, 32);
            ASM_INSN(srw rrrr, cnt, v);

            ASM_INSN(b l_done);
            ASM_INSN(l_notenoughbits: lwz tmp1, info->c);
            ASM_INSN(slw v, cb, cnt1);
            ASM_INSN(lwzu cb, 4(tmp1));
            ASM_INSN(addi tmp, tmp, 1);
            ASM_INSN(stw cb, info->currByte);
            ASM_INSN(srw cb, cb, cnt33);
            ASM_INSN(stw tmp1, info->c);
            ASM_INSN(add v, cb, v);
            ASM_INSN(stw tmp, info->cnt);
            ASM_INSN(subfic tmp, ssss, 32);
            ASM_INSN(srw rrrr, v, tmp);
            l_done:;

            if (__cntlzw((u32)rrrr) > 32 - ssss) {
                rrrr += ((0xFFFFFFFF << ssss) + 1);
            }

            block[__THPJpegNaturalOrder[k]] = (s16)rrrr;
        }

        else {
            if (rrrr != 15)
                break;
            k += 15;
        }
    }
}

static void __THPHuffDecodeDCTCompV(register THPFileInfo* info, THPCoeff* block)
{
    THP_HUFF_DECODE_TAB_VARS(c1)
    THP_HUFF_DECODE_TAB_VARS(c2)
    register s32 t;
    register THPCoeff diff;
    THPCoeff dc;
    register s32 v;
    register u32 cb;
    register u32 cnt;
    register u32 cnt33;
    register u32 tmp;
    register u32 cnt1;
    register u32 tmp1;
    register s32 k;
    register s32 ssss;
    register s32 rrrr;

    __dcbz((void*)block, 0);
    THP_HUFF_DECODE_TAB(c1, info, Vdchuff, t);
    __dcbz((void*)block, 32);
    diff = 0;
    __dcbz((void*)block, 64);

    if (t) {
        ASM_INSN(lwz cnt, info->cnt);
        ASM_INSN(subfic cnt33, cnt, 33);
        ASM_INSN(lwz cb, info->currByte);
        ASM_INSN(subf. tmp, cnt33, t);
        ASM_INSN(subi cnt1, cnt, 1);
        ASM_INSN(bgt l_notenoughbitsdiff);
        ASM_INSN(add v, cnt, t);
        ASM_INSN(slw cnt, cb, cnt1);
        ASM_INSN(stw v, info->cnt);
        ASM_INSN(subfic v, t, 32);
        ASM_INSN(srw diff, cnt, v);

        ASM_INSN(b l_donediff);
        ASM_INSN(l_notenoughbitsdiff: lwz tmp1, info->c);
        ASM_INSN(slw v, cb, cnt1);
        ASM_INSN(lwzu cb, 4(tmp1));
        ASM_INSN(addi tmp, tmp, 1);
        ASM_INSN(stw cb, info->currByte);
        ASM_INSN(srw cb, cb, cnt33);
        ASM_INSN(stw tmp1, info->c);
        ASM_INSN(add v, cb, v);
        ASM_INSN(stw tmp, info->cnt);
        ASM_INSN(subfic tmp, t, 32);
        ASM_INSN(srw diff, v, tmp);
        l_donediff:;

        if (__cntlzw((u32)diff) > 32 - t) {
            diff += ((0xFFFFFFFF << t) + 1);
        }
    }

    __dcbz((void*)block, 96);

    dc = (s16)(info->components[2].predDC + diff);
    block[0] = info->components[2].predDC = dc;

    for (k = 1; k < 64; k++) {
        THP_HUFF_DECODE_TAB(c2, info, Vachuff, ssss);
        rrrr = ssss >> 4;
        ssss &= 15;

        if (ssss) {
            k += rrrr;

            ASM_INSN(lwz cnt, info->cnt);
            ASM_INSN(subfic cnt33, cnt, 33);
            ASM_INSN(lwz cb, info->currByte);
            ASM_INSN(subf. tmp, cnt33, ssss);
            ASM_INSN(subi cnt1, cnt, 1);
            ASM_INSN(bgt l_notenoughbits);
            ASM_INSN(add v, cnt, ssss);
            ASM_INSN(slw cnt, cb, cnt1);
            ASM_INSN(stw v, info->cnt);
            ASM_INSN(subfic v, ssss, 32);
            ASM_INSN(srw rrrr, cnt, v);

            ASM_INSN(b l_done);
            ASM_INSN(l_notenoughbits: lwz tmp1, info->c);
            ASM_INSN(slw v, cb, cnt1);
            ASM_INSN(lwzu cb, 4(tmp1));
            ASM_INSN(addi tmp, tmp, 1);
            ASM_INSN(stw cb, info->currByte);
            ASM_INSN(srw cb, cb, cnt33);
            ASM_INSN(stw tmp1, info->c);
            ASM_INSN(add v, cb, v);
            ASM_INSN(stw tmp, info->cnt);
            ASM_INSN(subfic tmp, ssss, 32);
            ASM_INSN(srw rrrr, v, tmp);
            l_done:;

            if (__cntlzw((u32)rrrr) > 32 - ssss) {
                rrrr += ((0xFFFFFFFF << ssss) + 1);
            }

            block[__THPJpegNaturalOrder[k]] = (s16)rrrr;
        } else {
            if (rrrr != 15)
                break;
            k += 15;
        }
    }
}

BOOL THPInit(void)
{
    u8 *base;

    if (!(PPCMfhid2() & 0x10000000)) {
        return 0;
    }

    base = (u8 *)(0xE000 << 16);
    __THPLCWork512[0] = base;
    base += 0x2000;
    __THPLCWork512[1] = base;
    base += 0x800;
    __THPLCWork512[2] = base;
    base += 0x200;

    base = (u8 *)(0xE000 << 16);
    __THPLCWork640[0] = base;
    base += 0x2800;
    __THPLCWork640[1] = base;
    base += 0xA00;
    __THPLCWork640[2] = base;
    base += 0xA80;

    ASM_INSN(li r3, 4);
    ASM_INSN(oris r3, r3, 4);
    ASM_INSN(mtspr 914, r3);
    ASM_INSN(li r3, 5);
    ASM_INSN(oris r3, r3, 5);
    ASM_INSN(mtspr 915, r3);
    ASM_INSN(li r3, 6);
    ASM_INSN(oris r3, r3, 6);
    ASM_INSN(mtspr 916, r3);
    ASM_INSN(li r3, 7);
    ASM_INSN(oris r3, r3, 7);
    ASM_INSN(mtspr 917, r3);

    __THPInitFlag = 1;
    return 1;
}
