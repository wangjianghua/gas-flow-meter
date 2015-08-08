#include "includes.h"


TIME_PARAM g_time_count;
TIME_PARAM g_time_default;
bool g_time_count_complete;
bool g_time_count_pause;
bool g_time_set;
u32 g_app_tick;

const u8 g_time_default_pos[][2] = {
    {0, 0},
    {0, 3}
};

const u8 g_time_default_str[] = "定时";

const u8 g_time_default_set_pos[][2] = {
    {0, 5},
    {0, 3}
};

const u8 g_time_count_pos[][2] = {
    {1, 0},
    {1, 3}
};

const u8 g_time_count_str[] = "计时";

const u8 g_time_count_run_str[] = "运行";
const u8 g_time_count_pause_str[] = "暂停";
const u8 g_time_count_complete_str[] = "完毕";
const u8 g_time_count_null_str[] = "    "; //4 Space

const u8 g_time_status_pos[][2] = {
    {3, 0},
    {3, 3},
    {3, 6}
};

void init_time(void)
{
    g_time_count.second = 0;
    g_time_count.minute = 0;
    g_time_count.hour = 0;

    g_time_default.minute = 0;
    g_time_default.hour = MAX_TIME_VAL;

    g_time_count_complete = FALSE;
    g_param_setting = FALSE;
    g_time_set = FALSE;
    
    g_time_count_pause = TRUE;
    
    g_app_tick = 0;
}

u32 get_tick(void)						
{
	return (g_app_tick);
}

u32 get_diff_tick(u32 cur_tick, u32 prior_tick)
{
	if(cur_tick < prior_tick)
    {
		return (cur_tick + (~prior_tick));
	}
	else
    {
		return (cur_tick - prior_tick);
	}
}

bool check_timeout(u32 start_time, u32 interval)
{
    if(get_diff_tick(get_tick(), start_time) >= interval)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
} 

void mdelay(u32 msec)
{
 	u32 old_tick;
    

	old_tick = get_tick();

	while(get_diff_tick(get_tick(), old_tick) < msec);
}

