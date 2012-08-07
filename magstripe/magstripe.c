/*****************************************************************************
 ***    File:     magstripe.c                                              ***
 ***    Author:   xles, copyright (c) 2008  xles[at]mirakulix[dot]org      ***
 ***    Function: Various functions to read and process data from a        ***
 ***              magnetic cardreader. Some functions are possibly         ***
 ***              track 2 only. If the code works on other tracks          ***
 ***              please feel free to contact me about it.                 ***
 ***              I would also appriciate it if you dropped me a line      ***
 ***              if you use this software and find it useful.             ***
 ***              Even if it doesn't work, please let me know or send      ***
 ***              me your updated version of the code so that others       ***
 ***              wont have the same problem as you might have had.        ***
 *****************************************************************************/

#include "magstripe.h"
#include "hd44780.h"

/*
 * Initialisation of the magstripe reader
 */
void magstripe_init(void) 
{
	DDRD &=~0x07;
	DDRD |= 0xE0;
	PORTD|= 0x04;
	/* Setup interrupts, int0 should trig on falling edge */
	MCUCR|= 0x02;	/* Bit 1 set,                    */
	MCUCR&=~0x01;	/* bit 0 not set: = falling edge */
	GICR |= 0x40;	/* Enable int0 */

}

/*
 * Read the bitstream on the DATA line (defined in magstripe.h)
 */
char *read_data(void) 
{
	int i=0;
	char buffer[255];
	char *data = cardinfo;
	while(CP == 0) {
		if(CLK == 1) {
			buffer[i] = DATA;
			i++;
			while(CLK == 1);
		}
	}
	data = parse_data(buffer);
	return data;
}

/*
 * Parses the bitstream and returns the preformated datastring.
 */
char *parse_data(const char *buffer)
{
	int i, j=0, start, end;
	static char data[50];
	char tmp;
	start = find_start(buffer);
	end = find_end(buffer, start);
	for(i=start;i<end;i+=5) {
		tmp  = 1 * buffer[i];
		tmp += 2 * buffer[i+1];
		tmp += 4 * buffer[i+2];
		tmp += 8 * buffer[i+3];
		tmp += 0x30;
		data[j] = tmp;
		lcd_data(tmp);
		j++;
	}
	return data;
}

/*
 * Finds the start character 0b1011 ';' (ASCII 0x3B) in the bitstream and
 * returns it's position in the stream.
 */
int find_start(const char *buff)
{
	int ch=0, i;
	for(i=0;i<255;i++) {
		if((buff[i] == 1) && (buff[i+1] == 1) && (buff[i+2] == 0)
				&& (buff[i+3] == 1) && (buff[i+4] == 0)) {
			ch = i;
			break;
		}
	}
	return ch;
}

/*
 * Finds the end character 0b1111 '?' (ASCII 0x3F) in the bitstream and 
 * returns it's position in the stream.
 */
int find_end(const char *buff, int start)
{
	int ch=0, i;
	for(i=start;i<255;i+=5) {
		if((buff[i] == 1) && (buff[i+1] == 1) && (buff[i+2] == 1)
				&& (buff[i+3] == 1) && (buff[i+4] == 1)) {
			ch = i;
			break;
		}
	}
	return ch;
}

/*
 *
 
char read_card(void)
{
	char *tmp;
	
	tmp = read_data();
//	lcd_print(0,1,buffer);
	return *tmp;
}
*/
void beep(int dur, int rep, int delay)
{
	int i, j;
	for(i=0;i<rep;i++) {
		PORTD |= BUZZER;
		for(j=0;j<dur;j++) {
			_delay_ms(250);
		}
		PORTD &=~BUZZER;
		for(j=0;j<delay;j++) {
			_delay_ms(250);
		}
	}
}		

/*
 *
 */
ISR(SIG_INTERRUPT0)
{
	unsigned char *tmp, *bice = "test";
//	lcd_print(0,1,bice);
//	beep(2,3,2);
	tmp = read_data();
	lcd_print(0,1,tmp);
	beep(5,2,3);
}
