#include "includes.h"


MEM_PARA g_mem_para = 
{
    MEM_PARA_TAG,
    MEM_PARA_CRC,
    MEM_PARA_DATE,

    MEMS_DEBOUNCE_THRESHOLD_DEFAULT,
    MEMS_CAL_INCOMPLETE,

    {
        0,
    }
};

const MEM_PARA g_mem_para_default = 
{
    MEM_PARA_TAG,
    MEM_PARA_CRC,
    MEM_PARA_DATE,

    MEMS_DEBOUNCE_THRESHOLD_DEFAULT,
    MEMS_CAL_INCOMPLETE,

    {
        0,
    }
};

unsigned int CalcCRC(unsigned char data, unsigned int crc)
{
    unsigned char i, chk;


    crc = crc ^ data;

    for(i = 0; i < 8; i++)
    {        
        chk = crc & 1;

        crc = crc >> 1;

        crc = crc & 0x7fff;

        if(1 == chk)
        {
            crc = crc ^ 0xa001;
        }

        crc = crc & 0xffff;
    }

    return (crc);
}

unsigned int Get_Checksum(unsigned char *buf, unsigned int len)
{
    unsigned char *ptr;
    unsigned int i, crc;  


    ptr = buf;
    crc = 0xffff; 

    for(i = 0; i < len; i++) 
    { 
        crc = CalcCRC(*ptr, crc); 

        ptr++; 
    }  

    return (crc); 
}

unsigned int mem_verify_blank(void *mem_addr, unsigned int len)
{
    unsigned int i, *mem = mem_addr;
    
    
    if((NULL == mem) || (0 == len))
    {
        return (FALSE);
    }
        
    for(i = 0; i < len; i++)
    {
        if(0xFFFFFFFF != (*mem++))
        {
            return (FALSE);
        }
    }
    
    return (TRUE);
}

void *mem_para_cpy(void *dest_addr, const void *src_addr, unsigned int count)
{
    unsigned int i;
    char *dest = dest_addr;
    const char *src = src_addr;

    
    if((NULL == dest) || (NULL == src))
    {
        return (NULL);
    }
    
    if(count > MEM_PARA_PAGE_SIZE)
    {
        return (NULL);
    }

    for(i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }

    return (dest_addr);
}

unsigned int mem_para_write(void)
{
    unsigned int i, *p_mem_para, n = 0, addr, *src_addr;
    FLASH_Status status;


    FLASH_Unlock();

    g_mem_para.crc = Get_Checksum((unsigned char *)&g_mem_para.date, CHECKSUM_MEM_PARA_SIZE);

    while(n < MEM_PARA_PROGRAM_TIMES)
    {
        p_mem_para = (unsigned int *)((unsigned int)MEM_PARA_PAGE_ADDR + n * MEM_PARA_SIZE);

        if(TRUE == mem_verify_blank(p_mem_para, MEM_PARA_SIZE / SINGLE_PARA_SIZE))
        {
            addr = (unsigned int)p_mem_para;
            src_addr = (unsigned int *)&g_mem_para;

            for(i = 0; i < (MEM_PARA_SIZE / SINGLE_PARA_SIZE); i++)
            {
                clr_wdt();
                
                status = FLASH_ProgramWord(addr, *src_addr);

                if(FLASH_COMPLETE != status)
                {
                    FLASH_Lock();

                    return (FALSE);
                }

                addr += SINGLE_PARA_SIZE;
                src_addr++;
            }

            FLASH_Lock();

            return (TRUE);
        }

        n++;
    }

    clr_wdt();

    status = FLASH_ErasePage(MEM_PARA_PAGE_ADDR);

    if(FLASH_COMPLETE != status)
    {
        FLASH_Lock();
    
        return (FALSE);
    }

    addr = (unsigned int)MEM_PARA_PAGE_ADDR;
    src_addr = (unsigned int *)&g_mem_para;

    for(i = 0; i < (MEM_PARA_SIZE / SINGLE_PARA_SIZE); i++)
    {
        clr_wdt();
        
        status = FLASH_ProgramWord(addr, *src_addr);

        if(FLASH_COMPLETE != status)
        {
            FLASH_Lock();
        
            return (FALSE);
        }

        addr += SINGLE_PARA_SIZE;
        src_addr++;
    }    
    
    FLASH_Lock();

    return (TRUE);
}

void mem_para_read(void)
{
    unsigned int *p_mem_para, crc, n = MEM_PARA_PROGRAM_TIMES;


    while(n--)
    {
        p_mem_para = (unsigned int *)((unsigned int)MEM_PARA_PAGE_ADDR + (n * MEM_PARA_SIZE));

        if(MEM_PARA_TAG == p_mem_para[MEM_PARA_TAG_INDEX])
        {
            crc = Get_Checksum((unsigned char *)&p_mem_para[MEM_PARA_DATE_INDEX], CHECKSUM_MEM_PARA_SIZE);

            if(p_mem_para[MEM_PARA_CRC_INDEX] == crc)
            {
                if(p_mem_para[MEM_PARA_DATE_INDEX] >= g_mem_para.date)
                {
                    mem_para_cpy(&g_mem_para, p_mem_para, MEM_PARA_SIZE);
                }
                else
                {

                }

                return;
            }
        }
    }

    mem_para_write();
}

void mem_para_restore(void)
{
    memcpy(&g_mem_para, &g_mem_para_default, sizeof(MEM_PARA));

    mem_para_write();
}

void MEM_Init(void)
{
    mem_para_read();
}

