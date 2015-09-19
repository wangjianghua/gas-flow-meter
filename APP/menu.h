#ifndef __MENU_H__
#define __MENU_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define LCD_BACKLIGHT_DISP_TIMEOUT              300u

typedef enum
{
    FORM_MSG_NONE = 0,
    FORM_MSG_REFRESH,
    FORM_MSG_KEY,
    FORM_MSG_DATA,
} FORM_MSG;

typedef enum
{
    FORM_ID_HOME = 0,
    FORM_ID_SYSINFO,
    FORM_ID_SET,
    MAX_FORM_NUM,
} FORM_ID;

typedef enum 
{
    MENU_SET_ALARM_TIME,
    MENU_SET_TARGET_FLOW,
    MENU_SET_RTC_TIME,
    MENU_SET_MAX,
} MENU_SET;

typedef enum 
{
    ALARM_TIME_SET_HOUR,
    ALARM_TIME_SET_MINUTE,
    ALARM_TIME_SET_CONFIRM,
    ALARM_TIME_SET_QUIT,
    ALARM_TIME_SET_MAX,
} ALARM_TIME_SET;

typedef enum 
{
    TARGET_FLOW_SET_INT_PART,
    TARGET_FLOW_SET_DEC_PART,
    TARGET_FLOW_SET_CONFIRM,
    TARGET_FLOW_SET_QUIT,
    TARGET_FLOW_SET_MAX,
} TARGET_FLOW_SET;

typedef enum 
{
    RTC_TIME_SET_YEAR,
    RTC_TIME_SET_MONTH,
    RTC_TIME_SET_DATE,
    RTC_TIME_SET_HOUR,
    RTC_TIME_SET_MINUTE,
    RTC_TIME_SET_SECOND,
    RTC_TIME_SET_SAVE,
    RTC_TIME_SET_QUIT,
    RTC_TIME_SET_MAX,
} RTC_TIME_SET;

typedef int (*FORM)(unsigned int key_msg, unsigned int form_msg);

void MENU_Init(void);    
void menu_refresh(void);
void menu_key_proc(int key_msg);


#ifdef __cplusplus
}
#endif

#endif
