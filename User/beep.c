#include "includes.h"


u32 g_beep_cnt;

void init_beep(void)
{
    beep_off();
}

void beep(void)
{
    if(g_beep_cnt)
    {
        g_beep_cnt--;
        
        BUZZER_ON();

        if(!g_beep_cnt)
        {
            BUZZER_OFF();
        }
    }
}

void beep_on(void)
{
    g_beep_cnt = DEFAULT_BEEP_PERIOD;
}

void beep_off(void)
{
    g_beep_cnt = 0;
}

