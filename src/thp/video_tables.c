/* Source and layouts:
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/src/thp/THPDec.c
 * https://github.com/doldecomp/sms/blob/main/libs/dolphin/include/dolphin/thp.h */
#include "types.h"
#include "os_cache.h"

typedef struct THPHuffmanTable {
    u8 quick[32], increment[32];
    u8 *symbols;
    s32 max_code[18], value_pointer[18];
    u8 first_symbol;
    u8 reserved[11];
} THPHuffmanTable;

typedef struct THPComponent {
    u8 quantization, dc_table, ac_table;
    s16 previous_dc;
} THPComponent;

typedef struct THPVideoState {
    f32 quantization[3][64];
    THPHuffmanTable huffman[4];
    THPComponent components[3];
    u16 width, height, mcus_per_row, decompressed_y;
    u8 *input;
    u32 current_bits, bit_count;
    u8 valid_tables, restart;
    u16 restart_interval, restart_remaining;
    u8 *output[3];
} THPVideoState;

extern THPVideoState *__THPInfo;
extern u8 *__THPWorkArea;
extern s16 *__THPMCUBuffer[6];
extern u8 *__THPHuffmanBits, *__THPHuffmanSizeTab;
extern u16 *__THPHuffmanCodeTab;

static void __THPRestartDefinition(void)
{
    __THPInfo->restart = 1;
    __THPInfo->input += 2;
    __THPInfo->restart_interval = (__THPInfo->input[0] << 8) | __THPInfo->input[1];
    __THPInfo->input += 2;
    __THPInfo->restart_remaining = __THPInfo->restart_interval;
}

static void __THPSetupBuffers(void)
{
    u8 i;
    s16 *buffer = (s16 *)(((u32)__THPWorkArea + 31) & ~31u);
    for (i = 0; i < 6; i++)
        __THPMCUBuffer[i] = &buffer[i * 64];
}

static void __THPHuffGenerateSizeTable(void)
{
    s32 p, length, count;
    p = 0;
    for (length = 1; length <= 16; length++) {
        count = __THPHuffmanBits[length - 1];
        while (count--)
            __THPHuffmanSizeTab[p++] = (u8)length;
    }
    __THPHuffmanSizeTab[p] = 0;
}

static void __THPHuffGenerateCodeTable(void)
{
    u8 length;
    u16 p, code;
    p = 0;
    code = 0;
    length = __THPHuffmanSizeTab[0];
    while (__THPHuffmanSizeTab[p]) {
        while (__THPHuffmanSizeTab[p] == length) {
            __THPHuffmanCodeTab[p++] = code;
            code++;
        }
        code <<= 1;
        length++;
    }
}

static void __THPHuffGenerateDecoderTables(u8 table_index)
{
    s32 p, length;
    THPHuffmanTable *table;
    p = 0;
    table = &__THPInfo->huffman[table_index];
    for (length = 1; length <= 16; length++) {
        if (__THPHuffmanBits[length - 1]) {
            table->value_pointer[length] = p - __THPHuffmanCodeTab[p];
            p += __THPHuffmanBits[length - 1];
            table->max_code[length] = __THPHuffmanCodeTab[p - 1];
        } else {
            table->max_code[length] = -1;
            table->value_pointer[length] = -1;
        }
    }
    table->max_code[17] = 0xfffff;
}

#pragma dont_inline on
u8 __THPReadFrameHeader(void)
{
    u8 i, value;

    __THPInfo->input += 2;

    value = *(__THPInfo->input)++;

    if (value != 8) {
        return 10;
    }

    __THPInfo->height = (u16)((__THPInfo->input)[0] << 8 | (__THPInfo->input)[1]);
    __THPInfo->input += 2;
    __THPInfo->width = (u16)((__THPInfo->input)[0] << 8 | (__THPInfo->input)[1]);
    __THPInfo->input += 2;

    value = *(__THPInfo->input)++;
    if (value != 3) {
        return 12;
    }

    for (i = 0; i < 3; i++) {
        __THPInfo->input++;
        value = *(__THPInfo->input)++;
        if ((i == 0 && value != 0x22) || (i > 0 && value != 0x11)) {
            return 19;
        }

        __THPInfo->components[i].quantization
 = (*(__THPInfo->input)++);
    }

    return 0;
}

u8 __THPReadScaneHeader(void)
{
    u8 i, value;
    __THPInfo->input += 2;

    value = *(__THPInfo->input)++;

    if (value != 3) {
        return 12;
    }

    for (i = 0; i < 3; i++) {
        __THPInfo->input++;
        value = *(__THPInfo->input)++;
        __THPInfo->components[i].dc_table = (u8)(value >> 4);
        __THPInfo->components[i].ac_table = (u8)(value & 15);

        if ((__THPInfo->valid_tables & (1 << ((value >> 4)))) == 0) {
            return 15;
        }

        if ((__THPInfo->valid_tables & (1 << ((value & 15) + 1))) == 0) {
            return 15;
        }
    }

    __THPInfo->input += 3;
    __THPInfo->mcus_per_row = (u16)((__THPInfo->width + 15) / 16);
    __THPInfo->components[0].previous_dc = 0;
    __THPInfo->components[1].previous_dc = 0;
    __THPInfo->components[2].previous_dc = 0;
    return 0;
}

u8 __THPReadHuffmanTableSpecification(void)
{
    u8 table_class, table_id, i, table_index;
    u16 length, symbol_count;

    __THPHuffmanSizeTab = __THPWorkArea;
    __THPHuffmanCodeTab = (u16 *)((u32)__THPWorkArea + 256 + 1);
    length = (u16)((__THPInfo->input)[0] << 8 | (__THPInfo->input)[1]);
    __THPInfo->input += 2;
    length -= 2;

    for (;;) {
        i = (*(__THPInfo->input)++);
        table_id = (u8)(i & 15);
        table_class = (u8)(i >> 4);
        __THPHuffmanBits = __THPInfo->input;
        table_index = (u8)((table_id << 1) + table_class);
        symbol_count = 0;

        for (i = 0; i < 16; i++) {
            symbol_count += (*(__THPInfo->input)++);
        }

        __THPInfo->huffman[table_index].symbols = __THPInfo->input;
        __THPInfo->input += symbol_count;
        __THPHuffGenerateSizeTable();
        __THPHuffGenerateCodeTable();
        __THPHuffGenerateDecoderTables(table_index);
        __THPInfo->valid_tables |= 1 << table_index;
        length -= 17 + symbol_count;

        if (length == 0) {
            break;
        }
    }

    return 0;
}

static const u8 __THPJpegNaturalOrder[80] = {
    0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
    63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63
};

static const f64 __THPAANScaleFactor[8] = {
    1.0, 1.3870398998260498, 1.3065630197525024, 1.1758755445480347,
    1.0, 0.78569495677948, 0.5411961078643799, 0.27589938044548035
};

u8 __THPReadQuantizationTable(void)
{
    u16 length, table_id, i, row, col;
    f32 temporary[64];

    length = (u16)((__THPInfo->input)[0] << 8 | (__THPInfo->input)[1]);
    __THPInfo->input += 2;
    length -= 2;

    for (;;) {
        table_id = (*(__THPInfo->input)++);

        for (i = 0; i < 64; i++) {
            temporary[__THPJpegNaturalOrder[i]] = (f32)(*(__THPInfo->input)++);
        }

        i = 0;
        for (row = 0; row < 8; row++) {
            for (col = 0; col < 8; col++) {
                __THPInfo->quantization[table_id][i]
 = (f32)((f64)temporary[i] * __THPAANScaleFactor[row]
                            * __THPAANScaleFactor[col]);
                i++;
            }
        }

        length -= 65;
        if (!length) {
            break;
        }
    }

    return 0;
}

extern THPHuffmanTable *Ydchuff, *Udchuff, *Vdchuff;
extern THPHuffmanTable *Yachuff, *Uachuff, *Vachuff;

enum { THP_LOOKUP_DONE = 99 };

void __THPPrepBitStream(void)
{
    u32 *input;
    u32 offset, table_index, prefix, length;

    input = (u32 *)((u32)__THPInfo->input & 0xFFFFFFFC);
    offset = (u32)__THPInfo->input & 3;
    if (__THPInfo->bit_count != 33)
        __THPInfo->bit_count -= (3 - offset) * 8;
    else
        __THPInfo->bit_count = (offset * 8) + 1;

    __THPInfo->input = (u8 *)input;
    __THPInfo->current_bits = *input;
    for (table_index = 0; table_index < 4; table_index++) {
        if (__THPInfo->valid_tables & (1 << table_index)) {
            for (prefix = 0; prefix < 32; prefix++) {
                __THPInfo->huffman[table_index].quick[prefix] = 0xFF;
                for (length = 0; length < 5; length++) {
                    s32 code = (s32)(prefix >> (5 - length - 1));
                    if (code <= __THPInfo->huffman[table_index].max_code[length + 1]) {
                        __THPInfo->huffman[table_index].quick[prefix] =
                            __THPInfo->huffman[table_index].symbols[(s32)(code +
                                __THPInfo->huffman[table_index].value_pointer[length + 1])];
                        __THPInfo->huffman[table_index].increment[prefix] = (u8)(length + 1);
                        length = THP_LOOKUP_DONE;
                    }
                }
            }
        }
    }

    {
        s32 y_dc, u_dc, v_dc, y_ac, u_ac, v_ac;
        y_dc = __THPInfo->components[0].dc_table << 1;
        u_dc = __THPInfo->components[1].dc_table << 1;
        v_dc = __THPInfo->components[2].dc_table << 1;
        y_ac = (__THPInfo->components[0].ac_table << 1) + 1;
        u_ac = (__THPInfo->components[1].ac_table << 1) + 1;
        v_ac = (__THPInfo->components[2].ac_table << 1) + 1;
        Ydchuff = &__THPInfo->huffman[y_dc];
        Udchuff = &__THPInfo->huffman[u_dc];
        Vdchuff = &__THPInfo->huffman[v_dc];
        Yachuff = &__THPInfo->huffman[y_ac];
        Uachuff = &__THPInfo->huffman[u_ac];
        Vachuff = &__THPInfo->huffman[v_ac];
    }
}

extern u32 PPCMfhid2(void);
extern s32 __THPInitFlag;
extern void __THPDecompressYUV(void *, void *, void *);

s32 THPVideoDecode(void * file, void * tileY, void * tileU, void * tileV,
                   void * work)
{
    u8 all_done, status;
    s32 error_code;

    if (!file) {
        goto _err_no_input;
    }

    if (tileY == 0 || tileU == 0 || tileV == 0) {
        goto _err_no_output;
    }

    if (!work) {
        goto _err_no_work;
    }

    if (!(PPCMfhid2() & 0x10000000)) {
        goto _err_lc_not_enabled;
    }

    if (__THPInitFlag == 0) {
        goto _err_not_initialized;
    }

    __THPWorkArea = (u8 *)work;
    __THPInfo = (THPVideoState *)(((u32)__THPWorkArea + 31) & ~31u);
    __THPWorkArea = (u8 *)(((u32)__THPWorkArea + 31) & ~31u) + sizeof(THPVideoState);
    DCZeroRange(__THPInfo, sizeof(THPVideoState));
    __THPInfo->bit_count = 33;
    __THPInfo->decompressed_y = 0;
    __THPInfo->input = (u8 *)file;
    all_done = 0;

    for (;;) {
        if ((*(__THPInfo->input)++) != 255) {
            goto _err_bad_syntax;
        }

        while (*__THPInfo->input == 255) {
            ((__THPInfo->input)++);
        }

        status = (*(__THPInfo->input)++);

        if (status <= 0xD7) {
            if (status == 196) {
                status = __THPReadHuffmanTableSpecification();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else if (status == 192) {
                status = __THPReadFrameHeader();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else {
                goto _err_unsupported_marker;
            }
        }

        else if (0xD8 <= status && status <= 0xDF) {
            if (status == 221) {
                __THPRestartDefinition();
            }

            else if (status == 219) {
                status = __THPReadQuantizationTable();
                if (status != 0) {
                    goto _err_bad_status;
                }
            }

            else if (status == 218) {
                status = __THPReadScaneHeader();
                if (status != 0) {
                    goto _err_bad_status;
                }

                all_done = 1;
            } else if (status == 216) {
            } else {
                goto _err_unsupported_marker;
            }
        }

        else if (0xE0 <= status) {
            if ((224 <= status && status <= 239) || status == 254) {
                __THPInfo->input += (__THPInfo->input)[0] << 8 | (__THPInfo->input)[1];
            } else {
                goto _err_unsupported_marker;
            }
        }

        if (all_done) {
            break;
        }
    }

    __THPSetupBuffers();
    __THPDecompressYUV(tileY, tileU, tileV);
    return 0;

_err_no_input:
    error_code = 25;
    goto _err_exit;

_err_no_output:
    error_code = 27;
    goto _err_exit;

_err_no_work:
    error_code = 26;
    goto _err_exit;

_err_unsupported_marker:
    error_code = 11;
    goto _err_exit;



_err_bad_syntax:
    error_code = 3;
    goto _err_exit;

_err_bad_status:
    error_code = status;
    goto _err_exit;

_err_lc_not_enabled:
    error_code = 28;
    goto _err_exit;

_err_not_initialized:
    error_code = 29;
    goto _err_exit;

_err_exit:
    return error_code;
}
