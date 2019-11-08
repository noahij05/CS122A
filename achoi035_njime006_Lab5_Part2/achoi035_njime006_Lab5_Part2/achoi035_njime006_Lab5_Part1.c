/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 5 Exercise 1
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include <stdlib.h>
#include "scheduler.h"

#define SER 0
#define RCLK 2
#define SRCLK 3
#define SRCLR 4

#define UP 0x02
#define DOWN 0x01

unsigned char global_light = 0;
unsigned char inc = 0, dec = 0, clear = 0;

void transmit_data(unsigned char data) {
	PORTC |= (1 << SRCLR);
	PORTC &= ~(1 << RCLK);
	for (unsigned char i = 0; i < 8; i++) {
		PORTC &= ~(1 << SRCLK);
		if(data & 0x01) {
			PORTC |= (1 << SER);
			} else {
			PORTC &= ~(1 << SER);
		}
		PORTC |= (1 << SRCLK);
		data >>= 1;
	}
	PORTC |= (1 << RCLK);
}

enum ReadIN_states {Read_default, Read_up, Read_down, Read_both};

int ReadIN_tick(int state) {
	unsigned char button_up = ~PINA & UP;
	unsigned char button_down = ~PINA & DOWN;
	
	switch(state) {
		default:
		case Read_default:
		case Read_both:
			if(button_up && button_down) {
				state = Read_both;
				clear = 1;
			} else if(button_down) {
				state = Read_down;
				dec = 1;
			} else if(button_up) {
				state = Read_up;
				inc = 1;
			} else {
				state = Read_default;
			}
		break;
		case Read_up:
			if(button_up && button_down) {
					state = Read_both;
					clear = 1;
				} else if(button_down) {
					state = Read_down;
					dec = 1;
				} else if(button_up) {
					state = Read_up;
				} else {
					state = Read_default;
			}
		break;
		case Read_down:
			if(button_up && button_down) {
					state = Read_both;
					clear = 1;
				} else if(button_down) {
					state = Read_down;
				} else if(button_up) {
					state = Read_up;
					inc = 1;
				} else {
				state = Read_default;
			}
		break;
	}
	return state;
}

int updateVal_tick(int state) {
	state = 0;
	
	if(inc && global_light < 0xFF) {
		global_light++;
	} 
	if(dec && global_light > 0) {
		global_light--;
	}
	if(clear) {
		global_light = 0;
	}
	
	inc = 0;
	dec = 0;
	clear = 0;
	
	return state;
}

int outPut_tick(int state) {
	state = 0;
	
	transmit_data(global_light);
	
	return state;
}

int main(void)
{
	DDRA = 0x00; PINA = 0xFF;
	DDRC = 0xFF; PINC = 0x00;
	
	struct task readIN = {-1, 50, 50, &ReadIN_tick};
	struct task updateVal = {-1, 50, 50, &updateVal_tick};
	struct task outPut = {-1, 50, 50, &outPut_tick};
	tasksPeriodGCD = 50;
	
	tasksNum = 3;
	tasks = malloc(sizeof(struct task) * tasksNum);
	tasks[0] = readIN;
	tasks[1] = updateVal;
	tasks[2] = outPut;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}