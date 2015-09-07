#include "includes.h"


const FORM form_list[MAX_FORM_NUM] =
{
    {form_main},
    
    {form_info},
    
    {form_set},
};

const char *week_str[] =
{
    "星期日",
    "星期一",
    "星期二",
    "星期三",
    "星期四",
    "星期五",
    "星期六",
};

unsigned int form_id;

void form_refresh(void)
{
    if((FORM_ID_SET != form_id) && (NULL != form_list[form_id].fnct))
    {
        (*form_list[form_id].fnct)(KEY_MSG_NONE, FORM_MSG_REFRESH);
    }
}

void form_main_refresh(void)
{
    char disp_buf[18];

    
    sprintf(disp_buf, "计时:  %03d:%02d:%02d", g_time_count.hour, g_time_count.minute, g_time_count.second);
    GUI_DispStringAt(disp_buf, 0, 16);
    
    sprintf(disp_buf, "流量:  50.0 SLPM");
    GUI_DispStringAt(disp_buf, 0, 32);    
    
    sprintf(disp_buf, "运行  暂停  完毕");
    GUI_DispStringAt(disp_buf, 0, 48);
}

void form_info_refresh(void)
{
    char disp_buf[18];

    
    sprintf(disp_buf, "        %02x:%02x:%02x", g_rtc_time[HOUR], g_rtc_time[MIN], g_rtc_time[SEC]);
    GUI_DispStringAt(disp_buf, 0, 0);

    sprintf(disp_buf, "%s", week_str[g_rtc_time[HOUR] % 7]);
    GUI_DispStringAt(disp_buf, 0, 0);

    sprintf(disp_buf, "   20%02x-%02x-%02x   ", g_rtc_time[YEAR], g_rtc_time[MONTH], g_rtc_time[DATE]);
    GUI_DispStringAt(disp_buf, 0, 48);    
}

int form_main(unsigned int key_msg, unsigned int form_msg)
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
            break;

        default:
            break;
        }
    }
    else if(FORM_MSG_DATA == form_msg)
    {
        sprintf(disp_buf, "计时:  %03d:%02d:%02d", g_time_count.hour, g_time_count.minute, g_time_count.second);
        GUI_DispStringAt(disp_buf, 0, 16);

        sprintf(disp_buf, "流量:  50.0 SLPM");
        GUI_DispStringAt(disp_buf, 0, 32);    

        sprintf(disp_buf, "运行  暂停  完毕");
        GUI_DispStringAt(disp_buf, 0, 48);
    }
    else
    {
        return (FORM_MSG_NONE);
    }

    sprintf(disp_buf, "定时:  999时59分");
    GUI_DispStringAt(disp_buf, 0, 0);       

    return (FORM_MSG_NONE);
}

int form_info(unsigned int key_msg, unsigned int form_msg)
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
            break;

        default:
            break;
        }
    }
    else if(FORM_MSG_DATA == form_msg)
    {
        sprintf(disp_buf, "        %02x:%02x:%02x", g_rtc_time[HOUR], g_rtc_time[MIN], g_rtc_time[SEC]);
        GUI_DispStringAt(disp_buf, 0, 0);
        
        sprintf(disp_buf, "%s", week_str[g_rtc_time[HOUR] % 7]);
        GUI_DispStringAt(disp_buf, 0, 0);
        
        sprintf(disp_buf, "   20%02x-%02x-%02x   ", g_rtc_time[YEAR], g_rtc_time[MONTH], g_rtc_time[DATE]);
        GUI_DispStringAt(disp_buf, 0, 48);
    }
    else
    {
        return (FORM_MSG_NONE);
    }    

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
                      HARDWARE_VERSION[3]);
    GUI_DispStringAt(disp_buf, 0, 32);    

    return (FORM_MSG_NONE);
}

int form_set(unsigned int key_msg, unsigned int form_msg)
{
    return (FORM_MSG_NONE);
}

void form_init(void)
{
    form_id = FORM_ID_MAIN;
    
    form_main(KEY_MSG_NONE, FORM_MSG_DATA);
}

