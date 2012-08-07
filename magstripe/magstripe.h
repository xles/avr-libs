/*****************************************************************************
 ***    File:     magstripe.h                                              ***
 ***    Author:   xles, copyright (c) 2008  xles[at]mirakulix[dot]org      ***
 ***    Function: Header inclutions, macro definitions and function        ***
 ***              prototypes for the magstripe.c magnetic cardreader       ***
 ***              function library.                                        ***
 *****************************************************************************/

#define BUZZER		0x20
#define LED_R		0x40
#define LED_G		0x80
#define CLK		(PIND & 0x01)
#define DATA		~(PIND & 0x02)
#define CP		(PIND & 0x04)

#define F_CPU 		8000000UL

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* global */
char cardinfo[50];

void magstripe_init(void);
char read_card(void);
int find_end(const char *buffer, int start);
int find_start(const char *buffer);
char *parse_data(const char *buffer);
char *read_data(void);
