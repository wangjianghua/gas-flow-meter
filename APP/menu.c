#include "includes.h"


static int form_home(unsigned int key_msg, unsigned int form_msg);
static int form_sysinfo(unsigned int key_msg, unsigned int form_msg);
static int form_set(unsigned int key_msg, unsigned int form_msg);

static const FORM form_list[MAX_FORM_NUM] =
{
    {form_home},
    
    {form_sysinfo},
    
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
    form_id = FORM_ID_HOME;

    lcd_backlight_disp_time = 0;
    
    form_home(KEY_MSG_NONE, FORM_MSG_DATA);
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

void form_home_refresh(void)
{
    char disp_buf[18];

    
    sprintf(disp_buf, "计时:  %03d:%02d:%02d", g_time_count.hour, g_time_count.minute, g_time_count.second);
    GUI_DispStringAt(disp_buf, 0, 16);
    
    sprintf(disp_buf, "流量:  %02d.%d SLPM", g_mems_para.disp_flow_int_part, g_mems_para.disp_flow_dec_part);
    GUI_DispStringAt(disp_buf, 0, 32);    

    sprintf(disp_buf, "%s", time_count_ctrl_str[g_time_count_ctrl]);
    GUI_DispStringAt(disp_buf, 0, 48);
}

void form_sysinfo_refresh(void)
{
    char disp_buf[18];

    
    sprintf(disp_buf, "        %02x:%02x:%02x", g_rtc_time[HOUR], g_rtc_time[MIN], g_rtc_time[SEC]);
    GUI_DispStringAt(disp_buf, 0, 0);

    sprintf(disp_buf, "%s", week_str[g_rtc_time[WEEK] % MAX_WEEK_ITEM]);
    GUI_DispStringAt(disp_buf, 0, 0);

    sprintf(disp_buf, "   20%02x-%02x-%02x   ", g_rtc_time[YEAR], g_rtc_time[MONTH], g_rtc_time[DATE]);
    GUI_DispStringAt(disp_buf, 0, 48);    
}

static int form_home(unsigned int key_msg, unsigned int form_msg)
{
    char disp_buf[18];
    
    
    if(FORM_MSG_REFRESH == form_msg)
    {   
        form_home_refresh();
        
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
    
    sprintf(disp_buf, "流量:  %02d.%d SLPM", g_mems_para.disp_flow_int_part, g_mems_para.disp_flow_dec_part);
    GUI_DispStringAt(disp_buf, 0, 32);    
    
    sprintf(disp_buf, "%s", time_count_ctrl_str[g_time_count_ctrl]);
    GUI_DispStringAt(disp_buf, 0, 48);       

    return (FORM_MSG_NONE);
}

static int form_sysinfo(unsigned int key_msg, unsigned int form_msg)
{
    char disp_buf[18];
    
    
    if(FORM_MSG_REFRESH == form_msg)
    {        
        form_sysinfo_refresh();

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
    static unsigned int menu_set_sn;
    static unsigned int alarm_time_set_sn;
    static ALARM_TIME alarm_time;
    static unsigned int target_flow_set_sn;
    static FLOW_PARA target_flow;
    static unsigned int rtc_time_set_sn;
    static INT8U rtc_time[MAX_RTC_TIME_ITEM];
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
            if(MENU_SET_ALARM_TIME == menu_set_sn)
            {
                alarm_time_set_sn = (alarm_time_set_sn + 1) % ALARM_TIME_SET_MAX;
            }
            else if(MENU_SET_TARGET_FLOW == menu_set_sn)
            {
                target_flow_set_sn = (target_flow_set_sn + 1) % TARGET_FLOW_SET_MAX;
            }
            else if(MENU_SET_RTC_TIME == menu_set_sn)
            {
                rtc_time_set_sn = (rtc_time_set_sn + 1) % RTC_TIME_SET_MAX;
            }
            break;

        case KEY_MSG_DOWN:
            if(MENU_SET_ALARM_TIME == menu_set_sn)
            {
                switch(alarm_time_set_sn)
                {
                case ALARM_TIME_SET_HOUR:
                    if(alarm_time.hour)
                    {
                        alarm_time.hour--;
                    }
                    else
                    {
                        alarm_time.hour = MAX_TIME_COUNT;
                    }
                    break;

                case ALARM_TIME_SET_MINUTE:
                    if(alarm_time.minute)
                    {
                        alarm_time.minute--;
                    }
                    else
                    {
                        alarm_time.minute = 59;
                    }
                    break;  

                default:
                    break;
                }
            }
            else if(MENU_SET_TARGET_FLOW == menu_set_sn)
            {
                switch(target_flow_set_sn)
                {
                case TARGET_FLOW_SET_INT_PART:
                    if(target_flow.int_part)
                    {
                        target_flow.int_part--;
                    }
                    else
                    {
                        target_flow.int_part = MAX_MEMS_FLOW - 1;
                    }
                    break;

                case TARGET_FLOW_SET_DEC_PART:
                    if(target_flow.dec_part)
                    {
                        target_flow.dec_part--;
                    }
                    else
                    {
                        target_flow.dec_part = 9;
                    }                   
                    break;

                default:
                    break;
                }
            }
            else if(MENU_SET_RTC_TIME == menu_set_sn)
            {
                switch(rtc_time_set_sn)
                {
                case RTC_TIME_SET_YEAR:
                    temp = Bcd2Hex(rtc_time[YEAR]) + 99;
                    temp %= 100;
                    rtc_time[YEAR] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_MONTH:
                    temp = Bcd2Hex(rtc_time[MONTH]);

                    if(temp > 1)
                    {
                        temp--;
                    }
                    else
                    {
                        temp = 12;
                    }

                    rtc_time[MONTH] = Hex2Bcd(temp);
                    break;

                case RTC_TIME_SET_DATE:
                    temp = Bcd2Hex(rtc_time[DATE]);
                    
                    if(temp > 1)
                    {
                        temp--;
                    }
                    else
                    {
                        temp = get_days_of_month(2000 + Bcd2Hex(rtc_time[YEAR]), Bcd2Hex(rtc_time[MONTH])); 
                    }
                    
                    rtc_time[DATE] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_HOUR:
                    temp = Bcd2Hex(rtc_time[HOUR]) + 23;
                    temp %= 24;  
                    rtc_time[HOUR] = Hex2Bcd(temp);                    
                    break;       

                case RTC_TIME_SET_MINUTE:
                    temp = Bcd2Hex(rtc_time[MIN]) + 59;
                    temp %= 60;  
                    rtc_time[MIN] = Hex2Bcd(temp);                    
                    break;    

                case RTC_TIME_SET_SECOND:
                    temp = Bcd2Hex(rtc_time[SEC]) + 59;
                    temp %= 60;  
                    rtc_time[SEC] = Hex2Bcd(temp);                    
                    break;

                default:
                    break;
                }
            }
            break;

        case KEY_MSG_UP:
            if(MENU_SET_ALARM_TIME == menu_set_sn)
            {
                switch(alarm_time_set_sn)
                {
                case ALARM_TIME_SET_HOUR:
                    alarm_time.hour++;
                    
                    if(alarm_time.hour > MAX_TIME_COUNT)
                    {
                        alarm_time.hour = 0;
                    }
                    break;

                case ALARM_TIME_SET_MINUTE:
                    alarm_time.minute++;
                    
                    if(alarm_time.minute > 59)
                    {
                        alarm_time.minute = 0;
                    }
                    break;  

                default:
                    break;
                }
            } 
            else if(MENU_SET_TARGET_FLOW == menu_set_sn)
            {
                switch(target_flow_set_sn)
                {
                case TARGET_FLOW_SET_INT_PART:
                    target_flow.int_part++;

                    if(target_flow.int_part > (MAX_MEMS_FLOW - 1))
                    {
                        target_flow.int_part = 0;
                    }
                    break;

                case TARGET_FLOW_SET_DEC_PART:
                    target_flow.dec_part++;

                    if(target_flow.dec_part > 9)
                    {
                        target_flow.dec_part = 0;
                    }                    
                    break;

                default:
                    break;
                }
            }
            else if(MENU_SET_RTC_TIME == menu_set_sn)
            {
                switch(rtc_time_set_sn)
                {
                case RTC_TIME_SET_YEAR:   
                    temp = Bcd2Hex(rtc_time[YEAR]) + 1;
                    temp %= 100;
                    rtc_time[YEAR] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_MONTH:
                    temp = Bcd2Hex(rtc_time[MONTH]) + 1;
                    
                    if(temp > 12)
                    {
                        temp = 1;
                    }
                    
                    rtc_time[MONTH] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_DATE:  
                    temp = Bcd2Hex(rtc_time[DATE]) + 1;
                    
                    if(temp > get_days_of_month(2000 + Bcd2Hex(rtc_time[YEAR]), Bcd2Hex(rtc_time[MONTH])))
                    {
                       temp = 1;                    
                    }
                    
                    rtc_time[DATE] = Hex2Bcd(temp);                    
                    break;

                case RTC_TIME_SET_HOUR:    
                    temp = Bcd2Hex(rtc_time[HOUR]) + 1;
                    temp %= 24;  
                    rtc_time[HOUR] = Hex2Bcd(temp);                    
                    break;       

                case RTC_TIME_SET_MINUTE:  
                    temp = Bcd2Hex(rtc_time[MIN]) + 1;
                    temp %= 60;  
                    rtc_time[MIN] = Hex2Bcd(temp);                    
                    break;    

                case RTC_TIME_SET_SECOND:    
                    temp = Bcd2Hex(rtc_time[SEC]) + 1;
                    temp %= 60;  
                    rtc_time[SEC] = Hex2Bcd(temp);                    
                    break;

                default:
                    break;
                }
            }            
            break;
            
        case KEY_MSG_CONFIRM:
            if(MENU_SET_ALARM_TIME == menu_set_sn)
            {
                switch(alarm_time_set_sn)
                {
                case ALARM_TIME_SET_CONFIRM:
                    memset(&g_time_count, 0, sizeof(TIME_COUNT));
                    memcpy(&g_alarm_time, &alarm_time, sizeof(ALARM_TIME));
                    break;

                case ALARM_TIME_SET_QUIT:
                    form_id = FORM_ID_HOME;
                    
                    return (FORM_MSG_DATA);                    
                    break;

                default:
                    break;
                }

                menu_set_sn = MENU_SET_TARGET_FLOW;

                target_flow_set_sn = TARGET_FLOW_SET_INT_PART;

                target_flow.int_part = g_mems_para.target / 1000;
                target_flow.dec_part = g_mems_para.target / 100;
            }
            else if(MENU_SET_TARGET_FLOW == menu_set_sn)
            {
                switch(target_flow_set_sn)
                {
                case TARGET_FLOW_SET_CONFIRM:
                    g_mems_para.target = target_flow.int_part * 1000 + target_flow.dec_part * 100;
                    break;

                case TARGET_FLOW_SET_QUIT:
                    form_id = FORM_ID_HOME;
                    
                    return (FORM_MSG_DATA);                    
                    break;

                default:
                    break;
                }
                
                menu_set_sn = MENU_SET_RTC_TIME;

                RTC_ReadTime(rtc_time);

                rtc_time_set_sn = RTC_TIME_SET_YEAR;
            }
            else if(MENU_SET_RTC_TIME == menu_set_sn)
            {
                switch(rtc_time_set_sn)
                {
                case RTC_TIME_SET_SAVE:
                    rtc_time[WEEK] = Hex2Bcd(get_week(2000 + Bcd2Hex(rtc_time[YEAR]), Bcd2Hex(rtc_time[MONTH]), Bcd2Hex(rtc_time[DATE])));
                    RTC_WriteTime(rtc_time);
                    break;

                case RTC_TIME_SET_QUIT:
                    form_id = FORM_ID_HOME;
                    
                    return (FORM_MSG_DATA);                    
                    break;

                default:
                    break;
                }
                
                menu_set_sn = MENU_SET_ALARM_TIME;
                
                memcpy(&alarm_time, &g_alarm_time, sizeof(ALARM_TIME));
                
                alarm_time_set_sn = ALARM_TIME_SET_HOUR;
            }
            break;
            
        default:
            break;
        }
    }
    else if(FORM_MSG_DATA == form_msg)
    {
        menu_set_sn = MENU_SET_ALARM_TIME;
        memcpy(&alarm_time, &g_alarm_time, sizeof(ALARM_TIME));
        alarm_time_set_sn = ALARM_TIME_SET_HOUR;
    }
    else
    {
        return (FORM_MSG_NONE);
    }

    switch(menu_set_sn)
    {
    case MENU_SET_ALARM_TIME:
        sprintf(disp_buf, "定时时间:       ");
        GUI_DispStringAt(disp_buf, 0, 16);    
        sprintf(disp_buf, "    %02d时%02d分    ", alarm_time.hour, alarm_time.minute);
        GUI_DispStringAt(disp_buf, 0, 32);    
        sprintf(disp_buf, "   确认  退出   ");
        GUI_DispStringAt(disp_buf, 0, 48);

        switch(alarm_time_set_sn)
        {
        case ALARM_TIME_SET_HOUR:
            sprintf(disp_buf, "%02d", alarm_time.hour);
            GUI_DispRevStringAt(disp_buf, 32, 32);            
            break;
            
        case ALARM_TIME_SET_MINUTE:
            sprintf(disp_buf, "%02d", alarm_time.minute);
            GUI_DispRevStringAt(disp_buf, 64, 32);            
            break;

        case ALARM_TIME_SET_CONFIRM:
            sprintf(disp_buf, "确认"); 
            GUI_DispRevStringAt(disp_buf, 24, 48);            
            break;

        case ALARM_TIME_SET_QUIT:
            sprintf(disp_buf, "退出"); 
            GUI_DispRevStringAt(disp_buf, 72, 48);            
            break;

        default:
            break;
        }
        break;

    case MENU_SET_TARGET_FLOW:
        sprintf(disp_buf, "目标流量:       ");
        GUI_DispStringAt(disp_buf, 0, 16);    
        sprintf(disp_buf, "   %02d.%02d SLPM   ", target_flow.int_part, target_flow.dec_part * 10);
        GUI_DispStringAt(disp_buf, 0, 32);    
        sprintf(disp_buf, "   确认  退出   ");
        GUI_DispStringAt(disp_buf, 0, 48);   

        switch(target_flow_set_sn)
        {
        case TARGET_FLOW_SET_INT_PART:
            sprintf(disp_buf, "%02d", target_flow.int_part);
            GUI_DispRevStringAt(disp_buf, 24, 32);            
            break;
            
        case TARGET_FLOW_SET_DEC_PART:
            sprintf(disp_buf, "%02d", target_flow.dec_part * 10);
            GUI_DispRevStringAt(disp_buf, 48, 32);            
            break;

        case TARGET_FLOW_SET_CONFIRM:
            sprintf(disp_buf, "确认"); 
            GUI_DispRevStringAt(disp_buf, 24, 48);            
            break;

        case TARGET_FLOW_SET_QUIT:
            sprintf(disp_buf, "退出"); 
            GUI_DispRevStringAt(disp_buf, 72, 48);            
            break;

        default:
            break;
        }        
        break;

    case MENU_SET_RTC_TIME:
        sprintf(disp_buf, " Y:%02x,M:%02x,D:%02x ", rtc_time[YEAR], rtc_time[MONTH], rtc_time[DATE]);
        GUI_DispStringAt(disp_buf, 0, 16);    
        sprintf(disp_buf, " H:%02x,M:%02x,S:%02x ", rtc_time[HOUR], rtc_time[MIN], rtc_time[SEC]);
        GUI_DispStringAt(disp_buf, 0, 32);   
        sprintf(disp_buf, "   保存  退出   ");
        GUI_DispStringAt(disp_buf, 0, 48);     

        switch(rtc_time_set_sn)
        {
        case RTC_TIME_SET_YEAR:
            sprintf(disp_buf, "%02x", rtc_time[YEAR]); 
            GUI_DispRevStringAt(disp_buf, 25, 16);
            break;
            
        case RTC_TIME_SET_MONTH:
            sprintf(disp_buf, "%02x", rtc_time[MONTH]); 
            GUI_DispRevStringAt(disp_buf, 65, 16);           
            break;
            
        case RTC_TIME_SET_DATE:
            sprintf(disp_buf, "%02x", rtc_time[DATE]); 
            GUI_DispRevStringAt(disp_buf, 105, 16);                       
            break;
            
        case RTC_TIME_SET_HOUR:
            sprintf(disp_buf, "%02x", rtc_time[HOUR]); 
            GUI_DispRevStringAt(disp_buf, 25, 32);            
            break;
            
        case RTC_TIME_SET_MINUTE:
            sprintf(disp_buf, "%02x", rtc_time[MIN]); 
            GUI_DispRevStringAt(disp_buf, 65, 32);            
            break;
            
        case RTC_TIME_SET_SECOND:
            sprintf(disp_buf, "%02x", rtc_time[SEC]); 
            GUI_DispRevStringAt(disp_buf, 105, 32);   
            break;
            
        case RTC_TIME_SET_SAVE:
            sprintf(disp_buf, "保存"); 
            GUI_DispRevStringAt(disp_buf, 24, 48);
            break;

        case RTC_TIME_SET_QUIT:
            sprintf(disp_buf, "退出"); 
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

