#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define LCD_SCK_HIGH()          GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define LCD_SCK_LOW()           GPIO_ResetBits(GPIOB, GPIO_Pin_13)

#define LCD_MOSI_HIGH()         GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define LCD_MOSI_LOW()          GPIO_ResetBits(GPIOB, GPIO_Pin_15)

#define LCD_CS_HIGH()           GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define LCD_CS_LOW()            GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define LCD_RST_HIGH()          GPIO_SetBits(GPIOA, GPIO_Pin_11)
#define LCD_RST_LOW()           GPIO_ResetBits(GPIOA, GPIO_Pin_11)

#define LCD_RS_HIGH()           GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define LCD_RS_LOW()            GPIO_ResetBits(GPIOA, GPIO_Pin_8)

#define LCD_BACKLIGHT_HIGH()    GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define LCD_BACKLIGHT_LOW()     GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define LCD_BACKLIGHT_ON()      LCD_BACKLIGHT_LOW()
#define LCD_BACKLIGHT_OFF()     LCD_BACKLIGHT_HIGH()

extern const unsigned char lcd_map_welcome[];

void lcd_disp_map(unsigned char *map);
void lcd_init(void);


#ifdef __cplusplus
}
#endif

#endif
