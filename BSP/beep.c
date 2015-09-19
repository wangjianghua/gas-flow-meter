#include "includes.h"


INT32U g_beep_count;

void BEEP_Init(void)
{
    BEEP_Config();
    
    beep_off();
}

void beep(void)
{
    if(g_beep_count)
    {
        g_beep_count--;
        
        BUZZER_ON();

        if(0 == g_beep_count)
        {
            BUZZER_OFF();
        }
    }
}

void beep_on(void)
{
    g_beep_count = BEEP_PERIOD;
}

void beep_off(void)
{
    g_beep_count = 0;

    BUZZER_OFF();
}

