#include "includes.h"


void LED_On(INT8U led_id)
{
    switch (led_id) {
    case LED_RUN:
        GPIO_ResetBits(LED_RUN_PORT, LED_RUN_PIN);
        break;

    case LED_MEMS:
        GPIO_ResetBits(LED_MEMS_PORT, LED_MEMS_PIN);
        break;

    case LED_UART:
        GPIO_ResetBits(LED_UART_PORT, LED_UART_PIN);
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

    case LED_MEMS:
         GPIO_SetBits(LED_MEMS_PORT, LED_MEMS_PIN);
         break;

    case LED_UART:
         GPIO_SetBits(LED_UART_PORT, LED_UART_PIN);
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

    case LED_MEMS:
         GPIO_ToggleBits(LED_MEMS_PORT, LED_MEMS_PIN);
         break;

    case LED_UART:
         GPIO_ToggleBits(LED_UART_PORT, LED_UART_PIN);
         break;

    default:
         break;
    }     
}

void LED_Init(void)
{
    LED_Config();

    LED_Off(LED_RUN);
    LED_Off(LED_MEMS);
    LED_Off(LED_UART);
}

