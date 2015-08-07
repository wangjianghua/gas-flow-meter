#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define UART_DEBUG_EN               0u //串口调试使能
#define DEBUG_INFO_PRINT_EN         0u //调试打印使能

#if (DEBUG_INFO_PRINT_EN > 0u)
#define DEBUG_WARN(s)         printf s
#define DEBUG_PRINT(s)        printf s
#else
#define DEBUG_WARN(s)
#define DEBUG_PRINT(s)           
#endif

#define CHECK_TIME_255B_2400      900u //2400bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_4800      500u //4800bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_9600      300u //9600bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_19200     200u //19200bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_38400      80u //38400bps，255字节传输超时时间，单位ms
#define CHECK_TIME_255B_312500     10u //312500bps，255字节传输超时时间，单位ms
#define MAX_SEND_PKT_CHECK_TIME      CHECK_TIME_255B_4800

#define MAX_UART_RECV_BUF_LEN       (24) //串口接收缓冲区长度
#define UART_RX_AF_THRESHOLD      0xfa //串口接收门限
#define UART_TRANSMIT_TIMEOUT       (40 / (1000 / SYS_TICKS_PER_SEC)) //串口发送、接收一字节的超时时间
#define MAX_RECV_PKT_CHECK_TIME     (1000ul * 60) //60s

typedef struct {
    struct {                        //发送数据结构
        u16 *addr;                  //发送缓冲区地址
        u16 bytes;                  //发送字节数
        bool completed;             //发送完成标志
        u32 check_time;             //255字节发送超时时间
    } tx;
    
    struct {
        u8 buf[MAX_UART_RECV_BUF_LEN]; //接收缓冲区
        u8 * volatile write_index;  //写索引
        u8 * volatile read_index;   //读索引
        volatile u16 bytes;         //缓冲区现存字节数
        u32 start_time;             //开始接收时间
        u8 timeout;                 //数据字节的超时时间
    } rx;

    void *ext;                      //扩展指针
    
#if (UART_DEBUG_EN > 0u)
    bool recv_pkt_overflow;         //接收包溢出标志
    u16 send_timeout_times;         //发送超时次数
#endif
} UART_PARAM;

#define MAX_COM_BUF_LEN              24 //公共缓冲区最大长度

typedef struct {
    volatile u8 len;                //公共缓冲区数据字节
    u8 buf[MAX_COM_BUF_LEN];        //公共缓冲区
} COM_PARAM;

extern COM_PARAM g_com_param;

u8 uart_send_data(u16 *tx_buf, u16 tx_num);
u16 get_bytes(void);
u8 get_char(void);
void uart_send_end_callback(void);


#ifdef __cplusplus
}
#endif

#endif
