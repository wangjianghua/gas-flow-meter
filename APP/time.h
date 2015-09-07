#ifndef __TIME_H__
#define __TIME_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define MAX_TIME_COUNT               999u

typedef struct 
{
    INT8U second;
    INT8U minute;
    INT16U hour;
} TIME_COUNT, *P_TIME_COUNT;

extern TIME_COUNT g_time_count;

void App_TaskTime(void *p_arg);


#ifdef __cplusplus
}
#endif

#endif
