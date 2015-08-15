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
    u8 disp_tmp_buf[MAX_LCD_DISP_TMP_BUF_LEN];
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

                        if(((TRUE == g_time_set) || (TRUE == g_flow_set)) &&
                           (g_key_param.set_cnt > 4))
                        {
                            g_key_param.set_cnt = 0;
                            
                            g_param_setting = FALSE;

                            lcd_disp_normal();
                        }
                        else if((FALSE == g_time_set) &&
                                (FALSE == g_flow_set) &&
                                (g_key_param.set_cnt > 6))
                        {
                            g_key_param.set_cnt = 0;
                            
                            g_param_setting = FALSE;

                            lcd_disp_normal();
                        }                        
                        else
                        {
                            g_param_setting = TRUE;

                            lcd_disp_string(g_time_default_pos[TIME_DEFAULT_STR_POS][0], g_time_default_pos[TIME_DEFAULT_STR_POS][1], (u8 *)g_time_default_str);
                            lcd_disp_string(g_flow_set_pos[FLOW_STR_POS][0], g_flow_pos[FLOW_STR_POS][1], (u8 *)g_flow_str);
                            lcd_disp_string(g_cal_pos[CAL_STR_POS][0], g_cal_pos[CAL_STR_POS][1], (u8 *)g_cal_str);
                            lcd_disp_string(g_time_status_pos[TIME_STATUS_RUN_POS][0], g_time_status_pos[TIME_STATUS_RUN_POS][1], (u8 *)g_time_count_null_str);
                            lcd_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);
                            mdelay(10);
                            
                            if(g_key_param.set_cnt <= 2)
                            {
                                // -----------------------------------------------------
                                
                                disp_buf_len = 8;
                                sprintf((char *)disp_tmp_buf, "%02d时%02d分", g_time_default.hour, g_time_default.minute);
                                disp_tmp_buf[disp_buf_len] = '\0';
                                lcd_disp_string(g_time_default_pos[TIME_DEFAULT_POS][0], g_time_default_pos[TIME_DEFAULT_POS][1], disp_tmp_buf);
                                
                                // -----------------------------------------------------
                                
                                disp_buf_len = 9;

                                sprintf((char *)disp_tmp_buf, "%02d .%d   ", p_flow_ext_param->user_int_part, p_flow_ext_param->user_dec_part);

                                disp_tmp_buf[disp_buf_len] = '\0';
                                
                                lcd_disp_string(g_flow_set_pos[FLOW_POS][0], g_flow_set_pos[FLOW_POS][1], disp_tmp_buf);

                                // -----------------------------------------------------

                                p_flow_ext_param->cal_val = 0;
                                p_flow_ext_param->cal_int_part = 0;
                                p_flow_ext_param->cal_dec_part = 0;

                                disp_buf_len = 9;

                                sprintf((char *)disp_tmp_buf, "%02d .%02d  ", p_flow_ext_param->cal_int_part, p_flow_ext_param->cal_dec_part);

                                disp_tmp_buf[disp_buf_len] = '\0';
                                
                                lcd_disp_string(g_cal_pos[CAL_POS][0], g_cal_pos[CAL_POS][1], disp_tmp_buf);

                                // -----------------------------------------------------                                
                                
                                pos = g_key_param.set_cnt - 1;
                                
                                lcd_high_light(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1]);                                
                            }
                            else if(g_key_param.set_cnt <= 4)
                            {
                                pos = g_key_param.set_cnt - 2 - 1;
                                
                                lcd_high_light(g_flow_set_pos[pos][0], g_flow_set_pos[pos][1]);
                            }
                            else
                            {
                                pos = g_key_param.set_cnt - 4 - 1;
                                
                                lcd_high_light(g_cal_set_pos[pos][0], g_cal_set_pos[pos][1]);
                            }
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
                                lcd_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_pause_str);
                            }

                            // -------------------------------------------------------------

                            if(TRUE == g_flow_set)
                            {
                                g_flow_set = FALSE;
                                
                                p_flow_ext_param->user_val = p_flow_ext_param->user_int_part * 1000 + p_flow_ext_param->user_dec_part * 100;
                                p_flow_ext_param->inside_count = 0;
                            }

                            // -------------------------------------------------------------

                            if(TRUE == g_cal_set)
                            {
                                g_cal_set = FALSE;
                                
                                p_flow_ext_param->cal_val = p_flow_ext_param->cal_int_part * 1000 + p_flow_ext_param->cal_dec_part * 10;

                                if((0 != p_flow_ext_param->cal_val) && (49990 != p_flow_ext_param->cal_val))
                                {
                                    g_mem_param.cal_val = abs((int)p_flow_ext_param->avg_val - (int)p_flow_ext_param->cal_val);

                                    if(p_flow_ext_param->cal_val > p_flow_ext_param->avg_val)
                                    {
                                        g_mem_param.sign = POSITIVE_SIGN;
                                    }
                                    else
                                    {
                                        g_mem_param.sign = NEGATIVE_SIGN;
                                    }
                                    
                                    if(g_mem_param.cal_val < (1000 * 10))
                                    {
                                        mem_param_write();
                                    }
                                    else
                                    {
                                        mem_param_read();
                                    }
                                }
                                else if(49990 == p_flow_ext_param->cal_val) //重置校准
                                {
                                    g_mem_param.sign = NONE_SIGN;
                                    g_mem_param.cal_val = 0;

                                    mem_param_write();
                                }
                            }
                            
                            // -------------------------------------------------------------

                            disp_buf_len = 8;
                            sprintf((char *)disp_tmp_buf, "%02d:%02d:%02d", g_time_count.hour, g_time_count.minute, g_time_count.second);
                            disp_tmp_buf[disp_buf_len] = '\0';
                            
                            lcd_disp_string(g_time_count_pos[TIME_COUNT_POS][0], g_time_count_pos[TIME_COUNT_POS][1], disp_tmp_buf);

                            // -------------------------------------------------------------
                            
                            p_flow_ext_param->disp_int_part = p_flow_ext_param->disp_val / 1000; //整数部分
                            p_flow_ext_param->disp_dec_part = (p_flow_ext_param->disp_val - (p_flow_ext_param->disp_int_part * 1000)) / 100; //小数部分
                            
                            disp_buf_len = 10;
                            
                            if(p_flow_ext_param->disp_int_part < 10)
                            {
                                sprintf((char *)disp_tmp_buf, "%d.%d SLPM  "/* 2 Space */, p_flow_ext_param->disp_int_part, p_flow_ext_param->disp_dec_part);
                            }
                            else
                            {
                                sprintf((char *)disp_tmp_buf, "%d.%dSLPM  "/* 2 Space */, p_flow_ext_param->disp_int_part, p_flow_ext_param->disp_dec_part);
                            }
                            
                            disp_tmp_buf[disp_buf_len] = '\0';
                            
                            lcd_disp_string(g_flow_pos[FLOW_POS][0], g_flow_pos[FLOW_POS][1], disp_tmp_buf);   
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
                        lcd_disp_string(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1], disp_tmp_buf);
                        lcd_cursor_left();
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
                        lcd_disp_string(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1], disp_tmp_buf);
                        lcd_cursor_left();
                        g_time_set = TRUE;
                        break;

                    case FLOW_DEC_SET:
                        p_flow_ext_param->user_dec_part++;
                        
                        if(p_flow_ext_param->user_dec_part > MAX_FLOW_DEC_PART_VAL)
                        {
                            p_flow_ext_param->user_dec_part = 0;
                        }
                        
                        disp_buf_len = 2;
                        
                        sprintf((char *)disp_tmp_buf, "%d ", p_flow_ext_param->user_dec_part);

                        disp_tmp_buf[disp_buf_len] = '\0';

                        pos = g_key_param.set_cnt - 2 - 1;
                        
                        lcd_disp_string(g_flow_set_pos[pos][0], g_flow_set_pos[pos][1], disp_tmp_buf);

                        lcd_cursor_left();

                        g_flow_set = TRUE;
                        break;

                    case FLOW_INT_SET:
                        p_flow_ext_param->user_int_part++;
                        
                        if(p_flow_ext_param->user_int_part > MAX_FLOW_INT_PART_VAL)
                        {
                            p_flow_ext_param->user_int_part = 0;
                        }
                        
                        disp_buf_len = 2;
                        
                        sprintf((char *)disp_tmp_buf, "%02d", p_flow_ext_param->user_int_part);

                        disp_tmp_buf[disp_buf_len] = '\0';

                        pos = g_key_param.set_cnt - 2 - 1;
                        
                        lcd_disp_string(g_flow_set_pos[pos][0], g_flow_set_pos[pos][1], disp_tmp_buf);

                        lcd_cursor_left();

                        g_flow_set = TRUE;
                        break;

                    case CAL_DEC_SET:
                        p_flow_ext_param->cal_dec_part++;
                        
                        if(p_flow_ext_param->cal_dec_part > MAX_CAL_DEC_PART_VAL)
                        {
                            p_flow_ext_param->cal_dec_part = 0;
                        }
                        
                        disp_buf_len = 2;
                        
                        sprintf((char *)disp_tmp_buf, "%02d", p_flow_ext_param->cal_dec_part);

                        disp_tmp_buf[disp_buf_len] = '\0';

                        pos = g_key_param.set_cnt - 4 - 1;
                        
                        lcd_disp_string(g_cal_set_pos[pos][0], g_cal_set_pos[pos][1], disp_tmp_buf);

                        lcd_cursor_left();

                        g_cal_set = TRUE;
                        break;

                    case CAL_INT_SET:
                        p_flow_ext_param->cal_int_part++;
                        
                        if(p_flow_ext_param->cal_int_part > MAX_FLOW_INT_PART_VAL)
                        {
                            p_flow_ext_param->cal_int_part = 0;
                        }
                        
                        disp_buf_len = 2;
                        
                        sprintf((char *)disp_tmp_buf, "%02d", p_flow_ext_param->cal_int_part);

                        disp_tmp_buf[disp_buf_len] = '\0';

                        pos = g_key_param.set_cnt - 4 - 1;
                        
                        lcd_disp_string(g_cal_set_pos[pos][0], g_cal_set_pos[pos][1], disp_tmp_buf);

                        lcd_cursor_left();

                        g_cal_set = TRUE;
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
                        lcd_disp_string(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1], disp_tmp_buf);
                        lcd_cursor_left();
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
                        lcd_disp_string(g_time_default_set_pos[pos][0], g_time_default_set_pos[pos][1], disp_tmp_buf);
                        lcd_cursor_left();
                        g_time_set = TRUE;
                        break;

                    case FLOW_DEC_SET:
                        p_flow_ext_param->user_dec_part--;
                        
                        if(p_flow_ext_param->user_dec_part < 0)
                        {
                            p_flow_ext_param->user_dec_part = MAX_FLOW_DEC_PART_VAL;
                        }
                        
                        disp_buf_len = 2;
                        
                        sprintf((char *)disp_tmp_buf, "%d ", p_flow_ext_param->user_dec_part);

                        disp_tmp_buf[disp_buf_len] = '\0';

                        pos = g_key_param.set_cnt - 2 - 1;
                        
                        lcd_disp_string(g_flow_set_pos[pos][0], g_flow_set_pos[pos][1], disp_tmp_buf);

                        lcd_cursor_left();

                        g_flow_set = TRUE;
                        break;

                    case FLOW_INT_SET:
                        p_flow_ext_param->user_int_part--;
                        
                        if(p_flow_ext_param->user_int_part < 0)
                        {
                            p_flow_ext_param->user_int_part = MAX_FLOW_INT_PART_VAL;
                        }
                        
                        disp_buf_len = 2;
                        
                        sprintf((char *)disp_tmp_buf, "%02d", p_flow_ext_param->user_int_part);

                        disp_tmp_buf[disp_buf_len] = '\0';

                        pos = g_key_param.set_cnt - 2 - 1;
                        
                        lcd_disp_string(g_flow_set_pos[pos][0], g_flow_set_pos[pos][1], disp_tmp_buf);

                        lcd_cursor_left();

                        g_flow_set = TRUE;
                        break;

                    case CAL_DEC_SET:
                        p_flow_ext_param->cal_dec_part--;
                        
                        if(p_flow_ext_param->cal_dec_part < 0)
                        {
                            p_flow_ext_param->cal_dec_part = MAX_CAL_DEC_PART_VAL;
                        }
                        
                        disp_buf_len = 2;
                        
                        sprintf((char *)disp_tmp_buf, "%02d", p_flow_ext_param->cal_dec_part);

                        disp_tmp_buf[disp_buf_len] = '\0';

                        pos = g_key_param.set_cnt - 4 - 1;
                        
                        lcd_disp_string(g_cal_set_pos[pos][0], g_cal_set_pos[pos][1], disp_tmp_buf);

                        lcd_cursor_left();

                        g_cal_set = TRUE;
                        break;

                    case CAL_INT_SET:
                        p_flow_ext_param->cal_int_part--;
                        
                        if(p_flow_ext_param->cal_int_part < 0)
                        {
                            p_flow_ext_param->cal_int_part = MAX_CAL_INT_PART_VAL;
                        }
                        
                        disp_buf_len = 2;
                        
                        sprintf((char *)disp_tmp_buf, "%02d", p_flow_ext_param->cal_int_part);

                        disp_tmp_buf[disp_buf_len] = '\0';

                        pos = g_key_param.set_cnt - 4 - 1;
                        
                        lcd_disp_string(g_cal_set_pos[pos][0], g_cal_set_pos[pos][1], disp_tmp_buf);

                        lcd_cursor_left();

                        g_cal_set = TRUE;
                        break;

                    default:
                        break;
                    }
                }
                break;

            case KEY_MSG_PAUSE:
                if(Bit_RESET == GPIO_ReadInputDataBit(KEY_PAUSE_PORT, KEY_PAUSE_PIN))
                {
                    if((FALSE == g_time_count_complete) && (FALSE == g_param_setting))
                    {
                        g_key_param.pause_cnt++;
                        
                        if(!(g_key_param.pause_cnt % 2) || (TRUE == g_time_count_pause))
                        {
                            _enable_rtc();

                            g_key_param.pause_cnt = 0;

                            g_time_count_pause = FALSE;

                            lcd_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);    

                            // -----------
                            RELAY_N_ON();
                            RELAY_L_ON();
                            // ----------- 
                        }
                        else
                        {
                            _disble_rtc();

                            g_time_count_pause = TRUE;

                            lcd_disp_string(g_time_status_pos[TIME_STATUS_RUN_POS][0], g_time_status_pos[TIME_STATUS_RUN_POS][1], (u8 *)g_time_count_null_str);
                            lcd_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_pause_str);

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
                        
                        lcd_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);
                        lcd_disp_string(g_time_status_pos[TIME_STATUS_COMPLETE_POS][0], g_time_status_pos[TIME_STATUS_COMPLETE_POS][1], (u8 *)g_time_count_null_str);

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
                       (FALSE == g_param_setting))
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

    if(check_timeout(g_flow_param.rx.start_time, FS400X_RECV_TIMEOUT) && (FALSE == g_param_setting)) //处理FS400X数据接收超时
    {
        lcd_disp_string(g_flow_pos[FLOW_POS][0], g_flow_pos[FLOW_POS][1], "0.0 SLPM  "/* 2 Space */);

        g_flow_param.rx.start_time = get_tick();
    }

    if(!(_sys_tick % FS400X_INSTANT_FLOW_READ_PERIOD))
    {
        fs400x_read_instant_flow();
    }

    // -----
    beep();
    // -----
}

void on_second(void)
{
    u8 disp_tmp_buf[MAX_LCD_DISP_TMP_BUF_LEN];
    u8 disp_buf_len;

    
    _sys_sec++;

    if((!(_sys_sec % LCD_REFRESH_PERIOD)) && (FALSE == g_param_setting))
    {
        disp_buf_len = 8;
        sprintf((char *)disp_tmp_buf, "%02d时%02d分", g_time_default.hour, g_time_default.minute);
        disp_tmp_buf[disp_buf_len] = '\0';
 
        lcd_disp_string(g_time_default_pos[TIME_DEFAULT_STR_POS][0], g_time_default_pos[TIME_DEFAULT_STR_POS][1], (u8 *)g_time_default_str);
        lcd_disp_string(g_time_default_pos[TIME_DEFAULT_POS][0], g_time_default_pos[TIME_DEFAULT_POS][1], disp_tmp_buf);
        lcd_disp_string(g_time_count_pos[TIME_COUNT_STR_POS][0], g_time_count_pos[TIME_COUNT_STR_POS][1], (u8 *)g_time_count_str);    
        lcd_disp_string(g_flow_pos[FLOW_STR_POS][0], g_flow_pos[FLOW_STR_POS][1], (u8 *)g_flow_str);
    }
}

void on_time(void)
{
    u8 disp_tmp_buf[MAX_LCD_DISP_TMP_BUF_LEN];
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
            lcd_disp_string(g_time_status_pos[TIME_STATUS_RUN_POS][0], g_time_status_pos[TIME_STATUS_RUN_POS][1], (u8 *)g_time_count_null_str);
            lcd_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);
            lcd_disp_string(g_time_status_pos[TIME_STATUS_COMPLETE_POS][0], g_time_status_pos[TIME_STATUS_COMPLETE_POS][1], (u8 *)g_time_count_complete_str);

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

    lcd_disp_string(g_time_count_pos[TIME_COUNT_POS][0], g_time_count_pos[TIME_COUNT_POS][1], disp_tmp_buf);

    if(FALSE == g_time_count_complete)
    {
        lcd_disp_string(g_time_status_pos[TIME_STATUS_PAUSE_POS][0], g_time_status_pos[TIME_STATUS_PAUSE_POS][1], (u8 *)g_time_count_null_str);
        lcd_disp_string(g_time_status_pos[TIME_STATUS_RUN_POS][0], g_time_status_pos[TIME_STATUS_RUN_POS][1], (u8 *)g_time_count_run_str);
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
    u8 disp_tmp_buf[MAX_LCD_DISP_TMP_BUF_LEN];
    u8 disp_buf_len;    


    if((FRAME_HEAD != FS400X_FRAME_HEAD) && (FRAME_TAIL != FS400X_FRAME_TAIL))
    {
        return;
    }

    crc = FS400X_CMD;
    for(i = (FS400X_CMD_POS + 1); i <= (FS400X_DATA_LEN + (FS400X_CMD_POS + 1)); i++)
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
                
                p_flow_ext_param->inst_val = (u32)FRH * 65536 + (u32)FRM * 256 + (u32)FRL;

                p_flow_ext_param->buf[p_flow_ext_param->index] = p_flow_ext_param->inst_val;
                p_flow_ext_param->index++;

                if(MAX_FLOW_NUM == p_flow_ext_param->index)
                { 
                    p_flow_ext_param->sum = 0;
                        
                    for(i = 0; i < MAX_FLOW_NUM; i++)
                    {
                        p_flow_ext_param->sum += p_flow_ext_param->buf[i];
                    }
                    
                    p_flow_ext_param->avg_val = p_flow_ext_param->sum / MAX_FLOW_NUM; //取平均值 

                    p_flow_ext_param->sum = 0;
                    
                    for(i = 0; i < MAX_FLOW_NUM; i++)
                    {
                        p_flow_ext_param->sum += pow(abs((int)p_flow_ext_param->buf[i] - (int)p_flow_ext_param->avg_val), 2);
                    }

                    p_flow_ext_param->discrete = sqrt(p_flow_ext_param->sum) * 100;

                    p_flow_ext_param->index = 0;

                    if(p_flow_ext_param->discrete < (100000 * 3))
                    {
                        /* 校准显示 */
                        if((p_flow_ext_param->avg_val > (1000 * 10)) && (POSITIVE_SIGN == g_mem_param.sign))
                        {
                            p_flow_ext_param->disp_val = p_flow_ext_param->avg_val + g_mem_param.cal_val;
                        }
                        else if((p_flow_ext_param->avg_val > (1000 * 10)) && (NEGATIVE_SIGN == g_mem_param.sign))
                        {
                            p_flow_ext_param->disp_val = p_flow_ext_param->avg_val - g_mem_param.cal_val;
                        }
                        else
                        {
                            p_flow_ext_param->disp_val = p_flow_ext_param->avg_val;
                        }
                        
                        /* 如果测量值接近用户输入值，则显示用户输入值 */
                        if((0 != p_flow_ext_param->disp_val) && (abs((int)p_flow_ext_param->disp_val - (int)p_flow_ext_param->user_val) < 300))
                        {
                            p_flow_ext_param->inside_count++;
                        
                            if(p_flow_ext_param->inside_count >= 2)
                            {
                                p_flow_ext_param->inside_count = 0;
                                
                                if(FALSE == g_param_setting)
                                {
                                    //p_flow_ext_param->user_val += 50; //四舍五入
                                    
                                    p_flow_ext_param->user_int_part = p_flow_ext_param->user_val / 1000; //整数部分
                                    p_flow_ext_param->user_dec_part = (p_flow_ext_param->user_val - (p_flow_ext_param->user_int_part * 1000)) / 100; //小数部分
                                    
                                    disp_buf_len = 10;
                                
                                    if(p_flow_ext_param->user_int_part < 10)
                                    {
                                        sprintf((char *)disp_tmp_buf, "%d.%d SLPM  "/* 2 Space */, p_flow_ext_param->user_int_part, p_flow_ext_param->user_dec_part);
                                    }
                                    else
                                    {
                                        sprintf((char *)disp_tmp_buf, "%d.%dSLPM  "/* 2 Space */, p_flow_ext_param->user_int_part, p_flow_ext_param->user_dec_part);
                                    }
                                    
                                    disp_tmp_buf[disp_buf_len] = '\0';
                                    
                                    lcd_disp_string(g_flow_pos[FLOW_POS][0], g_flow_pos[FLOW_POS][1], disp_tmp_buf);
                                }  
                            }

                            break;
                        }
                        else
                        {
                            p_flow_ext_param->inside_count = 0;
                        }
                    
                        if(FALSE == g_param_setting)
                        {                            
                            p_flow_ext_param->disp_val += 50; //四舍五入
                                
                            p_flow_ext_param->disp_int_part = p_flow_ext_param->disp_val / 1000; //整数部分
                            p_flow_ext_param->disp_dec_part = (p_flow_ext_param->disp_val - (p_flow_ext_param->disp_int_part * 1000)) / 100; //小数部分
                        
                            disp_buf_len = 10;
                        
                            if(p_flow_ext_param->disp_int_part < 10)
                            {
                                sprintf((char *)disp_tmp_buf, "%d.%d SLPM  "/* 2 Space */, p_flow_ext_param->disp_int_part, p_flow_ext_param->disp_dec_part);
                            }
                            else
                            {
                                sprintf((char *)disp_tmp_buf, "%d.%dSLPM  "/* 2 Space */, p_flow_ext_param->disp_int_part, p_flow_ext_param->disp_dec_part);
                            }
                            
                            disp_tmp_buf[disp_buf_len] = '\0';
                            
                            lcd_disp_string(g_flow_pos[FLOW_POS][0], g_flow_pos[FLOW_POS][1], disp_tmp_buf);
                        }
                    }                                       
                }
                break;
            }

            default:
                break;
        }
    }

    g_com_param.len = 0; //务必在此清零
}

