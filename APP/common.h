#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define VERSION_LEN                 5u

extern const char HARDWARE_VERSION[];
extern const char SOFTWARE_VERSION[];

unsigned char Bcd2Hex(unsigned char bcd);
unsigned char Hex2Bcd(unsigned char hex);
int get_days_of_month(int year, int month);
int get_week(int year, int month, int date);


#ifdef __cplusplus
}
#endif

#endif
