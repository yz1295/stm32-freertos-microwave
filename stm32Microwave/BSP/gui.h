#ifndef __GUI_H
#define __GUI_H

#include <stdint.h>
#include "lcd.h"
#include "font.h"   /* FONT_GetASCIIFont6x12 / FONT_GetASCIIFont8x16 + optional Chinese tables */

#ifdef __cplusplus
extern "C" {
#endif

/* Global drawing colors (owned by gui.c) */
extern uint16_t POINT_COLOR;   /* pen/foreground */
extern uint16_t BACK_COLOR;    /* background for opaque drawing */

/* ===== Primitives ===== */
void GUI_DrawPoint(uint16_t x, uint16_t y, uint16_t color);

/* Lines / Rectangles / Circles / Triangles */
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void Draw_Circle(uint16_t x0, uint16_t y0, uint16_t fc, uint8_t r);
void Draw_Triangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void Fill_Triangel(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

/* ===== ASCII text (6x12 or 8x16) =====
 * size: 12 -> 6x12 (asc2_1206)
 *       16 -> 8x16 (asc2_1608)
 * mode: 0 = opaque (draw BACK_COLOR on 0 bits)
 *       1 = transparent (skip 0 bits)
 */
void LCD_ShowChar(uint16_t x,uint16_t y,
                  uint16_t fc, uint16_t bc,
                  uint8_t ch, uint8_t size, uint8_t mode);
void LCD_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode);
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint8_t size,uint8_t mode);

/* ===== Chinese GB fonts (optional; compile if you have tfont16/24/32 in font.c) ===== */
void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);
void GUI_DrawFont24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);
void GUI_DrawFont32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s,uint8_t mode);

/* Mixed string (ASCII + Chinese). If no Chinese tables compiled, ASCII still works. */
void Show_Str(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode);

/* Centered ASCII string: centered horizontally; provide Y line, colors, size, mode */
void Gui_StrCenter(uint16_t y, uint16_t fc, uint16_t bc, uint8_t *str,uint8_t size,uint8_t mode);

/* Small demos (you can call them from main.c) */
void run_lcd_probe(void);
void run_text_ascii(void);
void run_text_cn(void);   /* will show a note if Chinese tables missing */

#ifdef __cplusplus
}
#endif
#endif /* __GUI_H */
