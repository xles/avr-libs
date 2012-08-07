/*****************************************************************************
 ***    File:     hd44780.h                                                ***
 ***    Author:   xles, copyright (c) 2008  xles[at]mirakulix[dot]org      ***
 ***    Function: Library for easy access to a hd44780 LCD module          ***
 ***              If you use this software and find it useful, please      ***
 ***              drop me a line and let me know what you're doing. =)     ***
 ***              Even if it doesn't work, please let me know or send      ***
 ***              me your updated version of the code so that others       ***
 ***              wont have the same problem as you might have had.        ***
 *****************************************************************************/

#include "hd44780.h"

/*
 * Write to LCD in 4 bit mode. 
 * Set cmd to 1 for data and 0 for commands.
 */
void lcd_write(unsigned char cmd, unsigned char ch)
{
	if(cmd) {
		SET_RS;
	} else {
		UNSET_RS;
	}

	UNSET_E;
	UNSET_DATA;
	while(lcd_status() == 1);

	_delay_us(100);
	PORTC|= (ch & 0xF0); /* Insert upper nibble */
	SET_E;
	UNSET_E;

	UNSET_DATA;
	while(lcd_status() == 1);

	_delay_us(100);
	PORTC|=(ch & 0x0F) << 4; /* Insert lower nibble */
	SET_E;
	UNSET_E;
}

/*
 * Checks the status of the hd44780 chip, returns 1 if busy.
 */
unsigned char lcd_status(void)
{
	unsigned char status;
	DDRC&=~DATABITS;
//	PORTC|=0x80;
//	PORTC&=~0x80;
	SET_RW;
	if((PINC & 0x80) == 0) {
		status = 0;
	} else {
		status = 1;
	}
	UNSET_RW;
	DDRC|=DATABITS;
	UNSET_DATA;
	return status;
}

/*
 * Initialisation & reset the LCD display.
 * 4 bit interface, 2 lines, 5x7 fonts.
 */
void lcd_init(void)
{
	DDRC |= 0xF7;
	PORTC&=~0xF7;

	_delay_ms(128);
	lcd_cmd(0x33);
	lcd_cmd(0x32);
	lcd_cmd(0x28);
	lcd_cmd(0x06);

//	LCD_ON_NCNB; 

//	LCD_OFF;
	LCD_CLR;
	LCD_CURSOR_HOME;
}

/*
 * Move cursor to a specific row/column
 */
void lcd_mv_cur(unsigned char col, unsigned char row)
{
	unsigned char r[] = {0x80, 0xc0, 0x94, 0xd4};
	lcd_cmd(r[row]+col);
}

/*
 * Write a string to display on the specified row/column
 */
void lcd_print(unsigned char col, unsigned char row, unsigned char*str)
{
	unsigned char c=0;
	lcd_mv_cur(col, row);
	while(*str && (c++ < LCD_WIDTH)) {
		_delay_ms(2);
		lcd_data(*str++);
	}
}

