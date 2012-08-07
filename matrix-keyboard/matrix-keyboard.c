/*****************************************************************************
 ***    File:     untitled.c                                               ***
 ***    Author:   xles, copyright (c) 2008  xles[at]mirakulix[dot]org      ***
 ***    Function: TBA                                                      ***
 *****************************************************************************/

#include "matrix-keyboard.h"

/*
unsigned char kbd_scan()
{
	unsigned char ret;
	ret=keyboardLastscan;
	keyboardLastscan = 0xFF;
	return ret;
}
*/

void kbd_init()
{
	/* Setup the keyboard inputs, set PB0-PB2 as inputs, 
		PB3-PB6 as outputs. Also PD2(INT0) as input */
	DDRA &=~0x0F;	/* PB0, PB1, PB2, PB3 as inputs, 0x01|0x02|0x04|0x08 */
	DDRA |= 0xF0; 	/* PB4, PB5, PB6, PB7 as outputs, 0x10|0x20|0x40|0x80 */
	PORTA|= 0x0F;	/* Make sure we got pullup on PB0-PB2, 
				and low signal on PB3-PB6 */
	PORTA&=~0xF0;

//	DDRD &=~0x04;	/* PD2(int0) as input */
//	PORTD|= 0x04;	/* Pullup on PD2 */

	/* Setup interrupts, int0 should trig on high-to-low-transition */
//	MCUCR|=0x2;	/* Bit 1 set, bit 0 not set: xxxxxx10 = falling edge */
//	MCUCR&=~0x1;
//	GICR|=0x40;	/* Enable int0 */
	
	keyboardLastscan = 0xFF;
	/* Done! */
}

/* Keyboard ISR routine */
//SIGNAL(SIG_INTERRUPT0)
unsigned char kbd_scan()
{
	unsigned short r,c,tmp;

	/* User pressed a key. Scan the matrix to find WHICH key */
	const unsigned char keyMap[] = {0x31, 0x32, 0x33, 0x41,	 /* (1,2,3,A) */
					0x34, 0x35, 0x36, 0x42,	 /* (4,5,6,B) */
					0x37, 0x38, 0x39, 0x43,	 /* (7,8,9,C) */
					0x2A, 0x30, 0x23, 0x44}; /* (*,0,#,D) */

	/* Already got an unprocessed input */
//	if(keyboardLastscan != 0xFF)
//		return keyboardLastscan;
	
	/* Set all rows HIGH */
	PORTA|=0xF0;

	for(r=0; r < 4 && keyboardLastscan == 0xFF; r++) {
		/* Enable the row by setting it to LOW */
		PORTA&=~(1<<(r+4));
		for(c=0; c < 4 && keyboardLastscan == 0xFF; c++) {
			/* Read the column */
			if((PINA & (1<<(c))) == 0) {
				/* Key seems to be pressed. Assign the keyMap 
					value to result. Since result no longer 
					is 0xFF this will exit all loops. */
				keyboardLastscan = keyMap[(r*4) + c];
			}
		}
		/* Disable the row by setting it to 1 again */
		PORTA|=(1<<(r+4)); 
	}
	
	/* Set all rows low again to allow new interrupts */
	PORTA&=~0xF0;
	tmp = keyboardLastscan;
	keyboardLastscan = 0xFF;
	return tmp;
}

