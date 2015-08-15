#include "includes.h"


MEM_PARAM g_mem_param = 
{
    MEM_PARAM_TAG,
    0,
    MEM_PARAM_DATE,

    NONE_SIGN,
    0,

    {
        0,
    }
};

const MEM_PARAM g_mem_param_default = 
{
    MEM_PARAM_TAG,
    0,
    MEM_PARAM_DATE,

    NONE_SIGN,
    0,

    {
        0,
    }
};

unsigned int CalcCRC(unsigned char *buf, unsigned int crc)
{
    unsigned char i, chk;


    crc = crc ^ *buf;

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
    unsigned char *ptr, high, low;
    unsigned int i, crc;  


    ptr = buf;
    crc = 0xffff; 

    for(i = 0; i < len; i++) 
    { 
        crc = CalcCRC(ptr, crc); 

        ptr++; 
    } 

    high = crc % 256; 
    low = crc / 256; 
    crc = (high << 8) | low; 

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

void *mem_param_cpy(void *dest_addr, const void *src_addr, unsigned int count)
{
    unsigned int i;
    char *dest = dest_addr;
    const char *src = src_addr;

    
    if((NULL == dest) || (NULL == src))
    {
        return (NULL);
    }
    
    if(count > MEM_PARAM_PAGE_SIZE)
    {
        return (NULL);
    }

    for(i = 0; i < count; i++)
    {
        dest[i] = src[i];
    }

    return (dest_addr);
}

unsigned int mem_param_write(void)
{
    unsigned int i, *p_mem_param, n = 0, addr, *src_addr;
    FLASH_Status status;


    FLASH_Unlock();

    g_mem_param.crc = Get_Checksum((unsigned char *)&g_mem_param.date, MEM_PARAM_SIZE - 8);

    while(n < MEM_PARAM_PROGRAM_TIMES)
    {
        p_mem_param = (unsigned int *)((unsigned int)MEM_PARAM_PAGE_ADDR + n * MEM_PARAM_SIZE);

        if(TRUE == mem_verify_blank(p_mem_param, MEM_PARAM_SIZE / 4))
        {
            addr = (unsigned int)p_mem_param;
            src_addr = (unsigned int *)&g_mem_param;

            for(i = 0; i < (MEM_PARAM_SIZE / 4); i++)
            {
                status = FLASH_ProgramWord(addr, *src_addr);

                if(FLASH_COMPLETE != status)
                {
                    FLASH_Lock();

                    return (FALSE);
                }

                addr += 4;
                src_addr++;
            }

            FLASH_Lock();

            return (TRUE);
        }

        n++;
    }

    status = FLASH_ErasePage(MEM_PARAM_PAGE_ADDR);

    if(FLASH_COMPLETE != status)
    {
        FLASH_Lock();
    
        return (FALSE);
    }

    addr = (unsigned int)MEM_PARAM_PAGE_ADDR;
    src_addr = (unsigned int *)&g_mem_param;

    for(i = 0; i < (MEM_PARAM_SIZE / 4); i++)
    {
        status = FLASH_ProgramWord(addr, *src_addr);

        if(FLASH_COMPLETE != status)
        {
            FLASH_Lock();
        
            return (FALSE);
        }

        addr += 4;
        src_addr++;
    }    
    
    FLASH_Lock();

    return (TRUE);
}

void mem_param_read(void)
{
    unsigned int *p_mem_param, crc, n = MEM_PARAM_PROGRAM_TIMES;


    while(n--)
    {
        p_mem_param = (unsigned int *)((unsigned int)MEM_PARAM_PAGE_ADDR + (n * MEM_PARAM_SIZE));

        if(MEM_PARAM_TAG == p_mem_param[0])
        {
            crc = Get_Checksum((unsigned char *)&p_mem_param[2], (MEM_PARAM_SIZE - 8));

            if(p_mem_param[1] == crc)
            {
                if(p_mem_param[2] >= g_mem_param.date)
                {
                    mem_param_cpy(&g_mem_param, p_mem_param, MEM_PARAM_SIZE);
                }
                else
                {

                }

                return;
            }
        }
    }

    mem_param_write();
}

void mem_init(void)
{
    mem_param_read();
}

