/*****************************************************************************
 ***    File:     hd44780.h                                                ***
 ***    Author:   xles, copyright (c) 2008  xles[at]mirakulix[dot]org      ***
 ***    Function: Header inclutions, macro definitions and function        ***
 ***              prototypes for the hd44780.c LCD module library.         ***
 *****************************************************************************/

#define lcd_cmd(c) 	lcd_write(0,c)
#define lcd_data(c) 	lcd_write(1,c)

#define LCD_WIDTH 	16

#define LCD_CLR		lcd_cmd(0x01)
#define LCD_CURSOR_HOME	lcd_cmd(0x02)
#define LCD_OFF		lcd_cmd(0x08)

#define LCD_ON_NCNB	lcd_cmd(0x0c)	/* No cursor, no blink */
#define LCD_ON_NCB	lcd_cmd(0x0d)	/* No cursor,    blink */
#define LCD_ON_CNB	lcd_cmd(0x0e)	/*    Cursor, no blink */
#define LCD_ON_CB	lcd_cmd(0x0f)	/*    Cursor,    blink */

#define UNSET_E		PORTC&=~0x02
#define SET_E		PORTC|= 0x02

#define UNSET_RS	PORTC&=~0x01
#define SET_RS		PORTC|= 0x01

#define UNSET_RW	PORTC&=~0x04
#define SET_RW		PORTC|= 0x04

#define DATABITS	0xF0
#define UNSET_DATA	PORTC&=~DATABITS
#define SET_DATA	PORTC|= DATABITS

#define F_CPU 		8000000UL

#include <avr/io.h>
#include <util/delay.h>

void lcd_write(unsigned char cmd, unsigned char c);
unsigned char lcd_status(void);
void lcd_init(void);
void lcd_mv_cur(unsigned char row, unsigned char col);
void lcd_print(unsigned char row, unsigned char col, unsigned char*str);
