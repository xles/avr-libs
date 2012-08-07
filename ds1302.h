/**
 * ds1302.h
 *
 * 
 *
 * @package     org.mirakulix.avr
 * @subpackage  ds1302
 * @author      xles <xles@mirakulix.org>
 * @copyright   Copyright (c) xles 2011
 * @link        http://web.mirakulix.org/
 */

/** 
 * DS1302 register definitions (See datasheet for further info)
 * 
 *  Register	Function		Notes
 *	0x80	Seconds			00-59
 *	0x82	Minutes			00-59
 *	0x84	Hours & 12/24 hour mode	1-12/0-23
 *	0x86	Day			1-31
 *	0x88	Month			1-12
 *	0x8A	Weekday			1-7
 *	0x8C	Year			00-99
 *	0x8E	Control register	
 *	0x90	Trickle Charge register	
 *	0xBE	Clock Burst		Sets/reads all eight calendar
 *					registers at once.
 * 
 *  Note: These register values are write only, add 0x01 for read.
 */

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#define RTC_CE		0x01
#define RTC_IO		0x02
#define RTC_CLK		0x04

#define RTC_PORT	PORTA
#define RTC_DDR		DDRA
#define RTC_PIN		PINA

#define RTC_SCLK	RTC_PORT |= RTC_CLK; \
			_delay_ms(25);	  \
			RTC_PORT &=~RTC_CLK  \
			_delay_ms(25);

#define RTC_ENABLE	RTC_PORT &=~RTC_CE
#define RTC_DISABLE	RTC_PORT |= RTC_CE

#define RTC_READ	RTC_DDR &=~RTC_IO \
			RTC_PORT|= RTC_IO
#define RTC_WRITE	RTC_DDR |= RTC_IO \
			RTC_PORT&=~RTC_IO

struct rtc_date {
	int seconds;	/* 0-59 */
	int minutes;	/* 0-59 */
	int hours;	/* 0-24 */
	int weekday;	/* 1-7 */
	int day;	/* 1-31 */
	int month;	/* 1-12 */
	int year;	/* 0-(99)2000+ */
	char iso8601[]; /* 2000-12-21T16:01:07+02:00 */
	char rfc2822[]; /* Thu, 21 Dec 2000 16:01:07 +0200 */
	bool ampm;	/* 0 for AM, 1 for PM */
	bool y2k;	/* 0 for two, 1 for four digit years */
};

/**
 * Initiates the Maxim/Dallas DS1302 RTC chip.
 * 
 * @param boolean mil | military time
 * @param boolean y2k | y2k compliance
 * @return boolean
 */
bool rtc_init(bool mil = true, bool y2k = true);

/**
 * Sends command to RTC and read data from register addr on RTC.
 * 
 * @param char addr 
 * @param char data
 * @return boolean
 */
char rtc_cmd(char addr, char data[]);

/**
 * Sets the time on the RTC from a rtc_date struct.
 * 
 * @param rtc_date date
 * @return boolean
 */
bool set_time(rtc_date &date);

/**
 * Retrieves formatted time from RTC in a rtc_date struct.
 * 
 * @param date
 * @return rtc_date
 */
rtc_date get_time();

/**
 * Fetches verbose AM/PM from boolean value (12h time only).
 * 
 * @param boolean ampm
 * @return char
 */
char get_ampm(bool ampm);

/**
 * Fetches name of weekday for a decimal weekday.
 * 
 * @param integer day
 * @param boolean abbr
 * @return char
 */
char get_weekday(int day, bool abbr = true);

/**
 * Fetches name of month for a decimal month.
 * 
 * @param integer month
 * @param boolean abbr
 * @return char
 */
char get_month(int month, bool abbr = true);

/**
 * Decodes BCD to machine readable binary value.
 * 
 * @param char x
 * @return char
 */
char bcddec(char x);

/**
 * Encodes binary value into BCD.
 * 
 * @param char x
 * @return char
 */
char bcdenc(char x);

/**
 * EOF ds1302.h
 */
