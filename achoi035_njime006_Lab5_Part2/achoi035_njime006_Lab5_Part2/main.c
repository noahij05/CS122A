/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 5 Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */ 

#include <avr/io.h>

void ShiftData(unsigned char data) {
	int i;
	for(i = 7; i >= 0; i--) {
		//SRCLR to HIGH
		//Clear SRCLK
		PORTD = 0x08;
		//SER gets next bit
		PORTD |= ((data >> i) & 0x01);
		//SRCLK HIGH
		PORTD |= 0x04;
	}
	//RCLK HIGH
	PORTD |= 0x02;
	//Clear data Lines
	PORTD = 0x00;
}

void ShiftDataEnable() {
	PORTD = 0x08;
}


int main(void)
{
    DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	
	
    while (1) 
    {
    }
}

