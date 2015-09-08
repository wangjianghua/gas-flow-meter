#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define RTC_I2C_PORT            GPIOB
#define RTC_SCL_PIN             GPIO_Pin_6
#define RTC_SDA_PIN             GPIO_Pin_7

#define RTC_SCL_HIGH()          GPIO_SetBits(RTC_I2C_PORT, RTC_SCL_PIN)
#define RTC_SCL_LOW()           GPIO_ResetBits(RTC_I2C_PORT, RTC_SCL_PIN)

#define RTC_SDA_HIGH()          GPIO_SetBits(RTC_I2C_PORT, RTC_SDA_PIN)
#define RTC_SDA_LOW()           GPIO_ResetBits(RTC_I2C_PORT, RTC_SDA_PIN)

#define RTC_SDA_STATE()         GPIO_ReadInputDataBit(RTC_I2C_PORT, RTC_SDA_PIN)

#define MAX_RTC_I2C_TIMEOUT     50u

#define MAX_RTC_TIME_ITEM       7u

#define RTC_TIME_REG_ADDR       0u

#define MAX_WEEK_ITEM           7u

typedef enum {
    I2C_ADDR_WRITE = 0xD0,
    I2C_ADDR_READ = 0xD1
} RTCI2cHeader;

typedef enum {
    ACK_OK = 0x00,  
    ACK_ERROR = 0x01
} RTCI2cAckState;

typedef enum {
    SEC = 0,
    MIN,
    HOUR,
    WEEK,
    DATE,
    MONTH,
    YEAR,
    CTRL
} RTC_REG;

extern unsigned char g_rtc_time[MAX_RTC_TIME_ITEM];

void RTC_ReadTime(unsigned char *time);
void RTC_WriteTime(unsigned char *time);
void RTC_Init(void);


#ifdef __cplusplus
}
#endif

#endif
