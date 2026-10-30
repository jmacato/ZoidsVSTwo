#ifndef ZOIDS_GX_TEXTURE_H
#define ZOIDS_GX_TEXTURE_H
#include "gx_texture_types.h"
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, u8 mipmap, u8 max_lod);
void __GetImageTileCount(GXTexFmt fmt, u16 wd, u16 ht, u32 *rowTiles, u32 *colTiles,
                         u32 *cmpTiles);
void GXInitTexObj(GXTexObj *obj, void *image_ptr, u16 width, u16 height,
                  GXTexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t,
                  u8 mipmap);
void GXInitTexObjCI(GXTexObj *obj, void *image_ptr, u16 width, u16 height,
                    GXCITexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t,
                    u8 mipmap, u32 tlut_name);
void GXInitTexObjLOD(GXTexObj *obj, GXTexFilter min_filt, GXTexFilter mag_filt,
                     f32 min_lod, f32 max_lod, f32 lod_bias, u8 bias_clamp,
                     u8 do_edge_lod, GXAnisotropy max_aniso);
GXTexFmt GXGetTexObjFmt(const GXTexObj *to);
void GXLoadTexObjPreLoaded(GXTexObj *obj, GXTexRegion *region, GXTexMapID id);
void GXLoadTexObj(GXTexObj *obj, GXTexMapID id);
void GXInitTlutObj(GXTlutObj *tlut_obj, void *lut, GXTlutFmt fmt, u16 n_entries);
void GXLoadTlut(GXTlutObj *tlut_obj, u32 tlut_name);
void GXInitTexCacheRegion(GXTexRegion *region, u8 is_32b_mipmap, u32 tmem_even,
                          GXTexCacheSize size_even, u32 tmem_odd,
                          GXTexCacheSize size_odd);
void GXInitTlutRegion(GXTlutRegion *region, u32 tmem_addr, GXTlutSize tlut_size);
void GXInvalidateTexAll(void);
GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback f);
GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback f);
void __GXSetSUTexRegs(void);
void __GXSetTmemConfig(u32 config);
#endif
