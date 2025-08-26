#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize console after LCD_Init().
   width_px, height_px are taken from LCD driver; pass 0 to auto-read via LCD_Width/LCD_Height. */
void Console_Init(uint16_t width_px, uint16_t height_px);

/* Colors / cursor control */
void Console_SetColors(uint16_t fg, uint16_t bg);
void Console_GetColors(uint16_t *fg, uint16_t *bg);
void Console_SetCursor(uint16_t col, uint16_t row);   /* column & row in character cells */
void Console_GetCursor(uint16_t *col, uint16_t *row);

/* Clear whole screen and home cursor */
void Console_Clear(void);

/* Low-level character output (also used by putchar/printf) */
int  Console_PutChar(int ch);

/* Convenience */
void Console_Puts(const char *s);

/* If you want to change font later (e.g., to 6x12), call this first. */
void Console_SetFontCell(uint8_t cell_w, uint8_t cell_h);

#ifdef __cplusplus
}
#endif
#endif /* CONSOLE_H */
