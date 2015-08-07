#include "includes.h"


#define SCLK_PIN     GPIO_Pin_12
#define SID_PIN      GPIO_Pin_13
#define CS_PIN       GPIO_Pin_8

#define SCLK_PORT    GPIOB
#define SID_PORT     GPIOB
#define CS_PORT      GPIOA

#define LCM12864_SCLK_HIGH()  GPIO_SetBits(SCLK_PORT, SCLK_PIN)
#define LCM12864_SCLK_LOW()   GPIO_ResetBits(SCLK_PORT, SCLK_PIN)

#define LCM12864_SID_HIGH()   GPIO_SetBits(SID_PORT, SID_PIN)
#define LCM12864_SID_LOW()    GPIO_ResetBits(SID_PORT, SID_PIN)

#define LCM12864_CS_HIGH()    GPIO_SetBits(CS_PORT, CS_PIN)
#define LCM12864_CS_LOW()     GPIO_ResetBits(CS_PORT, CS_PIN)

void lcm12864_send_byte(u8 byte)
{
    u8 i;

    
    for(i=0; i<8; i++)
    {
    	if((byte << i) & 0x80)
    	{
            LCM12864_SID_HIGH();
    	}
    	else 
    	{
            LCM12864_SID_LOW();
    	}
        
        LCM12864_SCLK_LOW();
        LCM12864_SCLK_HIGH();
    }

    LCM12864_SCLK_LOW();
    LCM12864_SID_LOW();
}

void lcm12864_write_cmd(u8 cmd)
{
    LCM12864_CS_HIGH();
    lcm12864_send_byte(0xf8);
    lcm12864_send_byte(cmd & 0xf0);
    lcm12864_send_byte((cmd << 4) & 0xf0);
    mdelay(2);
    LCM12864_CS_LOW();
}

void lcm12864_write_data(u8 data)
{
    LCM12864_CS_HIGH();
    lcm12864_send_byte(0xfa);
    lcm12864_send_byte(data & 0xf0);
    lcm12864_send_byte((data << 4) & 0xf0); 
    mdelay(2);
    LCM12864_CS_LOW();
}

void lcm12864_disp_pos(u8 x, u8 y)
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

    lcm12864_write_cmd(pos);

    mdelay(5);
}

void lcm12864_clr_scr(void)
{
    lcm12864_write_cmd(0x01);
}

void lcm12864_addr_home(void)
{
    lcm12864_write_cmd(0x02);
}

void lcm12864_high_light(u8 x, u8 y)
{
    lcm12864_disp_pos(x, y);
    lcm12864_write_cmd(0x0f);
}

void lcm12864_disp_normal(void)
{
    lcm12864_write_cmd(0x0c);
}

void lcm12864_cursor_left(void)
{
    lcm12864_write_cmd(0x10);
}

void lcm12864_disp_string(u8 x, u8 y, u8 *str)
{
    lcm12864_disp_pos(x, y);
    
    while(*str)
    {
        lcm12864_write_data(*str);
        str++;
    }
}

void lcm12864_disp_init(void)
{
    u8 disp_tmp_buf[MAX_LCM_DISP_TMP_BUF_LEN];
    u8 disp_buf_len;


    lcm12864_disp_string(g_time_default_pos[TIME_DEFAULT_STR_POS][0], g_time_default_pos[TIME_DEFAULT_STR_POS][1], (u8 *)g_time_default_str);
    
    disp_buf_len = 8;
    sprintf((char *)disp_tmp_buf, "%02d时%02d分", g_time_default.hour, g_time_default.minute);
    disp_tmp_buf[disp_buf_len] = '\0';
    lcm12864_disp_string(g_time_default_pos[TIME_DEFAULT_POS][0], g_time_default_pos[TIME_DEFAULT_POS][1], disp_tmp_buf);
    
    lcm12864_disp_string(g_time_count_pos[TIME_COUNT_STR_POS][0], g_time_count_pos[TIME_COUNT_STR_POS][1], (u8 *)g_time_count_str);    
    
    lcm12864_disp_string(g_time_count_pos[TIME_COUNT_POS][0], g_time_count_pos[TIME_COUNT_POS][1], "00:00:00");

    lcm12864_disp_string(g_flow_pos[FLOW_STR_POS][0], g_flow_pos[FLOW_STR_POS][1], (u8 *)g_flow_str);

    disp_buf_len = 10;
    sprintf((char *)disp_tmp_buf, "%d.%d SLPM  "/* 2 Space */, p_flow_ext_param->int_part, p_flow_ext_param->dec_part);
    disp_tmp_buf[disp_buf_len] = '\0';
    lcm12864_disp_string(g_flow_pos[FLOW_POS][0], g_flow_pos[FLOW_POS][1], disp_tmp_buf);  

    lcm12864_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_pause_str);
}

void init_lcm12864(void)
{
    LCM12864_CS_LOW();
    LCM12864_SCLK_LOW();
    LCM12864_SID_LOW();

    mdelay(5);
    lcm12864_clr_scr();
    lcm12864_addr_home();
    lcm12864_disp_normal();
}

