/* Reference: https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/gx/GXAttr.c. */
#include "gx_state.h"
#include "gx_vertex_types.h"

void __GXCalculateVLim(void)
{
    static u8 tbl1[4] = {0, 4, 1, 2};
    static u8 tbl2[4] = {0, 8, 1, 2};
    static u8 tbl3[4] = {0, 12, 1, 2};
    u32 limit;
    u32 vcd_lo;
    u32 vcd_hi;
    GXCompCnt normal_count;

    if (gx->vNum == 0)
        return;
    vcd_lo = gx->vcdLo;
    vcd_hi = gx->vcdHi;
    normal_count = (GXCompCnt)((gx->vatA[0] & 0x200) >> 9);
    limit = GX_GET_FIELD(vcd_lo, 1, 0);
    limit += (u8)GX_GET_FIELD(vcd_lo, 1, 1);
    limit += (u8)GX_GET_FIELD(vcd_lo, 1, 2);
    limit += (u8)GX_GET_FIELD(vcd_lo, 1, 3);
    limit += (u8)GX_GET_FIELD(vcd_lo, 1, 4);
    limit += (u8)GX_GET_FIELD(vcd_lo, 1, 5);
    limit += (u8)GX_GET_FIELD(vcd_lo, 1, 6);
    limit += (u8)GX_GET_FIELD(vcd_lo, 1, 7);
    limit += (u8)GX_GET_FIELD(vcd_lo, 1, 8);
    limit += tbl3[(u8)GX_GET_FIELD(vcd_lo, 2, 9)];
    limit += tbl3[(u8)GX_GET_FIELD(vcd_lo, 2, 11)] * (normal_count == GX_NRM_NBT ? 3 : 1);
    limit += tbl1[(u8)GX_GET_FIELD(vcd_lo, 2, 13)];
    limit += tbl1[(u8)GX_GET_FIELD(vcd_lo, 2, 15)];
    limit += tbl2[(u8)GX_GET_FIELD(vcd_hi, 2, 0)];
    limit += tbl2[(u8)GX_GET_FIELD(vcd_hi, 2, 2)];
    limit += tbl2[(u8)GX_GET_FIELD(vcd_hi, 2, 4)];
    limit += tbl2[(u8)GX_GET_FIELD(vcd_hi, 2, 6)];
    limit += tbl2[(u8)GX_GET_FIELD(vcd_hi, 2, 8)];
    limit += tbl2[(u8)GX_GET_FIELD(vcd_hi, 2, 10)];
    limit += tbl2[(u8)GX_GET_FIELD(vcd_hi, 2, 12)];
    limit += tbl2[(u8)GX_GET_FIELD(vcd_hi, 2, 14)];
    gx->vLim = limit;
}
