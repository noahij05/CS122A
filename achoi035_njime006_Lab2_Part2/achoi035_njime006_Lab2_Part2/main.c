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

/*
#define LEADER__ 1
#define FOLLOWER__ 0

//LEADER CODE
unsigned char a_out = 0;
unsigned char isLeader = 1;

enum Transmitstate{transmitStop, transmitStart};

int TransmitTick(int state) {
	
	switch(state) {
		default:
		case transmitStop:
		if(isLeader) {
			state = transmitStart;
			} else {
			state = transmitStop;
		}
		break;
		case transmitStart:
		if(isLeader) {
			state = transmitStart;
			} else {
			state = transmitStop;
		}
		break;
	}
	
	switch(state) {
		case transmitStart:
		if(USART_IsSendReady(LEADER__)) {
			USART_Send(a_out, LEADER__);
		}
		case transmitStop:
		break;
	}
	
	return state;
}

enum LEDcontrolstate {LEDcontrol};

int LEDcontrolTick(int state) {
	state = LEDcontrol;
	
	PORTA = a_out;
	
	return state;
}

enum SetOutputState{outputLeadOn, outputLeadOff, outputFollower};

int SetOutputTick(int state) {
	
	switch(state) {
		default:
		case outputLeadOn:
		if(isLeader) {
			state = outputLeadOff;
			} else {
			state = outputFollower;
		}
		break;
		case outputLeadOff:
		if(isLeader) {
			state = outputLeadOn;
			} else {
			state = outputFollower;
		}
		break;
		case outputFollower:
		if(isLeader) {
			state = outputLeadOn;
			} else {
			state = outputFollower;
		}
		break;
	}
	
	switch(state) {
		case outputFollower:
		break;
		case outputLeadOn:
		a_out = 0x01;
		break;
		case outputLeadOff:
		a_out = 0x00;
		break;
	}
	
	return state;
}



int main(void)
{
	DDRA = 0xFF; PINA = 0x00;
	DDRC = 0xFF; PINC = 0x00;
	DDRB = 0x00; PINB = 0xFF;
	DDRD = 0b00000101; PIND = 0b11111010;
	
	initUSART(LEADER__);
	
	struct task SetOutput = {-1, 1000, 1000, &SetOutputTick};
	struct task LEDcontrol = {-1, 50, 50, &LEDcontrolTick};
	struct task Transmit = {-1, 50, 50, &TransmitTick};
	
	tasksNum = 3;
	tasksPeriodGCD = 50;
	
	tasks = malloc(tasksNum * sizeof(task));
	tasks[0] = SetOutput;
	tasks[1] = LEDcontrol;
	tasks[2] = Transmit;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();
*/

