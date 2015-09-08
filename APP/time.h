#ifndef __TIME_H__
#define __TIME_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define MAX_TIME_COUNT                99u

typedef enum
{
    TIME_COUNT_PAUSE = 0,
    TIME_COUNT_RUN,
    TIME_COUNT_FINISH,
} TIME_COUNT_CTRL;

typedef struct 
{
    INT8U second;
    INT8U minute;
    INT16U hour;
} TIME_COUNT, *P_TIME_COUNT;

typedef struct 
{
    INT8U minute;
    INT16U hour;
} ALARM_TIME, *P_ALARM_TIME;

extern TIME_COUNT g_time_count;
extern ALARM_TIME g_alarm_time;
extern TIME_COUNT_CTRL g_time_count_ctrl;

void TIME_Init(void);
void time_key_proc(int key_msg);
void App_TaskTime(void *p_arg);


#ifdef __cplusplus
}
#endif

#endif
