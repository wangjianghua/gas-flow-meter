#ifndef __FLOW_H__
#define __FLOW_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define FS400X_RECV_TIMEOUT  (1000u * 5) //FS400X数据接收超时时间

#define READ_INSTANT_FLOW          0xF0 //读瞬时流量命令

#define FRAME_HEAD                 0x9D
#define FRAME_TAIL                 0x0D

#define FS400X_CMD_POS                1

#define FS400X_FRAME_HEAD             g_com_param.buf[0]
#define FS400X_CMD                    g_com_param.buf[1]
#define FS400X_DATA_LEN               g_com_param.buf[2]
#define FS400X_CRC                    g_com_param.buf[g_com_param.len - 2]
#define FS400X_FRAME_TAIL             g_com_param.buf[g_com_param.len - 1]

#define MAX_FLOW_NUM                  10

#define MAX_FLOW_DEC_PART_VAL         9
#define MAX_FLOW_INT_PART_VAL         49

#define MAX_CAL_DEC_PART_VAL          99
#define MAX_CAL_INT_PART_VAL          49

typedef struct {
    u32 inst_val; //瞬时流量
    u32 avg_val; //平均流量

    u16 disp_int_part;
    u16 disp_dec_part;
    u32 disp_val; //显示流量

    s16 user_int_part;
    s16 user_dec_part;
    u32 user_val; //用户输入的指定流量

    s16 cal_int_part;
    s16 cal_dec_part;
    u32 cal_val;
    
    u16 inside_count;

    u32 sum;

    u32 discrete; //离散性

    u32 buf[MAX_FLOW_NUM];
    u16 index;
} FLOW_EXT_PARAM;

#define FLOW_STR_POS                  0
#define FLOW_POS                      1

#define CAL_STR_POS                   0
#define CAL_POS                       1

#define FS400X_INSTANT_FLOW_READ_PERIOD   5 

extern UART_PARAM g_flow_param;
extern FLOW_EXT_PARAM *p_flow_ext_param;
extern bool g_flow_set;
extern bool g_cal_set;
extern const u8 g_flow_str[];
extern const u8 g_cal_str[];
extern const u8 g_flow_pos[][2];
extern const u8 g_cal_pos[][2];
extern const u8 g_flow_set_pos[][2];
extern const u8 g_cal_set_pos[][2];

void init_flow(void);
void fs400x_read_instant_flow(void);


#ifdef __cplusplus
}
#endif

#endif
