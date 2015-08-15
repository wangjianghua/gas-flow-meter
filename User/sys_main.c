/** 
 * @file     main.c
 * @brief    gas-flow-meter
 * @details  气体流量计
 * @author   华兄
 * @email    591881218@qq.com
 * @date     2015
 * @version  vX.XX
 * @par Copyright (c):  
 *           华兄电子
 * @par History:          
 *   version: author, date, desc\n 
 */

#include "includes.h"


static const EVENT_PROC_PTR _sys_evt_proc[] =
{
    on_tick,
    on_second,
    on_time,
    on_key_press,
    on_uart_recv,
    on_flow
};

int  main(void)
{
    _sys_pos_t pos;
    _sys_event_t test_event_mask;


    bsp_init();
    mem_init();

test_event:
    test_event_mask = 1;
    
    for( pos = 0; pos < SYS_EVENT_MAX; pos++ )
    {
        if( test_event_mask & SYS_EVT_VAR )
        {
            SYS_EVT_PROC(pos);
            
            sys_evt_del(test_event_mask);
            
            clr_wdt();
            
            goto test_event;
        }
        
        if( 0 == ( SYS_EVT_VAR & SYS_EVENT_MASK ) )
        {
#if (LOW_POWER_EN > 0u)

#endif

            break;
        }
        
        test_event_mask <<= 1;
    }
    
    goto test_event;
}

