#include "os_font.h"
#include "gx_texture.h"
#include "gx_transform.h"
#include "gx_tev.h"
#include "matrix.h"

extern OSFontHeader *FontData;
extern void *LastSheet;

void LoadSheet(void *image, GXTexMapID id)
{
    Mtx matrix;
    GXTexObj texture;
    /* The original copies this magenta color to the stack without reading it. */
    GXColor color = { 0xFF, 0x00, 0xFF, 0xFF };

    if (LastSheet != image) {
        LastSheet = image;
        GXInitTexObj(&texture, image, FontData->sheetWidth, FontData->sheetHeight, GX_TF_I4, GX_CLAMP, GX_CLAMP, 0);
        GXInitTexObjLOD(&texture, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
        GXLoadTexObj(&texture, id);
        PSMTXScale(matrix, 1.0f / FontData->sheetWidth, 1.0f / FontData->sheetHeight, 1.0f);
        GXLoadTexMtxImm(matrix, GX_TEXMTX0, GX_MTX2x4);
    }
}
