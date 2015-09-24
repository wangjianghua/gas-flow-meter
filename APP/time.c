#include "includes.h"


TIME_COUNT g_time_count;
ALARM_TIME g_alarm_time;
TIME_COUNT_CTRL g_time_count_ctrl;

void TIME_Init(void)
{
    g_time_count_ctrl = TIME_COUNT_PAUSE;

    memset(&g_time_count, 0, sizeof(TIME_COUNT));

    g_alarm_time.hour = MAX_TIME_COUNT;
    g_alarm_time.minute = 59;
}

void time_relay_proc(void)
{
    if(TIME_COUNT_RUN == g_time_count_ctrl)
    {
        relay_turn_on();
    }
    else
    {
        relay_turn_off();
    }
}

void time_key_proc(int key_msg)
{
    switch(key_msg)
    {
    case KEY_MSG_PAUSE:
        if(TIME_COUNT_RUN == g_time_count_ctrl)
        {
            g_time_count_ctrl = TIME_COUNT_PAUSE;
        }
        else if(TIME_COUNT_PAUSE == g_time_count_ctrl)
        {
            g_time_count_ctrl = TIME_COUNT_RUN;
        }
        else
        {
            memset(&g_time_count, 0, sizeof(TIME_COUNT));

            g_time_count_ctrl = TIME_COUNT_RUN;
        }
        break;

    case KEY_MSG_CLEAR:
        if(TIME_COUNT_FINISH != g_time_count_ctrl)
        {
            memset(&g_time_count, 0, sizeof(TIME_COUNT));
        }
        break;

    default:
        return;
        break;
    }

    time_relay_proc();
}

void time_alarm_proc(void)
{
    g_time_count_ctrl = TIME_COUNT_FINISH;

    beep_on();

    time_relay_proc();
}

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
    INT8U cur_sec, last_sec;

    
    (void)p_arg; 

    RTC_ReadTime(g_rtc_time);

    last_sec = Bcd2Hex(g_rtc_time[SEC]);
    
    while (DEF_TRUE) {  
        LED_RUN_TOGGLE();

        RTC_ReadTime(g_rtc_time);

        cur_sec = Bcd2Hex(g_rtc_time[SEC]);

        if((TIME_COUNT_RUN == g_time_count_ctrl) && (last_sec != cur_sec))
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

                    if(g_time_count.hour > MAX_TIME_COUNT)
                    {
                        g_time_count.hour = 0;
                        g_time_count.minute = 0;
                        g_time_count.second = 0;
                    }
                }
            }

            if((g_time_count.hour == g_alarm_time.hour) && 
               (g_time_count.minute >= g_alarm_time.minute))
            {
                time_alarm_proc();
            }
        }
        else
        {
            last_sec = cur_sec;
        }

        menu_refresh();
        
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

