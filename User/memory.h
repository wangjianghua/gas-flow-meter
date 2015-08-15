#ifndef __MEMORY_H__
#define __MEMORY_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define MEM_PARAM_DATE                  0x20150815

#define MEM_PARAM_TAG                   0x19890924
#define MAX_PARAM_NUM                   16 //16 * 4B = 64B
#define MEM_PARAM_SIZE                 (sizeof(MEM_PARAM)) //64B

#define NONE_SIGN                       0
#define POSITIVE_SIGN                   1
#define NEGATIVE_SIGN                   2

#define MEM_PARAM_PAGE_ADDR            (0x08018C00ul)
#define MEM_PARAM_PAGE_SIZE            (1 * 1024) //1KB
#define MEM_PARAM_PROGRAM_TIMES        (MEM_PARAM_PAGE_SIZE / MEM_PARAM_SIZE) //1024 / 64 = 16

typedef struct _mem_param_
{
    unsigned int tag; //存储标签
    unsigned int crc; //校验和
    unsigned int date; //日期

    unsigned int sign; //符号位
    unsigned int cal_val; //校准值

    unsigned int param_reserve[MAX_PARAM_NUM - 5];
} MEM_PARAM, *P_MEM_PARAM;

extern MEM_PARAM g_mem_param;

unsigned int mem_param_write(void);
void mem_param_read(void);
void mem_init(void);


#ifdef __cplusplus
}
#endif

#endif
