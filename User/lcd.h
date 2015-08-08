#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define LCD_REFRESH_PERIOD         1
#define MAX_LCD_DISP_TMP_BUF_LEN  16
 
void init_lcd(void);
void lcd_disp_init(void);
void lcd_disp_string(u8 x, u8 y, u8 *str);
void lcd_disp_normal(void);
void lcd_high_light(u8 x, u8 y);
void lcd_cursor_left(void);


#ifdef __cplusplus
}
#endif

#endif
