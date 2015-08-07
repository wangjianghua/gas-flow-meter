#ifndef __TIME_H__
#define __TIME_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define TIME_MINUTE_SET           1
#define TIME_HOUR_SET             2

#define MAX_TIME_VAL             99

#define TIME_DEFAULT_STR_POS      0
#define TIME_DEFAULT_POS          1

#define TIME_COUNT_STR_POS        0
#define TIME_COUNT_POS            1

#define TIME_DEFAULT_HOUR_POS     0
#define TIME_DEFAULT_MINUTE_POS   1

#define TIME_STATUS_RUN_POS       0
#define TIME_STATUS_PAUSE_POS     1
#define TIME_STATUS_COMPLETE_POS  2

typedef struct {
    s8 second;
    s8 minute;
    s8 hour;
} TIME_PARAM;

extern TIME_PARAM g_time_count;
extern TIME_PARAM g_time_default;
extern const u8 g_time_default_pos[][2];
extern const u8 g_time_default_str[];
extern const u8 g_time_count_pos[][2];
extern const u8 g_time_count_str[];
extern bool g_time_count_complete;
extern const u8 g_time_default_set_pos[][2];
extern bool g_time_setting;
extern bool g_time_set;
extern const u8 g_time_count_run_str[];
extern const u8 g_time_count_pause_str[];
extern const u8 g_time_count_complete_str[];
extern const u8 g_time_count_null_str[];
extern const u8 g_time_status_pos[][2];
extern u32 g_app_tick; 
extern bool g_time_count_pause;

void init_time(void);
u32 get_tick(void);
bool check_timeout(u32 start_time, u32 interval);
void mdelay(u32 msec);


#ifdef __cplusplus
}
#endif

#endif
