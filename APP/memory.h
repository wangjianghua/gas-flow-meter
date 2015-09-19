#ifndef __MEMORY_H__
#define __MEMORY_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define MEM_PARA_DATE                   0x20150919

#define MEM_PARA_TAG                    0x19890924
#define MAX_PARA_NUM                    16 //16 * 4B = 64B
#define MEM_PARA_SIZE                  (sizeof(MEM_PARA)) //64B

#define MEM_PARA_TAG_INDEX              0
#define MEM_PARA_CRC_INDEX              1
#define MEM_PARA_DATE_INDEX             2

#define SINGLE_PARA_SIZE                4

#define CHECKSUM_MEM_PARA_SIZE         (MEM_PARA_SIZE - 2 * SINGLE_PARA_SIZE)

#define MEM_PARA_PAGE_ADDR             (0x08018C00ul)
#define MEM_PARA_PAGE_SIZE             (1 * 1024) //1KB
#define MEM_PARA_PROGRAM_TIMES         (MEM_PARA_PAGE_SIZE / MEM_PARA_SIZE) //1024 / 64 = 16

#define PLC_GROUP_DEFAULT               0x30

typedef struct
{
    unsigned int tag; //存储标签
    unsigned int crc; //校验和
    unsigned int date; //日期

    unsigned int plc_group;

    unsigned int para_reserve[MAX_PARA_NUM - 4];
} MEM_PARA, *P_MEM_PARA;

extern MEM_PARA g_mem_para;

unsigned int mem_para_write(void);
void mem_para_read(void);
void MEM_Init(void);


#ifdef __cplusplus
}
#endif

#endif
