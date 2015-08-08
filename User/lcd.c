#include "includes.h"


#define SCLK_PIN     GPIO_Pin_12
#define SID_PIN      GPIO_Pin_13
#define CS_PIN       GPIO_Pin_8

#define SCLK_PORT    GPIOB
#define SID_PORT     GPIOB
#define CS_PORT      GPIOA

#define LCD_SCLK_HIGH()       GPIO_SetBits(SCLK_PORT, SCLK_PIN)
#define LCD_SCLK_LOW()        GPIO_ResetBits(SCLK_PORT, SCLK_PIN)

#define LCD_SID_HIGH()        GPIO_SetBits(SID_PORT, SID_PIN)
#define LCD_SID_LOW()         GPIO_ResetBits(SID_PORT, SID_PIN)

#define LCD_CS_HIGH()         GPIO_SetBits(CS_PORT, CS_PIN)
#define LCD_CS_LOW()          GPIO_ResetBits(CS_PORT, CS_PIN)

void lcd_send_byte(u8 byte)
{
    u8 i;

    
    for(i = 0; i < 8; i++)
    {
    	if((byte << i) & 0x80)
    	{
            LCD_SID_HIGH();
    	}
    	else 
    	{
            LCD_SID_LOW();
    	}
        
        LCD_SCLK_LOW();
        LCD_SCLK_HIGH();
    }

    LCD_SCLK_LOW();
    LCD_SID_LOW();
}

void lcd_write_cmd(u8 cmd)
{
    LCD_CS_HIGH();
    lcd_send_byte(0xf8);
    lcd_send_byte(cmd & 0xf0);
    lcd_send_byte((cmd << 4) & 0xf0);
    mdelay(2);
    LCD_CS_LOW();
}

void lcd_write_data(u8 data)
{
    LCD_CS_HIGH();
    lcd_send_byte(0xfa);
    lcd_send_byte(data & 0xf0);
    lcd_send_byte((data << 4) & 0xf0); 
    mdelay(2);
    LCD_CS_LOW();
}

void lcd_disp_pos(u8 x, u8 y)
{
    u8 pos;
    u8 row, col; //行，列


    col = y;
    col = col % 16; //一行16列
    
    switch(x)
    {
    case 0:
        row = 0x80; //第一行
        break;
        
    case 1:
        row = 0x90; //第二行
        break;

    case 2:
        row = 0x88; //第三行
        break;

    case 3:
        row = 0x98; //第四行
        break;

    default:
        row = 0x80;
        break;
    }

    pos = row + col;

    lcd_write_cmd(pos);

    mdelay(5);
}

void lcd_clr_scr(void)
{
    lcd_write_cmd(0x01);
}

void lcd_addr_home(void)
{
    lcd_write_cmd(0x02);
}

void lcd_high_light(u8 x, u8 y)
{
    lcd_disp_pos(x, y);
    lcd_write_cmd(0x0f);
}

void lcd_disp_normal(void)
{
    lcd_write_cmd(0x0c);
}

void lcd_cursor_left(void)
{
    lcd_write_cmd(0x10);
}

void lcd_disp_string(u8 x, u8 y, u8 *str)
{
    lcd_disp_pos(x, y);
    
    while(*str)
    {
        lcd_write_data(*str);
        str++;
    }
}

void lcd_disp_init(void)
{
    u8 disp_tmp_buf[MAX_LCD_DISP_TMP_BUF_LEN];
    u8 disp_buf_len;


    lcd_disp_string(g_time_default_pos[TIME_DEFAULT_STR_POS][0], g_time_default_pos[TIME_DEFAULT_STR_POS][1], (u8 *)g_time_default_str);
    
    disp_buf_len = 8;
    sprintf((char *)disp_tmp_buf, "%02d时%02d分", g_time_default.hour, g_time_default.minute);
    disp_tmp_buf[disp_buf_len] = '\0';
    lcd_disp_string(g_time_default_pos[TIME_DEFAULT_POS][0], g_time_default_pos[TIME_DEFAULT_POS][1], disp_tmp_buf);
    
    lcd_disp_string(g_time_count_pos[TIME_COUNT_STR_POS][0], g_time_count_pos[TIME_COUNT_STR_POS][1], (u8 *)g_time_count_str);    
    
    lcd_disp_string(g_time_count_pos[TIME_COUNT_POS][0], g_time_count_pos[TIME_COUNT_POS][1], "00:00:00");

    lcd_disp_string(g_flow_pos[FLOW_STR_POS][0], g_flow_pos[FLOW_STR_POS][1], (u8 *)g_flow_str);

    disp_buf_len = 10;
    sprintf((char *)disp_tmp_buf, "%d.%d SLPM  "/* 2 Space */, p_flow_ext_param->int_part, p_flow_ext_param->dec_part);
    disp_tmp_buf[disp_buf_len] = '\0';
    lcd_disp_string(g_flow_pos[FLOW_POS][0], g_flow_pos[FLOW_POS][1], disp_tmp_buf);  

    lcd_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_pause_str);
}

void init_lcd(void)
{
    LCD_CS_LOW();
    LCD_SCLK_LOW();
    LCD_SID_LOW();

    mdelay(5);
    lcd_clr_scr();
    lcd_addr_home();
    lcd_disp_normal();
}

