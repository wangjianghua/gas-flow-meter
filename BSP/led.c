#include "includes.h"


void LED_On(INT8U led_id)
{
    switch (led_id) {
    case LED_RUN:
        GPIO_ResetBits(LED_RUN_PORT, LED_RUN_PIN);
        break;

    default:
        break;
    }
}

void LED_Off(INT8U led_id)
{
    switch (led_id) {
    case LED_RUN:
         GPIO_SetBits(LED_RUN_PORT, LED_RUN_PIN);
         break;   
         
    default:
         break;
    }    
}

void LED_Toggle(INT8U led_id)
{
    switch (led_id) {
    case LED_RUN:
         GPIO_ToggleBits(LED_RUN_PORT, LED_RUN_PIN);
         break;   

    default:
         break;
    }     
}

void LED_Init(void)
{
    LED_Config();

    LED_Off(LED_RUN);
}

