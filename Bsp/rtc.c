/** 
 * @file     rtc.c
 * @brief    实时时钟
 * @details  M41T11驱动
 * @author   华兄
 * @email    591881218@qq.com
 * @date     2015
 * @version  vX.XX
 * @par Copyright (c):  
 *           深圳市合尔凯科技有限公司
 * @par History:          
 *   version: author, date, desc\n 
 */

#include "includes.h"


unsigned char g_rtc_time[MAX_RTC_TIME_ITEM];

void RTC_CfgSDAOut(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_InitStructure.GPIO_Pin = RTC_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(RTC_I2C_PORT, &GPIO_InitStructure);
}

void RTC_CfgSDAIn(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    GPIO_InitStructure.GPIO_Pin = RTC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(RTC_I2C_PORT, &GPIO_InitStructure);
}

void RTC_I2cDelay(unsigned long dly)
{
    unsigned long i;

    
    for(i = 0; i < dly; i++);
}

void RTC_I2cStart(void)
{
    RTC_SDA_HIGH();  
    RTC_I2cDelay(50);
    RTC_SCL_HIGH();        
    RTC_I2cDelay(50);        
    RTC_SDA_LOW();    
    RTC_I2cDelay(50);        
    RTC_SCL_LOW();  
    RTC_I2cDelay(50);
}

void RTC_I2cStop(void)
{
    RTC_SDA_LOW(); 
    RTC_I2cDelay(50);
    RTC_SCL_HIGH();
    RTC_I2cDelay(50);    
    RTC_SDA_HIGH();     
    RTC_I2cDelay(50);        
    RTC_SCL_LOW();
    RTC_I2cDelay(50);
}

void RTC_I2cAck(void)
{
    RTC_SDA_LOW(); 
    RTC_I2cDelay(20);
    RTC_SCL_HIGH();
    RTC_I2cDelay(50);
    RTC_SCL_LOW();
    RTC_I2cDelay(20);   
}

void RTC_I2cNoAck(void)
{
    RTC_SDA_HIGH();     
    RTC_I2cDelay(20);
    RTC_SCL_HIGH();
    RTC_I2cDelay(50);
    RTC_SCL_LOW();
    RTC_I2cDelay(20);
}

unsigned char RTC_I2cReadByte(void)
{
    unsigned char i, data;


    RTC_CfgSDAIn();

    RTC_I2cDelay(50);

    for(i = 0, data = 0; i < 8; i++)
    {
        data <<= 1; 
        
        RTC_SCL_HIGH();   

        RTC_I2cDelay(50);
        
        if(SET == RTC_SDA_STATE()) 
        {
            data |= 0x01;
        }
        
        RTC_SCL_LOW();    

        RTC_I2cDelay(50);
    }

    RTC_CfgSDAOut();

    RTC_SDA_HIGH();

    RTC_I2cDelay(20);
    
    return (data);
}

unsigned char RTC_I2cWriteByte(unsigned char data)
{
    unsigned char i, ack = ACK_OK;
    unsigned int wait_time = 0;
      

    for(i = 0; i < 8; i++)
    {
        if(data & 0x80) 
        {
            RTC_SDA_HIGH();
        }
        else 
        {
            RTC_SDA_LOW();
        }
        
        RTC_I2cDelay(20);
        RTC_SCL_HIGH();
        RTC_I2cDelay(50);
        RTC_SCL_LOW();
        RTC_I2cDelay(20);
        
        data <<= 1;
    }

    RTC_CfgSDAIn();
        
    RTC_SCL_HIGH();    

    RTC_I2cDelay(20);

    while(SET == RTC_SDA_STATE())
    {
        if(wait_time++ > MAX_RTC_I2C_TIMEOUT)
        {
            ack = ACK_ERROR;
            
            break;
        }
    }

    RTC_SCL_LOW();

    RTC_CfgSDAOut();
    
    RTC_SDA_HIGH();

    RTC_I2cDelay(20);
    
    return (ack);
}

void RTC_ReadData(unsigned char addr, unsigned char *data, unsigned char len)
{
    unsigned char i;
    unsigned int wait_time = 0;


    if((NULL == data) || (0 == len))
    {
        return;
    }
    
    do
    {
        if(wait_time++ > MAX_RTC_I2C_TIMEOUT)
        {
            return;
        }

        RTC_I2cStart();
    } while(ACK_ERROR == RTC_I2cWriteByte(I2C_ADDR_WRITE));    

    RTC_I2cWriteByte(addr);

    RTC_I2cStart();

    RTC_I2cWriteByte(I2C_ADDR_READ);

    for(i = 0; i < (len - 1); i++)
    {             
        data[i] = RTC_I2cReadByte();         

        RTC_I2cAck();            
    }    

    data[i] = RTC_I2cReadByte(); 
    
    RTC_I2cNoAck();
    RTC_I2cStop();    
      
    RTC_SCL_HIGH();
    RTC_SDA_HIGH();
}

void RTC_WriteData(unsigned char addr, unsigned char *data, unsigned char len)
{
    unsigned char i;
    unsigned int wait_time = 0;
    

    if((NULL == data) || (0 == len))
    {
        return;
    }
    
    do
    {
        if(wait_time++ > MAX_RTC_I2C_TIMEOUT)
        {
            return;
        }
        
        RTC_I2cStart();       
    } while(ACK_ERROR == RTC_I2cWriteByte(I2C_ADDR_WRITE));    
       
    RTC_I2cWriteByte(addr);   

    for(i = 0; i < len; i++)
    {
        RTC_I2cWriteByte(data[i]);
    }    

    RTC_I2cStop();

    RTC_SCL_HIGH();     
    RTC_SDA_HIGH();
}

void RTC_ReadTime(unsigned char *time)
{
    if(NULL == time)
    {
        return;
    }

    RTC_ReadData(RTC_TIME_REG_ADDR, time, MAX_RTC_TIME_ITEM);
    
    time[SEC] &= 0x7f;
    time[MIN] &= 0x7f;
    time[HOUR] &= 0x3f;
    time[WEEK] &= 0x07;
    time[DATE] &= 0x3f;
    time[MONTH] &= 0x1f;
    time[YEAR] &= 0xff;
}

void RTC_WriteTime(unsigned char *time)
{
    if(NULL == time)
    {
        return;
    }

    time[SEC] &= 0x7f;
    time[MIN] &= 0x7f;
    time[HOUR] &= 0x3f;
    time[WEEK] &= 0x07;
    time[DATE] &= 0x3f;
    time[MONTH] &= 0x1f;
    time[YEAR] &= 0xff;

    RTC_WriteData(RTC_TIME_REG_ADDR, time, MAX_RTC_TIME_ITEM);
}

void RTC_Init(void)
{
    RTC_Config();

    RTC_SCL_HIGH();
    RTC_SDA_HIGH();
}

