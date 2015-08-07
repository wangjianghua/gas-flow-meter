#include "includes.h"


_sys_tick_t _sys_tick = NO_TICK;
_sys_sec_t _sys_sec = NO_SEC;
_sys_event_t _sys_event = NO_EVT;

void sys_evt_add( _sys_event_t evt )
{
    _disable_interrupt();
    _sys_event |= evt;
    _enable_interrupt();
}

void sys_evt_del( _sys_event_t evt )
{
    _disable_interrupt();
    _sys_event &= ~( evt | SYS_EVT_RESERVE );
    _enable_interrupt();
}

void on_tick(void)
{
    u8 disp_tmp_buf[MAX_LCM_DISP_TMP_BUF_LEN];
    u8 disp_buf_len;
    u8 pos;


    _sys_tick++;

    if(!(_sys_tick % SYS_TICKS_PER_SEC))
    {
        sys_evt_add(SYS_EVT_SECOND);
    }

    if(g_key_param.count) //按键处理
    {
        g_key_param.count--;

        if(0 == g_key_param.count)
        {
            switch(g_key_param.last_msg)
            {
            case KEY_MSG_SET:
                if(Bit_RESET == GPIO_ReadInputDataBit(KEY_SET_PORT, KEY_SET_PIN))
                {
                    if(FALSE == g_time_count_complete)
                    {
                        g_key_param.set_cnt++;

                        if(g_key_param.set_cnt > 2)
                        {
                            g_key_param.set_cnt = 0;
                            g_time_setting = FALSE;
                            lcm12864_disp_normal();
                        }
                        else
                        {
                            g_time_setting = TRUE;
                            
                            pos = g_key_param.set_cnt - 1;
                            
                            lcm12864_disp_string(g_time_status_pos[TIME_STATUS_RUN_POS][0], g_time_status_pos[TIME_STATUS_RUN_POS][1], (u8 *)g_time_count_null_str);
                            lcm12864_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);
                            mdelay(10);
                            
                            lcm12864_high_light(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1]);
                        }

                        if(0 == g_key_param.set_cnt)
                        {
                            if((FALSE == g_time_count_pause) || (TRUE == g_time_set))
                            {
                                if(TRUE == g_time_set)
                                {
                                    g_time_set = FALSE;
                                
                                    g_time_count.hour = 0;
                                    g_time_count.minute = 0;
                                    g_time_count.second = 0;

                                    if(0 == g_time_default.hour &&
                                       0 == g_time_default.minute)
                                    {
                                        g_time_default.hour = MAX_TIME_VAL;
                                    }

                                    if(TRUE == g_time_count_pause)
                                    {
                                        g_time_count_pause = FALSE;
                                        g_key_param.pause_cnt = 0;
                                        
                                        // -----------
                                        RELAY_N_ON();
                                        RELAY_L_ON();
                                        // -----------   
                                    }
                                }

                                _enable_rtc();
                            }
                            else
                            {
                                lcm12864_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_pause_str);
                            }
                        }
                        else
                        {
                            _disble_rtc();
                        }
                    }
                    else
                    {
                        g_key_param.set_cnt = 0;
                    }
                }
                break;

            case KEY_MSG_UP:
                if(Bit_RESET == GPIO_ReadInputDataBit(KEY_UP_PORT, KEY_UP_PIN))
                {
                    switch(g_key_param.set_cnt)
                    {
                    case TIME_MINUTE_SET:
                        g_time_default.minute++;
                        if(g_time_default.minute > 59)
                        {
                            g_time_default.minute = 0;
                        }
                        disp_buf_len = 2;
                        sprintf((char *)disp_tmp_buf, "%02d", g_time_default.minute);
                        disp_tmp_buf[disp_buf_len] = '\0';
                        pos = g_key_param.set_cnt - 1;
                        lcm12864_disp_string(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1], disp_tmp_buf);
                        lcm12864_cursor_left();
                        g_time_set = TRUE;
                        break;

                    case TIME_HOUR_SET:
                        g_time_default.hour++;
                        if(g_time_default.hour > MAX_TIME_VAL)
                        {
                            g_time_default.hour = 0;
                        }
                        disp_buf_len = 2;
                        sprintf((char *)disp_tmp_buf, "%02d", g_time_default.hour);
                        disp_tmp_buf[disp_buf_len] = '\0';
                        pos = g_key_param.set_cnt - 1;
                        lcm12864_disp_string(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1], disp_tmp_buf);
                        lcm12864_cursor_left();
                        g_time_set = TRUE;
                        break;

                    default:
                        break;
                    }
                }                
                break;  

            case KEY_MSG_DOWN:
                if(Bit_RESET == GPIO_ReadInputDataBit(KEY_DOWN_PORT, KEY_DOWN_PIN))
                {
                    switch(g_key_param.set_cnt)
                    {
                    case TIME_MINUTE_SET:
                        g_time_default.minute--;
                        if(g_time_default.minute < 0)
                        {
                            g_time_default.minute = 59;
                        }
                        disp_buf_len = 2;
                        sprintf((char *)disp_tmp_buf, "%02d", g_time_default.minute);
                        disp_tmp_buf[disp_buf_len] = '\0';
                        pos = g_key_param.set_cnt - 1;
                        lcm12864_disp_string(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1], disp_tmp_buf);
                        lcm12864_cursor_left();
                        g_time_set = TRUE;
                        break;

                    case TIME_HOUR_SET:
                        g_time_default.hour--;
                        if(g_time_default.hour < 0)
                        {
                            g_time_default.hour = MAX_TIME_VAL;
                        }
                        disp_buf_len = 2;
                        sprintf((char *)disp_tmp_buf, "%02d", g_time_default.hour);
                        disp_tmp_buf[disp_buf_len] = '\0';
                        pos = g_key_param.set_cnt - 1;
                        lcm12864_disp_string(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1], disp_tmp_buf);
                        lcm12864_cursor_left();
                        g_time_set = TRUE;
                        break;

                    default:
                        break;
                    }
                }
                break;

            case KEY_MSG_PAUSE:
                if(Bit_RESET == GPIO_ReadInputDataBit(KEY_PAUSE_PORT, KEY_PAUSE_PIN))
                {
                    if((FALSE == g_time_count_complete) && (FALSE == g_time_setting))
                    {
                        g_key_param.pause_cnt++;
                        
                        if(!(g_key_param.pause_cnt % 2) || (TRUE == g_time_count_pause))
                        {
                            _enable_rtc();

                            g_key_param.pause_cnt = 0;

                            g_time_count_pause = FALSE;

                            lcm12864_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);    

                            // -----------
                            RELAY_N_ON();
                            RELAY_L_ON();
                            // ----------- 
                        }
                        else
                        {
                            _disble_rtc();

                            g_time_count_pause = TRUE;

                            lcm12864_disp_string(g_time_status_pos[TIME_STATUS_RUN_POS][0], g_time_status_pos[TIME_STATUS_RUN_POS][1], (u8 *)g_time_count_null_str);
                            lcm12864_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_pause_str);

                            // -----------
                            RELAY_N_OFF();
                            RELAY_L_OFF();
                            // -----------
                        }
                    }
                    else if(TRUE == g_time_count_complete)
                    {
                        g_key_param.pause_cnt = 0;
                        g_time_count_complete = FALSE;

                        g_time_default.hour = MAX_TIME_VAL;
                        g_time_default.minute = 0;
                        
                        g_time_count.hour = 0;
                        g_time_count.minute = 0;
                        g_time_count.second = 0;                        
                        
                        _enable_rtc();
                        
                        lcm12864_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);
                        lcm12864_disp_string(g_time_status_pos[TIME_STATUS_COMPLETE_POS][0], g_time_status_pos[TIME_STATUS_COMPLETE_POS][1], (u8 *)g_time_count_null_str);

                        // -----------
                        RELAY_N_ON();
                        RELAY_L_ON();
                        // -----------                        
                    }
                }                
                break;

            case KEY_MSG_CLEAR:
                if(Bit_RESET == GPIO_ReadInputDataBit(KEY_CLEAR_PORT, KEY_CLEAR_PIN))
                {
                    if((FALSE == g_time_count_complete) && 
                       (FALSE == g_time_count_pause) && 
                       (FALSE == g_time_setting))
                    {
                        g_time_count.second = 0;    
                        g_time_count.minute = 0;
                        g_time_count.hour = 0;
                    }
                    else
                    {
                        g_key_param.clear_cnt = 0;
                    }
                }                
                break;             

            default:
                break;
            }
        }
    }

    if(get_bytes()) //串口接收有数据
    {
        sys_evt_add(SYS_EVT_UART_RECV);
    }    

    if(g_flow_param.rx.timeout) //处理串口接收超时
    {
        g_flow_param.rx.timeout--;

        if(!g_flow_param.rx.timeout)
        {
#if (UART_DEBUG_EN > 0u)
            if(TRUE == g_flow_param.recv_pkt_overflow)
            {
                g_flow_param.recv_pkt_overflow = FALSE;
                DEBUG_PRINT(("<uart_recv_overflow> %s(%d) - %s\n", __FILE__, __LINE__, __FUNCTION__));
            }
#endif

            sys_evt_add(SYS_EVT_FLOW);
        }
    }    

    if(check_timeout(g_flow_param.rx.start_time, FS400X_RECV_TIMEOUT) && (FALSE == g_time_setting)) //处理FS4008数据接收超时
    {
        lcm12864_disp_string(g_flow_pos[FLOW_POS][0], g_flow_pos[FLOW_POS][1], "0.0 SLPM  "/* 2 Space */);

        g_flow_param.rx.start_time = get_tick();
    }

    // -----
    beep();
    // -----
}

void on_second(void)
{
    u8 disp_tmp_buf[MAX_LCM_DISP_TMP_BUF_LEN];
    u8 disp_buf_len;

    
    _sys_sec++;

    if((!(_sys_sec % LCM_REFRESH_PERIOD)) && (FALSE == g_time_setting))
    {
        disp_buf_len = 8;
        sprintf((char *)disp_tmp_buf, "%02d时%02d分", g_time_default.hour, g_time_default.minute);
        disp_tmp_buf[disp_buf_len] = '\0';
 
        lcm12864_disp_string(g_time_default_pos[TIME_DEFAULT_STR_POS][0], g_time_default_pos[TIME_DEFAULT_STR_POS][1], (u8 *)g_time_default_str);
        lcm12864_disp_string(g_time_default_pos[TIME_DEFAULT_POS][0], g_time_default_pos[TIME_DEFAULT_POS][1], disp_tmp_buf);
        lcm12864_disp_string(g_time_count_pos[TIME_COUNT_STR_POS][0], g_time_count_pos[TIME_COUNT_STR_POS][1], (u8 *)g_time_count_str);    
        lcm12864_disp_string(g_flow_pos[FLOW_STR_POS][0], g_flow_pos[FLOW_STR_POS][1], (u8 *)g_flow_str);
    }

    if(!(_sys_sec % FS400X_INSTANT_FLOW_READ_PERIOD))
    {
        fs400x_read_instant_flow();
    }
}

void on_time(void)
{
    u8 disp_tmp_buf[MAX_LCM_DISP_TMP_BUF_LEN];
    u8 disp_buf_len;

    
    g_time_count.second++;

    if(60 == g_time_count.second)
    {
        g_time_count.second = 0;
        g_time_count.minute++;

        if(60 == g_time_count.minute)
        {
            g_time_count.minute = 0;
            g_time_count.hour++;

            if(g_time_count.hour > MAX_TIME_VAL)
            {
                g_time_count.hour = 0;
            }
        }
    }

    if(g_time_default.hour == g_time_count.hour)
    {
        if(g_time_default.minute == g_time_count.minute)
        {
            _disble_rtc();
            g_time_count_complete = TRUE;
            lcm12864_disp_string(g_time_status_pos[TIME_STATUS_RUN_POS][0], g_time_status_pos[TIME_STATUS_RUN_POS][1], (u8 *)g_time_count_null_str);
            lcm12864_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);
            lcm12864_disp_string(g_time_status_pos[TIME_STATUS_COMPLETE_POS][0], g_time_status_pos[TIME_STATUS_COMPLETE_POS][1], (u8 *)g_time_count_complete_str);

            // -----------
            RELAY_N_OFF();
            RELAY_L_OFF();
            // -----------

            beep_on(); //鸣叫提示
        }
    }

    disp_buf_len = 8;
    sprintf((char *)disp_tmp_buf, "%02d:%02d:%02d", g_time_count.hour, g_time_count.minute, g_time_count.second);
    disp_tmp_buf[disp_buf_len] = '\0';

    lcm12864_disp_string(g_time_count_pos[TIME_COUNT_POS][0], g_time_count_pos[TIME_COUNT_POS][1], disp_tmp_buf);

    if(FALSE == g_time_count_complete)
    {
        lcm12864_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);
        lcm12864_disp_string(g_time_status_pos[TIME_STATUS_RUN_POS][0], g_time_status_pos[TIME_STATUS_RUN_POS][1], (u8 *)g_time_count_run_str);
    }
}

void on_key_press(void)
{
    g_key_param.last_msg = g_key_param.msg;
    g_key_param.count = DEFAULT_KEY_COUNT;
}

void on_uart_recv(void)
{
    u8 ch;


    while(get_bytes()) //串口一边接收数据，事件一边处理接收的数据
    {
        clr_wdt();
        
        ch = get_char();
        g_flow_param.rx.timeout = UART_TRANSMIT_TIMEOUT;
        
        g_com_param.buf[g_com_param.len++] = ch;

        if(g_com_param.len >= UART_RX_AF_THRESHOLD) //达到串口接收门限，转发串口数据
        {
            g_flow_param.rx.timeout = 0; //处理门限整数倍数据

#if (UART_DEBUG_EN > 0u)
            if(TRUE == g_flow_param.recv_pkt_overflow)
            {
                g_flow_param.recv_pkt_overflow = FALSE;
                DEBUG_PRINT(("<uart_recv_overflow> %s(%d) - %s\n", __FILE__, __LINE__, __FUNCTION__));
            }
#endif
          
            break;
        }
    }
}

void on_flow(void)
{
    u8 i, crc;
    u8 FRH, FRM, FRL;
    u8 disp_tmp_buf[MAX_LCM_DISP_TMP_BUF_LEN];
    u8 disp_buf_len;    
    u32 data;


    if((FRAME_HEAD != FS400X_FRAME_HEAD) && (FRAME_TAIL != FS400X_FRAME_TAIL))
    {
        return;
    }

    crc = FS400X_CMD;
    for(i=(FS400X_CMD_POS + 1); i<=(FS400X_DATA_LEN + (FS400X_CMD_POS + 1)); i++)
    {
        crc ^= g_com_param.buf[i];
    }

    if(FS400X_CRC == crc)
    {
        g_flow_param.rx.start_time = get_tick(); //接收到有效数据，记录当前时间
        
        switch(FS400X_CMD)
        {
            case READ_INSTANT_FLOW:
            {
                FRH = g_com_param.buf[3];
                FRM = g_com_param.buf[4];
                FRL = g_com_param.buf[5];
                
                data = FRH * 65536 + FRM * 256 + FRL;
                p_flow_ext_param->int_part = data / 1000; //整数部分
                p_flow_ext_param->val = data;
                p_flow_ext_param->dec_part = p_flow_ext_param->val - (p_flow_ext_param->int_part * 1000); //小数部分
                
                if(p_flow_ext_param->dec_part > 99)
                {
                    p_flow_ext_param->dec_part = p_flow_ext_param->dec_part / 100;
                }
                else if(p_flow_ext_param->dec_part > 9)
                {
                    p_flow_ext_param->dec_part = p_flow_ext_param->dec_part / 10;
                }
                
                if(FALSE == g_time_setting)
                {
                    disp_buf_len = 10;

                    if(p_flow_ext_param->int_part < 10)
                    {
                        sprintf((char *)disp_tmp_buf, "%d.%d SLPM  "/* 2 Space */, p_flow_ext_param->int_part, p_flow_ext_param->dec_part);
                    }
                    else
                    {
                        sprintf((char *)disp_tmp_buf, "%d.%dSLPM  "/* 2 Space */, p_flow_ext_param->int_part, p_flow_ext_param->dec_part);
                    }
                    
                    disp_tmp_buf[disp_buf_len] = '\0';
                    
                    lcm12864_disp_string(g_flow_pos[FLOW_POS][0], g_flow_pos[FLOW_POS][1], disp_tmp_buf);
                }
                
                break;
            }

            default:
                break;
        }
    }

    g_com_param.len = 0; //务必在此清零
}

