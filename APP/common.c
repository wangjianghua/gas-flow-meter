#include "includes.h"


const char SOFTWARE_VERSION[] = {0x20, 0x15, 0x06, 0x29, 0x26};
const char HARDWARE_VERSION[] = {0x20, 0x15, 0x01, 0x25, 0x20};

const char DaysOfMonth[]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

unsigned char Bcd2Hex(unsigned char bcd)
{
    return ((bcd >> 4) * 10 + (bcd & 0x0f));
}

unsigned char Hex2Bcd(unsigned char hex)
{
    return ((hex % 10) | (((unsigned char)(hex / 10)) << 4));
}

int get_days_of_month(int year, int month)
{
    if((month >= 13) || (0 == month))
    {
        return (30);
    }

    if((0 == (year % 4)) && (2 == month))
    {
        return (DaysOfMonth[month - 1] + 1);
    }
    
    return (DaysOfMonth[month - 1]);
}

int get_week(int year, int month, int date)
{
	int	century, week;


    if(month <= 2)
    {
		month += 12;
		year -= 1;
    }
	
	century = year / 100;
	year = year % 100;
	week = century / 4 - 2 * century + year + year / 4 + 26 * (month + 1) / 10 + date - 1;
	week = week % 7;
    
	if(week < 0)
	{ 
		week += 7;
	}
	
	return (week);
}

