#include "includes.h"


static int form_main(unsigned int key_msg, unsigned int form_msg);
static int form_info(unsigned int key_msg, unsigned int form_msg);
static int form_set(unsigned int key_msg, unsigned int form_msg);

static const FORM form_list[MAX_FORM_NUM] =
{
    {form_main},
    
    {form_info},
    
    {form_set},
};

static unsigned int form_id;

static INT32U lcd_backlight_disp_time;

static const char *week_str[] =
{
    "星期日",
    "星期一",
    "星期二",
    "星期三",
    "星期四",
    "星期五",
    "星期六",
};

static const char *time_count_ctrl_str[] =
{
    "      暂停      ",
    "运行            ",
    "            完毕",
    "                ",
    "运行  暂停  完毕"
};

void MENU_Init(void)
{
    form_id = FORM_ID_MAIN;

    lcd_backlight_disp_time = 0;
    
    form_main(KEY_MSG_NONE, FORM_MSG_DATA);
}

void menu_refresh(void)
{
    if(lcd_backlight_disp_time)
    {
        lcd_backlight_disp_time--;
    
        if(0 == lcd_backlight_disp_time)
        {
            LCD_Off();
        }
    }
    
    if((FORM_ID_SET != form_id) && (NULL != form_list[form_id]))
    {
        (*form_list[form_id])(KEY_MSG_NONE, FORM_MSG_REFRESH);
    }
}

void menu_key_proc(int key_msg)
{
    int form_msg;
    

    lcd_backlight_disp_time = LCD_BACKLIGHT_DISP_TIMEOUT;
    LCD_On();

    if(NULL != form_list[form_id])
    {
        if(FORM_MSG_NONE != (form_msg = (*form_list[form_id])(key_msg, FORM_MSG_KEY)))
        {
            (*form_list[form_id])(key_msg, form_msg);
        }
    }
}

void menu_clear(void)
{
    char disp_buf[18];

    
    memset(disp_buf, ' ', 16);
    
    disp_buf[17] = '\0';
    
    GUI_DispStringAt(disp_buf, 0, 0); 
    GUI_DispStringAt(disp_buf, 0, 16); 
    GUI_DispStringAt(disp_buf, 0, 32); 
    GUI_DispStringAt(disp_buf, 0, 48); 
}

void form_main_refresh(void)
{
    char disp_buf[18];

    
    sprintf(disp_buf, "计时:  %03d:%02d:%02d", g_time_count.hour, g_time_count.minute, g_time_count.second);
    GUI_DispStringAt(disp_buf, 0, 16);
    
    sprintf(disp_buf, "流量:  %02d.%d SLPM", g_mems_para.flow / 1000, g_mems_para.flow / 100);
    GUI_DispStringAt(disp_buf, 0, 32);    

    sprintf(disp_buf, "%s", time_count_ctrl_str[g_time_count_ctrl]);
    GUI_DispStringAt(disp_buf, 0, 48);
}

void form_info_refresh(void)
{
    char disp_buf[18];

    
    sprintf(disp_buf, "        %02x:%02x:%02x", g_rtc_time[HOUR], g_rtc_time[MIN], g_rtc_time[SEC]);
    GUI_DispStringAt(disp_buf, 0, 0);

    sprintf(disp_buf, "%s", week_str[g_rtc_time[WEEK] % MAX_WEEK_ITEM]);
    GUI_DispStringAt(disp_buf, 0, 0);

    sprintf(disp_buf, "   20%02x-%02x-%02x   ", g_rtc_time[YEAR], g_rtc_time[MONTH], g_rtc_time[DATE]);
    GUI_DispStringAt(disp_buf, 0, 48);    
}

static int form_main(unsigned int key_msg, unsigned int form_msg)
{
    char disp_buf[18];
    
    
    if(FORM_MSG_REFRESH == form_msg)
    {   
        form_main_refresh();
        
        return (FORM_MSG_NONE);
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {
        case KEY_MSG_CONFIRM:
            form_id = (form_id + 1) % (MAX_FORM_NUM - 1);
            return (FORM_MSG_DATA);
            break;

        case KEY_MSG_SET:
            form_id = FORM_ID_SET;
            return (FORM_MSG_DATA);
            break;

        default:
            break;
        }
    }
    else if(FORM_MSG_DATA == form_msg)
    {

    }
    else
    {
        return (FORM_MSG_NONE);
    }

    sprintf(disp_buf, "定时:  %03d时%02d分", g_alarm_time.hour, g_alarm_time.minute);
    GUI_DispStringAt(disp_buf, 0, 0);

    sprintf(disp_buf, "计时:  %03d:%02d:%02d", g_time_count.hour, g_time_count.minute, g_time_count.second);
    GUI_DispStringAt(disp_buf, 0, 16);
    
    sprintf(disp_buf, "流量:  %02d.%d SLPM", g_mems_para.flow / 1000, g_mems_para.flow / 100);
    GUI_DispStringAt(disp_buf, 0, 32);    
    
    sprintf(disp_buf, "%s", time_count_ctrl_str[g_time_count_ctrl]);
    GUI_DispStringAt(disp_buf, 0, 48);       

    return (FORM_MSG_NONE);
}

static int form_info(unsigned int key_msg, unsigned int form_msg)
{
    char disp_buf[18];
    
    
    if(FORM_MSG_REFRESH == form_msg)
    {        
        form_info_refresh();

        return (FORM_MSG_NONE);
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {
        case KEY_MSG_CONFIRM:
            form_id = (form_id + 1) % (MAX_FORM_NUM - 1);
            return (FORM_MSG_DATA);
            break;

        case KEY_MSG_SET:
            form_id = FORM_ID_SET;
            return (FORM_MSG_DATA);            
            break;

        default:
            break;
        }
    }
    else if(FORM_MSG_DATA == form_msg)
    {

    }
    else
    {
        return (FORM_MSG_NONE);
    }    

    sprintf(disp_buf, "        %02x:%02x:%02x", g_rtc_time[HOUR], g_rtc_time[MIN], g_rtc_time[SEC]);
    GUI_DispStringAt(disp_buf, 0, 0);
    
    sprintf(disp_buf, "%s", week_str[g_rtc_time[WEEK] % MAX_WEEK_ITEM]);
    GUI_DispStringAt(disp_buf, 0, 0);

    sprintf(disp_buf, "软件:%02x%02x%02x%02x-%02x",  
                      SOFTWARE_VERSION[0],
                      SOFTWARE_VERSION[1],
                      SOFTWARE_VERSION[2],
                      SOFTWARE_VERSION[3],
                      SOFTWARE_VERSION[4]);
    GUI_DispStringAt(disp_buf, 0, 16); 

    sprintf(disp_buf, "硬件:%02x%02x%02x%02x-%02x",  
                      HARDWARE_VERSION[0],
                      HARDWARE_VERSION[1],
                      HARDWARE_VERSION[2],
                      HARDWARE_VERSION[3],
                      HARDWARE_VERSION[4]);
    GUI_DispStringAt(disp_buf, 0, 32);    

    sprintf(disp_buf, "   20%02x-%02x-%02x   ", g_rtc_time[YEAR], g_rtc_time[MONTH], g_rtc_time[DATE]);
    GUI_DispStringAt(disp_buf, 0, 48);

    return (FORM_MSG_NONE);
}

static int form_set(unsigned int key_msg, unsigned int form_msg)
{
    static MENU_SET menu_set;
    static ALARM_TIME_SET alarm_time_set;
    static ALARM_TIME g_alarm_time_set;
    static TARGET_FLOW_SET target_flow_set;
    static FLOW_PARA g_target_flow_set;
    static RTC_TIME_SET rtc_time_set;
    static INT8U g_rtc_time_set[MAX_RTC_TIME_ITEM];
    INT8U temp;
    char disp_buf[18];

    
    menu_clear();

    if(FORM_MSG_REFRESH == form_msg)
    {        
        return (FORM_MSG_NONE);
    }
    else if(FORM_MSG_KEY == form_msg)
    {
        switch(key_msg)
        {
        case KEY_MSG_SET:
            if(MENU_SET_ALARM_TIME == menu_set)
            {
                alarm_time_set = (alarm_time_set + 1) % ALARM_TIME_SET_MAX;
            }
            else if(MENU_SET_TARGET_FLOW == menu_set)
            {
                target_flow_set = (target_flow_set + 1) % TARGET_FLOW_SET_MAX;
            }
            else if(MENU_SET_RTC_TIME == menu_set)
            {
                rtc_time_set = (rtc_time_set + 1) % RTC_TIME_SET_MAX;
            }
            break;

        case KEY_MSG_DOWN:
            if(MENU_SET_ALARM_TIME == menu_set)
            {
                switch(alarm_time_set)
                {
                case ALARM_TIME_SET_HOUR:
                    if(g_alarm_time_set.hour)
                    {
                        g_alarm_time_set.hour--;
                    }
                    else
                    {
                        g_alarm_time_set.hour = MAX_TIME_COUNT;
                    }
                    break;

                case ALARM_TIME_SET_MINUTE:
                    if(g_alarm_time_set.minute)
                    {
                        g_alarm_time_set.minute--;
                    }
                    else
                    {
                        g_alarm_time_set.minute = 59;
                    }
                    break;  

                default:
                    break;
                }
            }
            else if(MENU_SET_TARGET_FLOW == menu_set)
            {
                switch(target_flow_set)
                {
                case TARGET_FLOW_SET_INT_PART:
                    if(g_target_flow_set.int_part)
                    {
                        g_target_flow_set.int_part--;
                    }
                    else
                    {
                        g_target_flow_set.int_part = MAX_MEMS_FLOW - 1;
                    }
                    break;

                case TARGET_FLOW_SET_DEC_PART:
                    if(g_target_flow_set.dec_part)
                    {
                        g_target_flow_set.dec_part--;
                    }
                    else
                    {
                        g_target_flow_set.dec_part = 9;
                    }                   
                    break;

                default:
                    break;
                }
            }
            else if(MENU_SET_RTC_TIME == menu_set)
            {
                switch(rtc_time_set)
                {
                case RTC_TIME_SET_YEAR:
                    temp = Bcd2Hex(g_rtc_time_set[YEAR]) + 99;
                    temp %= 100;
                    g_rtc_time_set[YEAR] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_MONTH:
                    temp = Bcd2Hex(g_rtc_time_set[MONTH]);

                    if(temp > 1)
                    {
                        temp--;
                    }
                    else
                    {
                        temp = 12;
                    }

                    g_rtc_time_set[MONTH] = Hex2Bcd(temp);
                    break;

                case RTC_TIME_SET_DATE:
                    temp = Bcd2Hex(g_rtc_time_set[DATE]);
                    
                    if(temp > 1)
                    {
                        temp--;
                    }
                    else
                    {
                        temp = get_days_of_month(2000 + Bcd2Hex(g_rtc_time_set[YEAR]), Bcd2Hex(g_rtc_time_set[MONTH])); 
                    }
                    
                    g_rtc_time_set[DATE] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_HOUR:
                    temp = Bcd2Hex(g_rtc_time_set[HOUR]) + 23;
                    temp %= 24;  
                    g_rtc_time_set[HOUR] = Hex2Bcd(temp);                    
                    break;       

                case RTC_TIME_SET_MINUTE:
                    temp = Bcd2Hex(g_rtc_time_set[MIN]) + 59;
                    temp %= 60;  
                    g_rtc_time_set[MIN] = Hex2Bcd(temp);                    
                    break;    

                case RTC_TIME_SET_SECOND:
                    temp = Bcd2Hex(g_rtc_time_set[SEC]) + 59;
                    temp %= 60;  
                    g_rtc_time_set[SEC] = Hex2Bcd(temp);                    
                    break;

                default:
                    break;
                }
            }
            break;

        case KEY_MSG_UP:
            if(MENU_SET_ALARM_TIME == menu_set)
            {
                switch(alarm_time_set)
                {
                case ALARM_TIME_SET_HOUR:
                    g_alarm_time_set.hour++;
                    
                    if(g_alarm_time_set.hour > MAX_TIME_COUNT)
                    {
                        g_alarm_time_set.hour = 0;
                    }
                    break;

                case ALARM_TIME_SET_MINUTE:
                    g_alarm_time_set.minute++;
                    
                    if(g_alarm_time_set.minute > 59)
                    {
                        g_alarm_time_set.minute = 0;
                    }
                    break;  

                default:
                    break;
                }
            } 
            else if(MENU_SET_TARGET_FLOW == menu_set)
            {
                switch(target_flow_set)
                {
                case TARGET_FLOW_SET_INT_PART:
                    g_target_flow_set.int_part++;

                    if(g_target_flow_set.int_part > (MAX_MEMS_FLOW - 1))
                    {
                        g_target_flow_set.int_part = 0;
                    }
                    break;

                case TARGET_FLOW_SET_DEC_PART:
                    g_target_flow_set.dec_part++;

                    if(g_target_flow_set.dec_part > 9)
                    {
                        g_target_flow_set.dec_part = 0;
                    }                    
                    break;

                default:
                    break;
                }
            }
            else if(MENU_SET_RTC_TIME == menu_set)
            {
                switch(rtc_time_set)
                {
                case RTC_TIME_SET_YEAR:   
                    temp = Bcd2Hex(g_rtc_time_set[YEAR]) + 1;
                    temp %= 100;
                    g_rtc_time_set[YEAR] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_MONTH:
                    temp = Bcd2Hex(g_rtc_time_set[MONTH]) + 1;
                    
                    if(temp > 12)
                    {
                        temp = 1;
                    }
                    
                    g_rtc_time_set[MONTH] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_DATE:  
                    temp = Bcd2Hex(g_rtc_time_set[DATE]) + 1;
                    
                    if(temp > get_days_of_month(2000 + Bcd2Hex(g_rtc_time_set[YEAR]), Bcd2Hex(g_rtc_time_set[MONTH])))
                    {
                       temp = 1;                    
                    }
                    
                    g_rtc_time_set[DATE] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_HOUR:    
                    temp = Bcd2Hex(g_rtc_time_set[HOUR]) + 1;
                    temp %= 24;  
                    g_rtc_time_set[HOUR] = Hex2Bcd(temp);                    
                    break;       

                case RTC_TIME_SET_MINUTE:  
                    temp = Bcd2Hex(g_rtc_time_set[MIN]) + 1;
                    temp %= 60;  
                    g_rtc_time_set[MIN] = Hex2Bcd(temp);                    
                    break;    

                case RTC_TIME_SET_SECOND:    
                    temp = Bcd2Hex(g_rtc_time_set[SEC]) + 1;
                    temp %= 60;  
                    g_rtc_time_set[SEC] = Hex2Bcd(temp);                    
                    break;

                default:
                    break;
                }
            }            
            break;
            
        case KEY_MSG_CONFIRM:
            if(MENU_SET_ALARM_TIME == menu_set)
            {
                switch(alarm_time_set)
                {
                case ALARM_TIME_SET_CONFIRM:
                    memcpy(&g_alarm_time, &g_alarm_time_set, sizeof(ALARM_TIME));
                    break;

                case ALARM_TIME_SET_CANCEL:
                    form_id = FORM_ID_MAIN;
                    
                    return (FORM_MSG_DATA);                    
                    break;

                default:
                    break;
                }

                menu_set = MENU_SET_TARGET_FLOW;

                target_flow_set = TARGET_FLOW_SET_INT_PART;

                g_target_flow_set.int_part = g_mems_para.target / 1000;
                g_target_flow_set.dec_part = g_mems_para.target / 100;
            }
            else if(MENU_SET_TARGET_FLOW == menu_set)
            {
                switch(target_flow_set)
                {
                case TARGET_FLOW_SET_CONFIRM:
                    g_mems_para.target = g_target_flow_set.int_part * 1000 + g_target_flow_set.dec_part * 100;
                    break;

                case TARGET_FLOW_SET_CANCEL:
                    form_id = FORM_ID_MAIN;
                    
                    return (FORM_MSG_DATA);                    
                    break;

                default:
                    break;
                }
                
                menu_set = MENU_SET_RTC_TIME;

                RTC_ReadTime(g_rtc_time_set);

                rtc_time_set = RTC_TIME_SET_YEAR;
            }
            else if(MENU_SET_RTC_TIME == menu_set)
            {
                switch(rtc_time_set)
                {
                case RTC_TIME_SET_SAVE:
                    g_rtc_time_set[WEEK] = Hex2Bcd(get_week(2000 + Bcd2Hex(g_rtc_time_set[YEAR]), Bcd2Hex(g_rtc_time_set[MONTH]), Bcd2Hex(g_rtc_time_set[DATE])));
                    RTC_WriteTime(g_rtc_time_set);
                    break;

                case RTC_TIME_SET_CANCEL:
                    form_id = FORM_ID_MAIN;
                    
                    return (FORM_MSG_DATA);                    
                    break;

                default:
                    break;
                }
                
                menu_set = MENU_SET_ALARM_TIME;
                
                memcpy(&g_alarm_time_set, &g_alarm_time, sizeof(ALARM_TIME));
                
                alarm_time_set = ALARM_TIME_SET_HOUR;
            }
            break;
            
        default:
            break;
        }
    }
    else if(FORM_MSG_DATA == form_msg)
    {
        menu_set = MENU_SET_ALARM_TIME;
        memcpy(&g_alarm_time_set, &g_alarm_time, sizeof(ALARM_TIME));
        alarm_time_set = ALARM_TIME_SET_HOUR;
    }
    else
    {
        return (FORM_MSG_NONE);
    }

    switch(menu_set)
    {
    case MENU_SET_ALARM_TIME:
        sprintf(disp_buf, "定时时间:       ");
        GUI_DispStringAt(disp_buf, 0, 16);    
        sprintf(disp_buf, "    %02d时%02d分    ", g_alarm_time_set.hour, g_alarm_time_set.minute);
        GUI_DispStringAt(disp_buf, 0, 32);    
        sprintf(disp_buf, "   确认  取消   ");
        GUI_DispStringAt(disp_buf, 0, 48);

        switch(alarm_time_set)
        {
        case ALARM_TIME_SET_HOUR:
            sprintf(disp_buf, "%02d", g_alarm_time_set.hour);
            GUI_DispRevStringAt(disp_buf, 32, 32);            
            break;
            
        case ALARM_TIME_SET_MINUTE:
            sprintf(disp_buf, "%02d", g_alarm_time_set.minute);
            GUI_DispRevStringAt(disp_buf, 64, 32);            
            break;

        case ALARM_TIME_SET_CONFIRM:
            sprintf(disp_buf, "确认"); 
            GUI_DispRevStringAt(disp_buf, 24, 48);            
            break;

        case ALARM_TIME_SET_CANCEL:
            sprintf(disp_buf, "取消"); 
            GUI_DispRevStringAt(disp_buf, 72, 48);            
            break;

        default:
            break;
        }
        break;

    case MENU_SET_TARGET_FLOW:
        sprintf(disp_buf, "目标流量:       ");
        GUI_DispStringAt(disp_buf, 0, 16);    
        sprintf(disp_buf, "   %02d.%02d SLPM   ", g_target_flow_set.int_part, g_target_flow_set.dec_part * 10);
        GUI_DispStringAt(disp_buf, 0, 32);    
        sprintf(disp_buf, "   确认  取消   ");
        GUI_DispStringAt(disp_buf, 0, 48);   

        switch(target_flow_set)
        {
        case TARGET_FLOW_SET_INT_PART:
            sprintf(disp_buf, "%02d", g_target_flow_set.int_part);
            GUI_DispRevStringAt(disp_buf, 24, 32);            
            break;
            
        case TARGET_FLOW_SET_DEC_PART:
            sprintf(disp_buf, "%02d", g_target_flow_set.dec_part * 10);
            GUI_DispRevStringAt(disp_buf, 48, 32);            
            break;

        case TARGET_FLOW_SET_CONFIRM:
            sprintf(disp_buf, "确认"); 
            GUI_DispRevStringAt(disp_buf, 24, 48);            
            break;

        case TARGET_FLOW_SET_CANCEL:
            sprintf(disp_buf, "取消"); 
            GUI_DispRevStringAt(disp_buf, 72, 48);            
            break;

        default:
            break;
        }        
        break;

    case MENU_SET_RTC_TIME:
        sprintf(disp_buf, " Y:%02x,M:%02x,D:%02x ", g_rtc_time_set[YEAR], g_rtc_time_set[MONTH], g_rtc_time_set[DATE]);
        GUI_DispStringAt(disp_buf, 0, 16);    
        sprintf(disp_buf, " H:%02x,M:%02x,S:%02x ", g_rtc_time_set[HOUR], g_rtc_time_set[MIN], g_rtc_time_set[SEC]);
        GUI_DispStringAt(disp_buf, 0, 32);   
        sprintf(disp_buf, "   保存  取消   ");
        GUI_DispStringAt(disp_buf, 0, 48);     

        switch(rtc_time_set)
        {
        case RTC_TIME_SET_YEAR:
            sprintf(disp_buf, "%02x", g_rtc_time_set[YEAR]); 
            GUI_DispRevStringAt(disp_buf, 25, 16);
            break;
            
        case RTC_TIME_SET_MONTH:
            sprintf(disp_buf, "%02x", g_rtc_time_set[MONTH]); 
            GUI_DispRevStringAt(disp_buf, 65, 16);           
            break;
            
        case RTC_TIME_SET_DATE:
            sprintf(disp_buf, "%02x", g_rtc_time_set[DATE]); 
            GUI_DispRevStringAt(disp_buf, 105, 16);                       
            break;
            
        case RTC_TIME_SET_HOUR:
            sprintf(disp_buf, "%02x", g_rtc_time_set[HOUR]); 
            GUI_DispRevStringAt(disp_buf, 25, 32);            
            break;
            
        case RTC_TIME_SET_MINUTE:
            sprintf(disp_buf, "%02x", g_rtc_time_set[MIN]); 
            GUI_DispRevStringAt(disp_buf, 65, 32);            
            break;
            
        case RTC_TIME_SET_SECOND:
            sprintf(disp_buf, "%02x", g_rtc_time_set[SEC]); 
            GUI_DispRevStringAt(disp_buf, 105, 32);   
            break;
            
        case RTC_TIME_SET_SAVE:
            sprintf(disp_buf, "保存"); 
            GUI_DispRevStringAt(disp_buf, 24, 48);
            break;

        case RTC_TIME_SET_CANCEL:
            sprintf(disp_buf, "取消"); 
            GUI_DispRevStringAt(disp_buf, 72, 48);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
                
    return (FORM_MSG_NONE);
}

