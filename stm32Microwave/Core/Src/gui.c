#include <string.h>
#include <stdint.h>
#include "lcd.h"
#include "font.h"
#include "gui.h"

/* ====== global drawing state ====== */
uint16_t POINT_COLOR = WHITE;
uint16_t BACK_COLOR  = BLACK;

/* ===== Primitives ===== */

void GUI_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_DrawPixel(x, y, color);
}

/* Classic Bresenham */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int16_t dx = (int16_t)x2 - (int16_t)x1;
    int16_t dy = (int16_t)y2 - (int16_t)y1;
    int16_t sx = (dx >= 0) ? 1 : -1;
    int16_t sy = (dy >= 0) ? 1 : -1;
    dx = (dx >= 0) ? dx : -dx;
    dy = (dy >= 0) ? dy : -dy;

    int16_t err = (dx > dy ? dx : -dy) / 2;
    int16_t x = (int16_t)x1, y = (int16_t)y1;

    for(;;) {
        if ((uint16_t)x < LCD_Width() && (uint16_t)y < LCD_Height())
            LCD_DrawPixel((uint16_t)x, (uint16_t)y, POINT_COLOR);

        if (x == (int16_t)x2 && y == (int16_t)y2) break;
        int16_t e2 = err;
        if (e2 > -dx) { err -= dy; x += sx; }
        if (e2 <  dy) { err += dx; y += sy; }
    }
}

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (x2 < x1) { uint16_t t=x1; x1=x2; x2=t; }
    if (y2 < y1) { uint16_t t=y1; y1=y2; y2=t; }
    LCD_DrawFastHLine(x1, y1, (x2>=x1)?(x2-x1+1):0, POINT_COLOR);
    LCD_DrawFastHLine(x1, y2, (x2>=x1)?(x2-x1+1):0, POINT_COLOR);
    LCD_DrawFastVLine(x1, y1, (y2>=y1)?(y2-y1+1):0, POINT_COLOR);
    LCD_DrawFastVLine(x2, y1, (y2>=y1)?(y2-y1+1):0, POINT_COLOR);
}

void LCD_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (x2 < x1) { uint16_t t=x1; x1=x2; x2=t; }
    if (y2 < y1) { uint16_t t=y1; y1=y2; y2=t; }
    if (x1 >= LCD_Width() || y1 >= LCD_Height()) return;
    if (x2 >= LCD_Width())  x2 = LCD_Width() - 1;
    if (y2 >= LCD_Height()) y2 = LCD_Height() - 1;

    LCD_FillRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1, POINT_COLOR);
}

/* ===== Circles / Triangles ===== */

static void _draw_circle_8(int xc, int yc, int x, int y, uint16_t c)
{
    if ((uint16_t)(xc + x) < LCD_Width() && (uint16_t)(yc + y) < LCD_Height()) GUI_DrawPoint((uint16_t)(xc + x), (uint16_t)(yc + y), c);
    if ((uint16_t)(xc - x) < LCD_Width() && (uint16_t)(yc + y) < LCD_Height()) GUI_DrawPoint((uint16_t)(xc - x), (uint16_t)(yc + y), c);
    if ((uint16_t)(xc + x) < LCD_Width() && (uint16_t)(yc - y) < LCD_Height()) GUI_DrawPoint((uint16_t)(xc + x), (uint16_t)(yc - y), c);
    if ((uint16_t)(xc - x) < LCD_Width() && (uint16_t)(yc - y) < LCD_Height()) GUI_DrawPoint((uint16_t)(xc - x), (uint16_t)(yc - y), c);
    if ((uint16_t)(xc + y) < LCD_Width() && (uint16_t)(yc + x) < LCD_Height()) GUI_DrawPoint((uint16_t)(xc + y), (uint16_t)(yc + x), c);
    if ((uint16_t)(xc - y) < LCD_Width() && (uint16_t)(yc + x) < LCD_Height()) GUI_DrawPoint((uint16_t)(xc - y), (uint16_t)(yc + x), c);
    if ((uint16_t)(xc + y) < LCD_Width() && (uint16_t)(yc - x) < LCD_Height()) GUI_DrawPoint((uint16_t)(xc + y), (uint16_t)(yc - x), c);
    if ((uint16_t)(xc - y) < LCD_Width() && (uint16_t)(yc - x) < LCD_Height()) GUI_DrawPoint((uint16_t)(xc - y), (uint16_t)(yc - x), c);
}

static void gui_circle(int xc, int yc, uint16_t c, int r, int fill)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;

    if (fill) {
        while (x <= y) {
            for (int yi = x; yi <= y; yi++)
                _draw_circle_8(xc, yc, x, yi, c);
            if (d < 0) d += 4 * x + 6;
            else { d += 4 * (x - y) + 10; y--; }
            x++;
        }
    } else {
        while (x <= y) {
            _draw_circle_8(xc, yc, x, y, c);
            if (d < 0) d += 4 * x + 6;
            else { d += 4 * (x - y) + 10; y--; }
            x++;
        }
    }
}

void Draw_Circle(uint16_t x0, uint16_t y0, uint16_t fc, uint8_t r)
{
    uint16_t old = POINT_COLOR;
    POINT_COLOR = fc;
    gui_circle(x0, y0, fc, r, 0);
    POINT_COLOR = old;
}

void Draw_Triangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    LCD_DrawLine(x0,y0,x1,y1);
    LCD_DrawLine(x1,y1,x2,y2);
    LCD_DrawLine(x2,y2,x0,y0);
}

static void _swap_u16(uint16_t *a, uint16_t *b)
{
    uint16_t t = *a; *a = *b; *b = t;
}

void Fill_Triangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    int dx01 = (int)x1 - (int)x0, dy01 = (int)y1 - (int)y0;
    int dx02 = (int)x2 - (int)x0, dy02 = (int)y2 - (int)y0;
    int dx12 = (int)x2 - (int)x1, dy12 = (int)y2 - (int)y1;
    uint16_t a, b, y, last;
    long sa = 0, sb = 0;

    if (y0 > y1) { _swap_u16(&y0,&y1); _swap_u16(&x0,&x1); }
    if (y1 > y2) { _swap_u16(&y2,&y1); _swap_u16(&x2,&x1); }
    if (y0 > y1) { _swap_u16(&y0,&y1); _swap_u16(&x0,&x1); }

    if (y0 == y2) {
        a = b = x0;
        if (x1 < a) a = x1; else if (x1 > b) b = x1;
        if (x2 < a) a = x2; else if (x2 > b) b = x2;
        if (y0 < LCD_Height())
            LCD_FillRect(a, y0, b >= a ? (b - a + 1) : 0, 1, POINT_COLOR);
        return;
    }

    last = (y1 == y2) ? y1 : (uint16_t)(y1 - 1);

    for (y = y0; y <= last; y++) {
        a = (uint16_t)((int)x0 + (dy01 ? sa / dy01 : 0));
        b = (uint16_t)((int)x0 + (dy02 ? sb / dy02 : 0));
        sa += dx01; sb += dx02;
        if (a > b) _swap_u16(&a,&b);
        if (y < LCD_Height()) LCD_FillRect(a, y, b >= a ? (b - a + 1) : 0, 1, POINT_COLOR);
    }

    sa = (long)dx12 * (long)((int)y - (int)y1);
    sb = (long)dx02 * (long)((int)y - (int)y0);
    for (; y <= y2; y++) {
        a = (uint16_t)((int)x1 + (dy12 ? sa / dy12 : 0));
        b = (uint16_t)((int)x0 + (dy02 ? sb / dy02 : 0));
        sa += dx12; sb += dx02;
        if (a > b) _swap_u16(&a,&b);
        if (y < LCD_Height()) LCD_FillRect(a, y, b >= a ? (b - a + 1) : 0, 1, POINT_COLOR);
    }
}

/* ===== Helpers ===== */

static uint32_t mypow10(uint8_t n)
{
    uint32_t r = 1;
    while (n--) r *= 10U;
    return r;
}

/* ===== ASCII text =====
 * size: 12 -> use FONT_GetASCIIFont6x12 (6x12)
 *       16 -> use FONT_GetASCIIFont8x16 (8x16)
 * mode: 0 opaque (draw BACK_COLOR on 0 bits), 1 transparent
 */
void LCD_ShowChar(uint16_t x, uint16_t y,
                  uint16_t fc, uint16_t bc,
                  uint8_t ch, uint8_t size, uint8_t mode)
{
    const uint8_t *rowptr = NULL;
    uint8_t w = (size == 12) ? 6 : 8;
    uint8_t h = (size == 12) ? 12 : 16;

    /* only printable ASCII */
    if (ch < 32 || ch > 126) return;

    /* fetch glyph rows */
    rowptr = (size == 12) ? FONT_GetASCIIFont6x12((char)ch)
                          : FONT_GetASCIIFont8x16((char)ch);
    if (!rowptr) return;

    /* quick reject if completely outside */
    if (x >= LCD_Width() || y >= LCD_Height()) return;

    /* per-pixel clipped draw (safe even near edges) */
    for (uint8_t row = 0; row < h; row++) {
        if ((uint16_t)(y + row) >= LCD_Height()) break;

        uint8_t bits = rowptr[row];          /* MSB-left */
        for (uint8_t col = 0; col < w; col++) {
            if ((uint16_t)(x + col) >= LCD_Width()) break;

            uint8_t mask = (uint8_t)(0x80u >> col);
            if (bits & mask) {
                LCD_DrawPixel((uint16_t)(x + col), (uint16_t)(y + row), fc);
            } else if (!mode) {
                LCD_DrawPixel((uint16_t)(x + col), (uint16_t)(y + row), bc);
            }
        }
    }
}

void LCD_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode)
{
    uint8_t w = (size == 12) ? 6 : 8;
    while ((*p >= ' ') && (*p <= '~')) {
        LCD_ShowChar(x, y, POINT_COLOR, BACK_COLOR, *p, size, mode);
        x = (uint16_t)(x + w);
        if (x >= LCD_Width()) break;
        p++;
    }
}

void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{
    uint8_t w = (size == 12) ? 6 : 8;
    uint8_t t, temp;
    uint8_t shown = 0;
    for (t = 0; t < len; t++) {
        temp = (uint8_t)((num / mypow10((uint8_t)(len - t - 1))) % 10U);
        if (!shown && t < (len - 1)) {
            if (temp == 0) {
                LCD_ShowChar((uint16_t)(x + w*t), y, POINT_COLOR, BACK_COLOR, ' ', size, 0);
                continue;
            } else {
                shown = 1;
            }
        }
        LCD_ShowChar((uint16_t)(x + w*t), y, POINT_COLOR, BACK_COLOR, (uint8_t)(temp + '0'), size, 0);
    }
}

void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint8_t size,uint8_t mode)
{
    uint8_t w = (size == 12) ? 6 : 8;
    for (int i = (int)len - 1; i >= 0; --i) {
        uint8_t digit = (uint8_t)((num / mypow10((uint8_t)i)) % 10U);
        LCD_ShowChar((uint16_t)(x + (len - 1 - i) * w), y, POINT_COLOR, BACK_COLOR,
                     (uint8_t)('0' + digit), size, mode);
    }
}

/* ===== Chinese fonts (optional) ===== */

#ifdef HAVE_TFONT16
extern const typFNT_GB16 tfont16[];
extern const uint32_t    tfont16_count;
#endif
#ifdef HAVE_TFONT24
extern const typFNT_GB24 tfont24[];
extern const uint32_t    tfont24_count;
#endif
#ifdef HAVE_TFONT32
extern const typFNT_GB32 tfont32[];
extern const uint32_t    tfont32_count;
#endif

static void _draw_cn_block(uint16_t x, uint16_t y,
                           uint16_t fc, uint16_t bc,
                           const char *msk, uint16_t w, uint16_t h,
                           uint8_t mode)
{
    /* msk is row-major, 8 pixels per byte, high->low bits within byte */
    uint32_t bi = 0;
    for (uint16_t row = 0; row < h; row++) {
        for (uint16_t colb = 0; colb < (uint16_t)(w/8); colb++) {
            uint8_t b = (uint8_t)msk[bi++];
            for (uint8_t k = 0; k < 8; k++) {
                uint16_t xx = (uint16_t)(x + colb*8 + k);
                uint16_t yy = (uint16_t)(y + row);
                if (xx >= LCD_Width() || yy >= LCD_Height()) continue;
                if (b & (uint8_t)(0x80u >> k)) {
                    LCD_DrawPixel(xx, yy, fc);
                } else if (!mode) {
                    LCD_DrawPixel(xx, yy, bc);
                }
            }
        }
    }
}

void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
#ifdef HAVE_TFONT16
    for (uint32_t i = 0; i < tfont16_count; i++) {
        if (tfont16[i].Index[0]==s[0] && tfont16[i].Index[1]==s[1]) {
            _draw_cn_block(x,y,fc,bc,tfont16[i].Msk,16,16,mode);
            return;
        }
    }
#else
    (void)x;(void)y;(void)fc;(void)bc;(void)s;(void)mode;
#endif
}

void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
#ifdef HAVE_TFONT24
    for (uint32_t i = 0; i < tfont24_count; i++) {
        if (tfont24[i].Index[0]==s[0] && tfont24[i].Index[1]==s[1]) {
            _draw_cn_block(x,y,fc,bc,tfont24[i].Msk,24,24,mode);
            return;
        }
    }
#else
    (void)x;(void)y;(void)fc;(void)bc;(void)s;(void)mode;
#endif
}

void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode)
{
#ifdef HAVE_TFONT32
    for (uint32_t i = 0; i < tfont32_count; i++) {
        if (tfont32[i].Index[0]==s[0] && tfont32[i].Index[1]==s[1]) {
            _draw_cn_block(x,y,fc,bc,tfont32[i].Msk,32,32,mode);
            return;
        }
    }
#else
    (void)x;(void)y;(void)fc;(void)bc;(void)s;(void)mode;
#endif
}

/* ===== Mixed strings (ASCII + Chinese) ===== */

void Show_Str(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
{
    uint16_t x0 = x;
    uint8_t ascii_w = (size == 12) ? 6 : 8;

    while (*str != 0) {
        if ((uint8_t)*str > 0x80) { /* Chinese: 2 bytes */
            if (x > (uint16_t)(LCD_Width() - size) || y > (uint16_t)(LCD_Height() - size)) return;
#if defined(HAVE_TFONT32) || defined(HAVE_TFONT24) || defined(HAVE_TFONT16)
            if      (size == 32) GUI_DrawFont32(x,y,fc,bc,str,mode);
            else if (size == 24) GUI_DrawFont24(x,y,fc,bc,str,mode);
            else                 GUI_DrawFont16(x,y,fc,bc,str,mode);
#endif
            str += 2;
            x = (uint16_t)(x + size);
        } else {
            if (*str == '\r') { y = (uint16_t)(y + size); x = x0; str++; continue; }
            if (x > (uint16_t)(LCD_Width() - ascii_w) || y > (uint16_t)(LCD_Height() - size)) return;
            LCD_ShowChar(x,y,fc,bc,*str,size,mode);
            x = (uint16_t)(x + ascii_w);
            str++;
        }
    }
}

/* Center horizontally on current rotation; only needs Y */
void Gui_StrCenter(uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode)
{
    uint8_t ascii_w = (size == 12) ? 6 : 8;
    /* rough width: ASCII only; if CN mixed, this centers approximately */
    uint32_t len = (uint32_t)strlen((const char*)str);
    uint32_t px  = len * ascii_w;
    uint16_t x   = (LCD_Width() > px) ? (uint16_t)((LCD_Width() - px) / 2U) : 0;
    Show_Str(x, y, fc, bc, str, size, mode);
}

/* ===== Simple demos ===== */

void run_lcd_probe(void)
{
    LCD_Clear(NAVY);
    POINT_COLOR = YELLOW;
    LCD_DrawRectangle(2,2,(uint16_t)(LCD_Width()-3),(uint16_t)(LCD_Height()-3));

    POINT_COLOR = GREEN;
    LCD_DrawLine(0,0,(uint16_t)(LCD_Width()-1),(uint16_t)(LCD_Height()-1));
    LCD_DrawLine((uint16_t)(LCD_Width()-1),0,0,(uint16_t)(LCD_Height()-1));

    POINT_COLOR = CYAN;
    Draw_Circle(40,40,CYAN,18);

    POINT_COLOR = MAGENTA;
    Fill_Triangel(80,30,110,60,50,60);
}

void run_text_ascii(void)
{
    LCD_Clear(BLACK);
    POINT_COLOR = WHITE; BACK_COLOR = BLACK;

    LCD_ShowString(6,  6, 16, (uint8_t*)"Hello, STM32!", 0);
    LCD_ShowString(6, 28, 12, (uint8_t*)"ASCII 6x12 font", 0);

    LCD_ShowString(6, 50, 16, (uint8_t*)"Count:", 0);
    LCD_ShowNum((uint16_t)(6 + 7*8), 50, 123456, 6, 16);

    LCD_ShowString(6, 72, 16, (uint8_t*)"Zero-pad:", 0);
    LCD_Show2Num((uint16_t)(6 + 9*8), 72, 42, 5, 16, 0);

    Gui_StrCenter(100, CYAN, BLACK, (uint8_t*)"CENTER DEMO", 16, 0);
}

void run_text_cn(void)
{
#if defined(HAVE_TFONT16) || defined(HAVE_TFONT24) || defined(HAVE_TFONT32)
    LCD_Clear(DARKBLUE);
    POINT_COLOR = WHITE; BACK_COLOR = DARKBLUE;

    /* Example strings must exist in your tfont tables */
    #ifdef HAVE_TFONT16
    GUI_DrawFont16(6, 20, YELLOW, DARKBLUE, (uint8_t*)"深", 0);
    GUI_DrawFont16(22,20, YELLOW, DARKBLUE, (uint8_t*)"圳", 0);
    #endif

    #ifdef HAVE_TFONT24
    GUI_DrawFont24(6, 50, CYAN, DARKBLUE, (uint8_t*)"市", 0);
    GUI_DrawFont24(30,50, CYAN, DARKBLUE, (uint8_t*)"中", 0);
    #endif

    #ifdef HAVE_TFONT32
    GUI_DrawFont32(6, 90, WHITE, DARKBLUE, (uint8_t*)"文", 0);
    GUI_DrawFont32(38,90, WHITE, DARKBLUE, (uint8_t*)"测", 0);
    #endif
#else
    /* If no Chinese tables compiled, show info text */
    LCD_Clear(DARKBLUE);
    POINT_COLOR = WHITE; BACK_COLOR = DARKBLUE;
    Show_Str(8, 40, WHITE, DARKBLUE, (uint8_t*)"CN fonts not built.", 16, 0);
    Show_Str(8, 64, WHITE, DARKBLUE, (uint8_t*)"Define HAVE_TFONTxx", 16, 0);
#endif
}
//******************************************************test (can be deleted)
void GUI_Test_First_CN16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t mode)
{
#ifdef HAVE_TFONT16
    if (tfont16_count == 0) return;
    // draw the very first entry from your table
    const typFNT_GB16 *g = &tfont16[0];
    // reuse the internal helper already in gui.c:
    extern void _draw_cn_block(uint16_t, uint16_t, uint16_t, uint16_t,
                               const char*, uint16_t, uint16_t, uint8_t);
    _draw_cn_block(x, y, fc, bc, g->Msk, 16, 16, mode);
#else
    (void)x;(void)y;(void)fc;(void)bc;(void)mode;
#endif
}
