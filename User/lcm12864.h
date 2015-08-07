#ifndef __LCM12864_H__
#define __LCM12864_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define LCM_REFRESH_PERIOD         1
#define MAX_LCM_DISP_TMP_BUF_LEN  16
 
void init_lcm12864(void);
void lcm12864_disp_init(void);
void lcm12864_disp_string(u8 x, u8 y, u8 *str);
void lcm12864_disp_normal(void);
void lcm12864_high_light(u8 x, u8 y);
void lcm12864_cursor_left(void);


#ifdef __cplusplus
}
#endif

#endif
