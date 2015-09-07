#include "includes.h"


const char SOFTWARE_VERSION[] = {0x20, 0x15, 0x06, 0x29, 0x26};
const char HARDWARE_VERSION[] = {0x20, 0x15, 0x01, 0x25, 0x20};

unsigned char Bcd2Hex(unsigned char bcd)
{
    return ((bcd >> 4) * 10 + (bcd & 0x0f));
}

unsigned char Hex2Bcd(unsigned char hex)
{
    return (( hex % 10) | (((unsigned char)(hex / 10)) << 4));
}

