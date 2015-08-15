#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define KEY_MSG_SET                0
#define KEY_MSG_UP                 1
#define KEY_MSG_DOWN               2   
#define KEY_MSG_PAUSE              3
#define KEY_MSG_CLEAR              4

#define TIME_MINUTE_SET            1
#define TIME_HOUR_SET              2
#define FLOW_DEC_SET               3
#define FLOW_INT_SET               4
#define CAL_DEC_SET                5
#define CAL_INT_SET                6

#define KEY_SET_PIN       GPIO_Pin_4
#define KEY_SET_PORT           GPIOA

#define KEY_UP_PIN        GPIO_Pin_5
#define KEY_UP_PORT            GPIOA

#define KEY_DOWN_PIN      GPIO_Pin_6
#define KEY_DOWN_PORT          GPIOA

#define KEY_PAUSE_PIN     GPIO_Pin_7
#define KEY_PAUSE_PORT         GPIOA

#define KEY_CLEAR_PIN     GPIO_Pin_0
#define KEY_CLEAR_PORT         GPIOB

#define DEFAULT_KEY_COUNT          3

typedef struct {
    u8 msg;
    u8 last_msg;
    u8 count;

    u8 pause_cnt;
    u8 clear_cnt;
    u8 set_cnt;
    u8 up_cnt;
    u8 down_cnt;
} KEY_PARAM; 

extern KEY_PARAM g_key_param;
extern bool g_param_setting;

void init_key(void);


#ifdef __cplusplus
}
#endif

#endif
