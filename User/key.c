#include "includes.h"


KEY_PARAM g_key_param;
bool g_param_setting;

void init_key(void)
{
    g_key_param.set_cnt = 0;
    g_key_param.up_cnt = 0;
    g_key_param.down_cnt = 0;
    g_key_param.pause_cnt = 0;
    g_key_param.clear_cnt = 0;
}

