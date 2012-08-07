/*****************************************************************************
 ***    File:     untitled.h                                               ***
 ***    Author:   xles, copyright (c) 2008  xles[at]mirakulix[dot]org      ***
 ***    Function: TBA                                                      ***
 *****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

/* Function prototypes */
unsigned char kbd_scan();
void kbd_init();
unsigned char keyboardLastscan;
