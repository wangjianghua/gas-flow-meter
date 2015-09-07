#include "includes.h"


TIME_COUNT g_time_count;

/*
*********************************************************************************************************
*                                             App_TaskTime()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskTime()' by 'OSTaskCreateExt()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  App_TaskTime (void *p_arg)
{   
    INT8U cur_sec = 0, last_sec = 0;

    
    (void)p_arg; 
    
    while (DEF_TRUE) {  
        LED_RUN_TOGGLE();

        form_refresh();

        RTC_ReadTime(g_rtc_time);

        cur_sec = Bcd2Hex(g_rtc_time[SEC]);

        if(last_sec != cur_sec)
        { 
            g_time_count.second += (last_sec < cur_sec) ? (cur_sec - last_sec) : (cur_sec + 60 - last_sec);

            last_sec = cur_sec;
            
            if(g_time_count.second >= 60)
            {
                g_time_count.second %= 60;

                g_time_count.minute++;

                if(g_time_count.minute >= 60)
                {
                    g_time_count.minute = 0;

                    g_time_count.hour++;

                    if(g_time_count.hour >= MAX_TIME_COUNT)
                    {
                        g_time_count.hour = 0;
                        g_time_count.minute = 0;
                        g_time_count.second = 0;
                    }
                }
            }
        }
        
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

