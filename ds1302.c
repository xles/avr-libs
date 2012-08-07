// <editor-fold defaultstate="collapsed" desc="Header DocBlock">
/**
 * ds1302.c
 *
 * 
 *
 * @package     org.mirakulix.avr
 * @subpackage  ds1302
 * @author      xles <xles@mirakulix.org>
 * @copyright   Copyright (c) xles 2011
 * @link        http://web.mirakulix.org/
 */
// </editor-fold>

#include "ds1302.h"


bool rtc_mil, rtc_y2k;

bool rtc_init(bool mil = true, bool y2k = true)
{
	RTC_DDR |= RTC_CE + RTC_IO + RTC_CLK;
	RTC_PORT&=~RTC_CE + RTC_IO + RTC_CLK;
	
	rtc_mil = mil;
	rtc_y2k = y2k;
	
	return 0;
}

char rtc_cmd(char addr, char data[])
{
	char tmp, r[];
	int i, c;
	RTC_ENABLE;
	RTC_WRITE;
	for(i=0;i<8;i++) {
		if(((addr >> i) & 0x01) == 1)
			RTC_PORT|= RTC_IO;
		else
			RTC_PORT&=~RTC_IO;
		
		RTC_SCLK;
	}
	if((addr & 0x01) == 1) {
		RTC_READ;
		for(i=0;i<data;i++) {
			tmp |= RTC_PIN & RTC_IO;
			RTC_SCLK;
			tmp>>=1;
			if(data%8 == 0) {
				r[] = tmp;
				tmp=0;
			}
		}
	} else {
		RTC_WRITE;
		for(c=0;c<strlen(data);c++) {
			for(i=0;i<8;i++) {
				if(((data[c] >> i) & 0x01) == 1)
					RTC_PORT|= RTC_IO;
				else
					RTC_PORT&=~RTC_IO;

				RTC_SCLK;
			}
		}
	}
		
	RTC_DISABLE;
	return r;
}

/* fix me */
bool set_time(rtc_date &date)
{
	char data[];
	
	data[] = bcdenc(date.seconds);
	data[] = bcdenc(date.minutes);
	data[] = bcdenc(date.hours); /* fix me */
	data[] = bcdenc(date.day);
	data[] = bcdenc(date.month);
	data[] = bcdenc(date.weekday);
	data[] = bcdenc(date.year);
	
	rtc_cmd(0xBE,data);
	return 0;
}

/* fix me */
rtc_date get_time()
{
	char data[], tmp[];
	int i;
	
	data = rtc_cmd(0xBF,64);
	
	for(i=0;i<strlen(data);i++) {
		
	}
	
	return data;
}

char get_ampm(bool ampm)
{
	char data[];
	if(ampm == true) {
		data = "PM";
	} else {
		data = "AM";
	}
	return data;
}

char get_weekday(int day, bool abbr = true)
{
	char data[];
	
	switch(day) {
		case 1:
			data = "Monday";
			break;
		case 2:
			data = "Tuesday";
			break;
		case 3:
			data = "Wednesday";
			break;
		case 4:
			data = "Thursday";
			break;
		case 5:
			data = "Friday";
			break;
		case 6:
			data = "Saturday";
			break;
		case 7:
			data = "Sunday";
			break;
	}
	if(abbr == true) {
		strncpy(data,data,3);
	}
	
	return data;
}

char get_month(int month, bool abbr = true)
{
	char data[];
	
	switch(month) {
		case 1:
			data = "January";
			break;
		case 2:
			data = "February";
			break;
		case 3:
			data = "March";
			break;
		case 4:
			data = "April";
			break;
		case 5:
			data = "May";
			break;
		case 6:
			data = "June";
			break;
		case 7:
			data = "July";
			break;
		case 8:
			data = "August";
			break;
		case 9:
			data = "September";
			break;
		case 10:
			data = "October";
			break;
		case 11:
			data = "November";
			break;
		case 12:
			data = "December";
			break;
	}
	if(abbr == true) {
		strncpy(data,data,3);
	}
	
	return data;
}

char bcdenc(char x)
{
	char y;
	
	y = (x / 10) << 4;
	y = y | (x % 10);
	
	return y;
}

char bcddec(char x)
{
	char y;
    
	y = (x >> 4) * 10;
	y = y + (x & 0x0F);

	return y;
}

/**
 * EOF ds1302.c
 */
