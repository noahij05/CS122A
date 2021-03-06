/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 3 Exercise 1
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "bit.h"
#include "lcd.h"

void SPI_MasterInit(void) {
	DDRB = 0xBF; PORTB = 0x40;
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	SREG =0x80;
}

void SPI_MasterTransmit(unsigned char cData) {
	PORTB = SetBit(PORTB,4,0);
	SPDR = cData;
	while(!(SPSR & (1<<SPIF))) { // wait for transmission to complete
		;
	}

	PORTB = SetBit(PORTB,4,1);
}
/*
void SPI_ServantInit(void) {

	DDRB = 0x40; PORTB = 0xBF;
	SPCR |= (1<<SPE) | (1<<SPIE);
	SREG = 0x80;
}

ISR(SPI_STC_vect) { 
	receivedData = SPDR;
}
*/


int main(void)
{
	DDRA = 0x0F; PORTA = 0xF0;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	SPI_MasterInit();
	unsigned short int count = 1;
	unsigned char b = 'a';
    /* Replace with your application code */
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(4);
	LCD_WriteData(b);
    while (1) 
    {
		SPI_MasterTransmit(count);
		count++;
		_delay_ms(1000);
    }
}

