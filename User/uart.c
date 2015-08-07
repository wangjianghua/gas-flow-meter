#include "includes.h"


COM_PARAM g_com_param;

u8 uart_send_data(u16 *tx_buf, u16 tx_num)
{
	u8 status = 0;
    u32 start_time;
    
    
    start_time = get_tick();
    
    while(g_flow_param.tx.bytes)
    {
        clr_wdt();

        if(check_timeout(start_time, g_flow_param.tx.check_time)) //检测串口发送超时
        {
#if (UART_DEBUG_EN > 0u)
            g_flow_param.send_timeout_times++;
#endif

            /* 初始化串口 */

            break;
        }       
    }

	if (tx_num < 1)
	{
		status = 1;
	}
	else
	{
	    g_flow_param.tx.completed = FALSE;
		g_flow_param.tx.addr = tx_buf;
		g_flow_param.tx.bytes = tx_num;
        USART_ITConfig(USART2, USART_IT_TC, DISABLE);
        USART_SendData(USART2, *g_flow_param.tx.addr);
		g_flow_param.tx.addr++;
		g_flow_param.tx.bytes--;
        USART_ITConfig(USART2, USART_IT_TC, ENABLE);
	}
  
	return (status);
}

u16 get_bytes(void)
{
    return (g_flow_param.rx.bytes);
}

u8 get_char(void)
{
    u8 ch;


    ch = *g_flow_param.rx.read_index;
    g_flow_param.rx.read_index++;

    if(g_flow_param.rx.read_index >= (g_flow_param.rx.buf + MAX_UART_RECV_BUF_LEN))
    {
        g_flow_param.rx.read_index = g_flow_param.rx.buf;
    }

    DI();
    g_flow_param.rx.bytes--;
    EI();

    return (ch);
}

void uart_send_end_callback(void)
{
    g_flow_param.tx.completed = TRUE;
}

