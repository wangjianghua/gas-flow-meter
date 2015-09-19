#include "includes.h"


END_OBJ g_EndObjectPool[MAX_COM_PORT] =
{
    {PC_COM_PORT, END_STATUS_IDLE, 0, 0, 0, NULL, NULL, NULL, NULL},
    {MEMS_COM_PORT, END_STATUS_IDLE, 0, 0, 0, NULL, NULL, NULL, NULL},
};

const pvoid g_EndObject[END_OBJECT_NUM] =
{
    PC_UART,
    MEMS_UART,
};

queue *g_EndTxQueue[MAX_COM_PORT] = {NULL};
queue *g_EndRxQueue[MAX_COM_PORT] = {NULL};

unsigned char g_PCTxEndQueueBuf[((END_TX_QUEUE_SIZE+2) * 4)];
unsigned char g_PCRxEndQueueBuf[((END_RX_QUEUE_SIZE+2) * 4)];

unsigned char g_MEMSTxEndQueueBuf[((END_TX_QUEUE_SIZE+2) * 4)];
unsigned char g_MEMSRxEndQueueBuf[((END_RX_QUEUE_SIZE+2) * 4)];

unsigned char *pTxEndQueueBuf[] = {
    g_PCTxEndQueueBuf,
    g_MEMSTxEndQueueBuf,
};

unsigned char *pRxEndQueueBuf[] = {
    g_PCRxEndQueueBuf,
    g_MEMSRxEndQueueBuf,
};

/***********************************************************
END层初始化< // 需要提供一个reset 接口供上层调用>
************************************************************/

OS_EVENT *g_sem_end;

UART_CCB g_uart_ccb[MAX_COM_PORT];

U8 g_UartPCRxBuf[UART_RECV_BUF_SIZE];
U8 g_UartMEMSRxBuf[UART_RECV_BUF_SIZE];

U8 *pUartRxBuf[] = {
    g_UartPCRxBuf,
    g_UartMEMSRxBuf,
};

U32 UART_ReceiveData(U8 end_id, UCHAR* rxbuf, USHORT rxnum)
{
    OS_CPU_SR_ALLOC();
    P_UART_CCB p_uc = &g_uart_ccb[end_id];

    if( (rxnum < 1) || (end_id >= MAX_COM_PORT) )
    {
        return FALSE;
    }
    OS_ENTER_CRITICAL();
    //p_uc->gUartRxCnt = 0;
    //p_uc->gUartRxLen = rxnum;
    p_uc->gpUartRxAddress = rxbuf;
    p_uc->gpUartRxStartAddress = rxbuf;
    p_uc->gpUartRxReadAddress = rxbuf;
    p_uc->gpUartRxEndAddress = (UCHAR*)((ULONG)rxbuf + rxnum);
    OS_EXIT_CRITICAL()
    return TRUE;
}


void End_Init(void)
{
    P_END_OBJ pEndObj = NULL;
    unsigned char i;
    

    mem_msg_buffer_init((MSG_INFO *)gShortMsgPool, (P_MSG_INFO *)pShortMsgPool, MAX_MSG_SHORT, sizeof(MSG_SHORT_INFO));

    //alan test  需要暂时注释掉, 不知为啥IIC Start 一调用, MCU 就飞啦.
    for( i = START_COM_PORT; i < MAX_COM_PORT; i++)
    {
        // 找到当前End Object
        pEndObj = g_EndObjectPool + i;

        /* end queue[x] initialize */ /* each end object define '50 block' queue */
        g_EndTxQueue[i] = define_new_queue((queue *)pTxEndQueueBuf[i], END_TX_QUEUE_SIZE);
        g_EndRxQueue[i] = define_new_queue((queue *)pRxEndQueueBuf[i], END_RX_QUEUE_SIZE);
       
        pEndObj->end_recv_buffer = (unsigned char *)pUartRxBuf[i];

        pEndObj->last_receive_len = 0;
        pEndObj->receive_len = 0;

        pEndObj->recv_timeout = 0;

        UART_ReceiveData(i, pEndObj->end_recv_buffer, UART_RECV_BUF_SIZE);

        // 所有串口状态转到REVC STATUS
        pEndObj->end_send_status = END_STATUS_IDLE;

        USART_ITConfig((USART_TypeDef *)g_EndObject[i], USART_IT_RXNE, ENABLE);
    }
}


/***********************************************************
Tick任务调用，检查每个END接口是否有新的frame收完
************************************************************/
unsigned short End_tick_check(void)
{
    OS_CPU_SR_ALLOC();
    unsigned char i;
    U16 cp_len, msg_len;

    P_END_OBJ     pEndObj = NULL;
    P_MSG_INFO     pnewmsg = NULL;

    P_UART_CCB p_uc;

    for(i = START_COM_PORT; i < MAX_COM_PORT; i++ )
    {
        /////////////////////////////
        pEndObj = g_EndObjectPool + i;

        p_uc = &g_uart_ccb[i];

        if(pEndObj->end_send_status == END_STATUS_SENDING)
        {
            /* 检查当前对象是否发送完毕. 完毕的话, 转接收,   或者转空闲*/
            if( End_check_send(pEndObj->end_id) == TRUE )
            {

                //Send  已经结束， 转到receive status 初始状态
                pEndObj->end_send_status = END_STATUS_IDLE;

                pnewmsg = (P_MSG_INFO)pEndObj->pMsgInfo;

                pnewmsg->msg_header.block_state = SENDED;

                // alan dynamic sending buffer.
                // 当前end sending 正常结束，可以直接释放sending buffer
                if( pnewmsg->msg_header.need_buffer_free == TRUE)    /* 不需要保留到上层application 进行释放*/
                {
                    free_send_buffer(pnewmsg);
                }
            }
        }
        else
        {
            if( NULL != (pnewmsg = dequeue( g_EndTxQueue[pEndObj->end_id])) )
            {
                End_send(pnewmsg);
            }
        }

        if( End_check_recv(pEndObj) == TRUE ) /* 没有新数据到来, 开始处理*/
        {
#if 0            
            // 将内存数据copy  出来, 压入queue 进行后续处理
            if(pEndObj->receive_len >= MSA_MESSAGAE_MAX_SIZE)
                pnewmsg = alloc_send_buffer(MSG_LARGE);
            else if(pEndObj->receive_len >= PLC_BUFFER_UART_SIZE)
                pnewmsg = alloc_send_buffer(MSG_LONG);
            else
                pnewmsg = alloc_send_buffer(MSG_SHORT);
#else
            pnewmsg = alloc_send_buffer(MSG_SHORT);
#endif

            if(pnewmsg == NULL)
            {               
                continue;
            }

            msg_len = pEndObj->receive_len;

            pnewmsg->msg_header.msg_len = msg_len;
            pnewmsg->msg_header.end_id = i;

            OS_ENTER_CRITICAL();
            cp_len = (U16)(p_uc->gpUartRxEndAddress - p_uc->gpUartRxReadAddress);
            

            if(cp_len >= msg_len)
            {
                //OS_ENTER_CRITICAL();
                memcpy(pnewmsg->msg_buffer, p_uc->gpUartRxReadAddress, msg_len );                
                p_uc->gpUartRxReadAddress += msg_len;
                if(p_uc->gpUartRxReadAddress == p_uc->gpUartRxEndAddress)
                    p_uc->gpUartRxReadAddress = p_uc->gpUartRxStartAddress;
                //OS_EXIT_CRITICAL();
            }
            else
            {
                //OS_ENTER_CRITICAL();
                memcpy(pnewmsg->msg_buffer, p_uc->gpUartRxReadAddress, cp_len);
                memcpy((pnewmsg->msg_buffer + cp_len), p_uc->gpUartRxStartAddress, (msg_len-cp_len));                
                p_uc->gpUartRxReadAddress = p_uc->gpUartRxStartAddress + (msg_len-cp_len);
                //OS_EXIT_CRITICAL();
            }
            OS_EXIT_CRITICAL();

            pEndObj->last_receive_len = 0;
            pEndObj->receive_len = 0;

            pEndObj->endStatistics.rxPacketCount++;

            /* 将数据挂在queue 里*/
            if(enqueue(g_EndRxQueue[pEndObj->end_id], (P_MSG_INFO)(pnewmsg)))
            {			
                // 发送UART Rx 处理事件量
                OSSemPost(g_sem_end);
            }
            else
            {
                free_send_buffer(pnewmsg);
            }       
        }
    }

    return TRUE;
}


P_END_OBJ End_get_end_obj(UCHAR end_id)
{
    unsigned char i = 0;
    while( i < MAX_COM_PORT)
    {
        if( g_EndObjectPool[i].end_id == end_id)
            return &g_EndObjectPool[i];
        i++;
    }

    return NULL;
}

U32 End_uart_send(UCHAR end_id, UCHAR *txbuf, USHORT txnum)
{
    USART_TypeDef *UARTx;
    P_UART_CCB p_uc;
    UCHAR send_byte = 0;
    USHORT mems_frame_head = 0;
    

    if(txnum < 1)
    {
        return FALSE;
    }

    switch(end_id)
    {    
    case PC_COM_PORT:      
        UARTx = PC_UART;
        break; 
        
    case MEMS_COM_PORT:      
        UARTx = MEMS_UART;

        mems_frame_head = 0x100;
        break;  
        
    default:
        return FALSE;
    }

    p_uc = &g_uart_ccb[end_id];

    p_uc->gpUartTxAddress = txbuf;
    p_uc->gUartTxCnt = txnum;

    send_byte = *(p_uc->gpUartTxAddress);

    p_uc->gpUartTxAddress++;
    p_uc->gUartTxCnt--;

    USART_SendData(UARTx, send_byte | mems_frame_head);

    if(p_uc->gUartTxCnt)
        USART_ITConfig(UARTx, USART_IT_TXE, ENABLE);

    return TRUE;
}

/***********************************************************
pEndObj:  发送接口
pMsgInfo: 发送消息内容通过链路层发送消息
************************************************************/
unsigned short End_send( P_MSG_INFO pMsgInfo)
{
    P_END_OBJ pEndObj;

    if( (( pEndObj = End_get_end_obj(pMsgInfo->msg_header.end_id)) == NULL) ||
            ( pMsgInfo->msg_header.msg_len == 0) )
    {
        // alan dynamic sending buffer.
        // 这些发送失败时，这里不直接释放sending buffer，由end send 调用释放。
        if( pMsgInfo->msg_header.need_buffer_free == TRUE)
        {
            free_send_buffer(pMsgInfo);
        }

        return FALSE;
    }


    if( END_IDLE != End_IsIdle(pEndObj) )
    {
        /* 不在IDLE State, 将数据挂在queue 里*/
        enqueue(g_EndTxQueue[pEndObj->end_id], pMsgInfo);

        pMsgInfo->msg_header.time_stamp = OSTimeGet();

        return TRUE;
    }


    if( TRUE == End_uart_send( pEndObj->end_id, pMsgInfo->msg_buffer, pMsgInfo->msg_header.msg_len) )
    {

        // 发送buffer 直接挂在end object ， 后续可以直接释放!
        pEndObj->pMsgInfo = pMsgInfo;

        // 底层状态迁移到SENDING
        pEndObj->end_send_status = END_STATUS_SENDING;

        pMsgInfo->msg_header.block_state = SENDING;

        pEndObj->endStatistics.txPacketCount++;

        pMsgInfo->msg_header.time_stamp = OSTimeGet();

        
        return TRUE;
    }
    else
    {
        if( pMsgInfo->msg_header.need_buffer_free == TRUE)
        {
            free_send_buffer(pMsgInfo);
        }

        return FALSE;
    }

}

unsigned char End_check_recv(P_END_OBJ pEndObj)
{
    P_UART_CCB p_uc = &g_uart_ccb[pEndObj->end_id];

    // 实时记录当前UART Rx 数据长度
    pEndObj->last_receive_len = pEndObj->receive_len;

    //pEndObj->receive_len = gUart1RxCnt;
    if(p_uc->gpUartRxReadAddress <= p_uc->gpUartRxAddress)
        pEndObj->receive_len = p_uc->gpUartRxAddress - p_uc->gpUartRxReadAddress;//gIic0RxCnt;
    else
        pEndObj->receive_len = (USHORT)((ULONG)p_uc->gpUartRxAddress + UART_RECV_BUF_SIZE - (ULONG)p_uc->gpUartRxReadAddress);

    if(pEndObj->receive_len > (220))//if(pEndObj->receive_len > (UART_RECV_BUF_SIZE/2))
    {
        pEndObj->recv_timeout = 0;
        return TRUE;
    }


    if((pEndObj->receive_len != 0) && (pEndObj->receive_len == pEndObj->last_receive_len))
    {
        /* 没有新的接收数据到了,直接返回FALSE 告诉上层可以处理啦*/
        pEndObj->recv_timeout++;
        if(pEndObj->recv_timeout > 1)
        {
            pEndObj->recv_timeout = 0;
            return TRUE;
        }
        else
        {
            return FALSE;
        }

    }
    else
    {
        /* 还有新的接收数据*/
        pEndObj->recv_timeout = 0;
        return FALSE;
    }

}

/**********************************************

判断当前End Send 数据发送完没有. 发送完返回TRUE,  没发送完返回FALSE

**********************************************/
unsigned char End_check_send(UCHAR end_id)
{

    if( g_uart_ccb[end_id].gUartTxCnt == 0)
        /* 发送数据完成, 返回TRUE 告诉上层状态迁移*/
        return TRUE;
    else
        /* 还有数据没发送完成, 继续发送*/
        return FALSE;

}


/**************************************************************************************

函数描述:       底层抄表数据回来回调函数. 在这里将数据写入表内存, 发起下一个表项抄写.

输入参数:      回程数据结果.

**************************************************************************************/
unsigned char End_postProcess(unsigned char end_type,  pvoid h)
{
    P_MSG_INFO pMsg = (P_MSG_INFO)h;
    USHORT iRet = FALSE;
    

    switch(end_type)
    {
    case PC_COM_PORT:
        iRet = PC_postProcess(pMsg);
        break;

    case MEMS_COM_PORT:
        iRet = MEMS_postProcess(pMsg);
        break;            

    default:
        break;  
    }

    free_send_buffer(pMsg);

    return iRet;
}


// 查询底层end 当前状态是否空闲??
unsigned char End_IsIdle(P_END_OBJ pEndObj)
{

    if( pEndObj->end_send_status == END_STATUS_IDLE)
        return END_IDLE;
    else
        return END_BUSY;
    

}

void UART_IRQProc(UART_CCB *uccb, USART_TypeDef *UARTx)
{
  if(USART_GetITStatus(UARTx, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */    
    *(uccb->gpUartRxAddress) = USART_ReceiveData(UARTx);
    uccb->gpUartRxAddress++;

    if(uccb->gpUartRxAddress == uccb->gpUartRxEndAddress)
    {
        //回头
        uccb->gpUartRxAddress = uccb->gpUartRxStartAddress;
    }
	
    if(uccb->gpUartRxReadAddress == uccb->gpUartRxAddress)
    {
        //可以考虑加错误统计
    } 
    
    LED_UART_ON();
  }

  if(USART_GetITStatus(UARTx, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    if( uccb->gUartTxCnt > 0 )
    {
        USART_SendData(UARTx, *(uccb->gpUartTxAddress));
    	uccb->gpUartTxAddress++;   	
    	uccb->gUartTxCnt--;
	}
    else
    {        
        USART_ITConfig(UARTx, USART_IT_TXE, DISABLE);
    }

    LED_UART_ON();
  }
}

/*
*********************************************************************************************************
*                                             App_TaskEndTick()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskEndTick()' by 'OSTaskCreateExt()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  App_TaskEndTick (void *p_arg)
{
    (void)p_arg; 
    
    while (DEF_TRUE) {    
        End_tick_check();
        
        LED_UART_OFF();

        OSTimeDlyHMSM(0, 0, 0, 20);
    }
}

/*
*********************************************************************************************************
*                                             App_TaskEndProc()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskEndProc()' by 'OSTaskCreateExt()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  App_TaskEndProc (void *p_arg)
{
    INT8U i, err;
    P_END_OBJ pEndObj = NULL;
    P_MSG_INFO pMsg = NULL;
    

    (void)p_arg;
    
    while (DEF_TRUE) { 
        OSSemPend(g_sem_end, 0, &err);

        for(i = START_COM_PORT; i < MAX_COM_PORT; i++)
        {
            pEndObj = g_EndObjectPool + i;

            while((pMsg = dequeue(g_EndRxQueue[pEndObj->end_id])) != NULL)
            {
                if(pMsg->msg_header.end_id == pEndObj->end_id)
                {
                    End_postProcess(pEndObj->end_id, pMsg);
                }
                else
                {
                    free_send_buffer(pMsg);
                }
            }
        }
    }
}

