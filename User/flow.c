#include "includes.h"


UART_PARAM g_flow_param;
FLOW_EXT_PARAM g_flow_ext_param;
FLOW_EXT_PARAM *p_flow_ext_param;

const u8 g_flow_str[] = "Á÷Á¿";
const u8 g_flow_pos[][2] = {
    {2, 0},
    {2, 3}
};

const u16 FS400X_READ_INSTANT_FLOW[] = {0x19d, 0xf0, 0x01, 0x08, 0xf9, 0x0d, 0x00};

void init_flow(void)
{
    g_flow_param.tx.check_time = CHECK_TIME_255B_38400;
    
    g_flow_param.rx.bytes = 0;
    g_flow_param.rx.start_time = 0;
    g_flow_param.rx.timeout = 0;
    g_flow_param.rx.write_index = g_flow_param.rx.buf;
    g_flow_param.rx.read_index = g_flow_param.rx.buf;

    g_flow_param.ext = (void *)&g_flow_ext_param;

    p_flow_ext_param = (FLOW_EXT_PARAM *)g_flow_param.ext;

    memset(p_flow_ext_param, 0, sizeof(FLOW_EXT_PARAM));
}

void fs400x_read_instant_flow(void)
{
    uart_send_data((u16 *)FS400X_READ_INSTANT_FLOW, sizeof(FS400X_READ_INSTANT_FLOW));
}

