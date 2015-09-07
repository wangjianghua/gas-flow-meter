#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
 extern "C" {
#endif


extern const char SOFTWARE_VERSION[];
extern const char HARDWARE_VERSION[];

unsigned char Bcd2Hex(unsigned char bcd);
unsigned char Hex2Bcd(unsigned char hex);


#ifdef __cplusplus
}
#endif

#endif
