/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 7 Exercise 1
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <util/delay.h>


#define DDR_SPI DDRB

#define DD_SS 4
#define DD_MOSI 5
#define DD_MISO 6
#define DD_SCK 7
unsigned char speech[30];

unsigned char SPI_received_data = 0;

void SPI_ServantInit(void) {
	DDR_SPI |= (1 << DD_MISO);
	DDR_SPI &= ~(1 << DD_MOSI) & ~(1 << DD_SS) & ~(1 << DD_SCK);
	SPCR |= (1 << SPE) | (1 << SPIE);
	SREG |= (0x80);
}

ISR(SPI_STC_vect) {
	SPI_received_data = SPDR;
}



int main(void)
{
	DDRB = 0x0F; PORTB = 0xF0;
	DDRD = 0xFF; PORTD = 0x00;
	SPI_ServantInit();
	LCD_init();
	LCD_Cursor(1);
	unsigned char lightValue;  
    while (1) 
    {
		lightValue = SPI_received_data;
		speech[3] = '0' + (lightValue % 10); 
		speech[2] =  '0' + ((lightValue / 10)%10);
		speech[1] =  '0' + ((lightValue / 100)%10);
		speech[0] =  '0' + ((lightValue / 1000)%10);
		unsigned char* to_output = speech;
		LCD_DisplayString(1, to_output);
		_delay_ms(100);
    }
}

