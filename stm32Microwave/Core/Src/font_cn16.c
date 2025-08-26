#include <stdint.h>
#include "font.h"

/* Enable the 16x16 CN path in gui.c */
#define HAVE_TFONT16 1

/* One test glyph: GB2312 code for "中" is 0xD6 0xD0.
   Mask format: row-major, 16 px/row => 2 bytes per row, MSB is leftmost pixel.
   (This bitmap is simple-but-readable; adjust to taste.) */
const typFNT_GB16 tfont16[] = {
    {
        {0xD6, 0xD0},
        {
            /* 16 rows, 2 bytes each */
            0x00,0x00,
            0x0F,0xF0,
            0x08,0x10,
            0x08,0x10,
            0x7F,0xFE,
            0x08,0x10,
            0x08,0x10,
            0x7F,0xFE,
            0x08,0x10,
            0x08,0x10,
            0x08,0x10,
            0x08,0x10,
            0x0F,0xF0,
            0x00,0x00,
            0x00,0x00,
            0x00,0x00,
        }
    },
};

const uint32_t tfont16_count = sizeof(tfont16)/sizeof(tfont16[0]);
