#include "includes.h"


void RELAY_Init(void)
{
    RELAY_Config();

    RELAY_OFF();
}

void relay_turn_on(void)
{
    RELAY_ON();
}

void relay_turn_off(void)
{
    RELAY_OFF();
}

