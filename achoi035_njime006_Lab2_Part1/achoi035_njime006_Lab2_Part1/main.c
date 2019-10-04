/*	Alexander Choi achoi035@ucr.edu
*	Noah Jimenez njime006@ucr.edu
*	Lab Section: #23
*	Assignment: Lab 2 Exercise 1
*	Exercise Description:

*		I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <usart_ATmega1284.h>


char temp = 0;


enum SM1_States {Sent, NSent} state;

void Receive()
{
	switch(state)
	{
		case NSent:
			if(USART_HasReceived(0))
				{state = Sent; break;}
			else
				{state = NSent; break;}
				
		case Sent:
			{state = NSent; break;}
			
		default:
			state = NSent;
	}
	
	switch(state)
	{
		case Sent:
			temp = USART_Receive(0);
			break;
			
		default: 
			break;
	}
	
	//return state;
}

int main(void)
{
	DDRA = 0xFF; PINA = 0x00;
	//DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0x00; PIND = 0xFF;
	initUSART(0);
	USART_Flush(0);
    
    while (1) 
    {
		Receive();
		PORTA = temp;
    }
}

