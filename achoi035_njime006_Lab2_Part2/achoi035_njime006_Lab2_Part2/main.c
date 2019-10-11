/*	Alexander Choi achoi035@ucr.edu
*	Noah Jimenez njime006@ucr.edu
*	Lab Section: #23
*	Assignment: Lab 2 Exercise 1
*	Exercise Description:
*		I acknowledge all content contained herein, excluding template or example
*	code, is my own original work.


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

/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 2 Exercise 3
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include <stdlib.h>
#include "usart_ATmega1284.h"
#include "scheduler.h"

#define LEADER__ 1
#define FOLLOWER__ 0

//LEADER CODE
unsigned char a_out = 0;
unsigned char c_out = 0;
unsigned char isLeader = 0;

unsigned short timeToLead = 0;

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
	PORTC = c_out;
	
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

enum Receivestate{receiveStop, receiveStart};
	
int ReceiveTick(int state) {
	switch(state) {
		default:
		case receiveStop:
			if(isLeader) {
				state = receiveStop;
			} else {
				state = receiveStart;
			}
		break;
		case receiveStart:
			if(isLeader) {
				state = receiveStop;
			} else {
				state = receiveStart;
			}
		break;
	}
	
	switch(state) {
		case receiveStop:
		break;
		case receiveStart:
			if(USART_HasReceived(FOLLOWER__)) {
				a_out = USART_Receive(FOLLOWER__);
				timeToLead = 0;
				
			}
			if(USART_IsSendReady(LEADER__)) {
				USART_Send(a_out, LEADER__);
			}
	}
	
	return state;
}

enum UpdateStatusstate {updateLeader, updateFollower};
	
int UpdateStatusTick(int state) {
	
	switch(state) {
		case updateLeader:
			if(isLeader) {
				state = updateLeader;
			} else {
				state = updateFollower;
				timeToLead = 0;
			}
		break;
		default:
		case updateFollower:
			if(isLeader) {
				state = updateLeader;
			} else {
				state = updateFollower;
			}
		break;
	}
	
	switch(state) {
		case updateFollower:
			if(timeToLead >= 3000) {
				isLeader = 1;
				
			} else {
				timeToLead+=5;
			}
		break;
		case updateLeader:
			if(USART_HasReceived(FOLLOWER__)) {
				isLeader = 0;
			} else {
				isLeader = 1;
			}
	}
	c_out = isLeader;
	
	return state;
}


int main(void)
{
	DDRA = 0xFF; PINA = 0x00;
	DDRC = 0xFF; PINC = 0x00;
	DDRB = 0x00; PINB = 0xFF;
	DDRD = 0b00000101; PIND = 0b11111010;
	
	initUSART(LEADER__);
	initUSART(FOLLOWER__);
	
	struct task SetOutput = {-1, 1000, 1000, &SetOutputTick};
	struct task Receive = {-1, 5, 5, &ReceiveTick};
	struct task LEDcontrol = {-1, 50, 50, &LEDcontrolTick};
	struct task Transmit = {-1, 50, 50, &TransmitTick};
	struct task UpdateStatus = {-1, 5, 5, &UpdateStatusTick};
		
	tasksNum = 5;
	tasksPeriodGCD = 5;
	
	tasks = malloc(tasksNum * sizeof(task));
	tasks[0] = SetOutput;
	tasks[1] = Receive;
	tasks[2] = LEDcontrol;
	tasks[3] = Transmit;
	tasks[4] = UpdateStatus;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();
	
    while (1) 
    {
    }
	return 0;
}