/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 5 Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */ 

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

void ShiftData(unsigned char data) {
	int i;
	for(i = 7; i >= 0; i--) {
		//SRCLR to HIGH
		//PORTD = 0x08;
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


enum LEDState {INIT,L0,L1,L2,L3,L4,L5,L6,L7} led_state;
enum LEDFestive {INIT1,L0F,LF0} led_festive;
enum LEDFestive1 {INIT2,L05,L0A,L41,L28,L50,LA0} led_festive1;
char count = 1;
char flag = 1;	
	
	

void LEDS_Init(){
	led_state = INIT;
	led_festive = INIT1;
	led_festive1 = INIT2;
}


void button_check()
{	
	if((~PINA & 0x03) == 0x03)
	{	
		switch(flag)
		{
			case 0:
				flag = 1; //count = 1;
				break;
			case 1:
				flag = 0; ShiftData(0);
				break;
		}
	}
	
	if ((~PINA & 0x01) == 1 && count != 3 && flag == 1)
	{
		count++;
	}
	if ((~PINA & 0x02) == 2 && count != 1 && flag == 1)
	{
		count--;
	}
	
}

void LEDS_Tick(char count1, char flag1){
	
	if (count1 == 1 && flag1 == 1)
	{
		//Actions
		switch(led_state){
			case INIT:
				ShiftData(0x00);
			break;
			case L0:
				ShiftData(0x01);
			break;
			case L1:
				ShiftData(0x02);
			break;
			case L2:
				ShiftData(0x04);
			break;
			case L3:
				ShiftData(0x08);
			break;
			case L4:
				ShiftData(16);
			break;
			case L5:
				ShiftData(32);
			break;
			case L6:
				ShiftData(64);
			break;
			case L7:
				ShiftData(128);
			break;
		
			default:
			ShiftData(0x00);
		}
	
		switch(led_state){
			case INIT:
				led_state = L0;
			break;
			case L0:
				led_state = L1;
			break;
			case L1:
				led_state = L2;
			break;
			case L2:
				led_state = L3;
			break;
			case L3:
				led_state = L4;
			break;
			case L4:
				led_state = L5;
			break;
			case L5:
				led_state = L6;
			break;
			case L6:
				led_state = L7;
			break;
			case L7:
				led_state = L0;
			break;
		
			default:
				led_state = L0;
			break;
		}
	}
	
	if (count1 == 2 && flag1 == 1)
	{
		//Actions
		switch(led_festive){
			case INIT1:
			ShiftData(0x0F);
			break;
			case L0F:
			ShiftData(0x0F);
			break;
			case LF0:
			ShiftData(0xF0);
			break;
		
			default:
			ShiftData(0x0F);
		}
	
		switch(led_festive){
			case INIT:
			led_festive = L0F;
			break;
			case L0F:
			led_festive = LF0;
			break;
			case LF0:
			led_festive = L0F;
			break;
		
			default:
			led_festive = L0F;
			break;
		}
	}
	if (count1 == 3 && flag1 == 1)
	{
		//Actions
		switch(led_festive1){
			case INIT2:
				ShiftData(0x05);
			break;
			case L05:
				ShiftData(0x05);
			break;
			case L0A:
				ShiftData(0x0A);
			break;
			case L41:
				ShiftData(0x14);
			break;
			case L28:
				ShiftData(0x28);
			break;
			case L50:
				ShiftData(0x50);
			break;
			case LA0:
				ShiftData(0xA0);
			break;
		
		
			default:
			ShiftData(0x05);
		}
	
		switch(led_festive1){
			case INIT:
				led_festive1 = L05;
			break;
			case L05:
				led_festive1 = L0A;
			break;
			case L0A:
				led_festive1 = L41;
			break;
			case L41:
				led_festive1 = L28;
			break;
			case L28:
				led_festive1 = L50;
			break;
			case L50:
				led_festive1 = LA0;
			break;
			case LA0:
				led_festive1 = L05;
			break;
		
			default:
				led_festive1 = L05;
			break;
		}
	}
}


void LedSecTask() {
	LEDS_Init();
	for(;;) {
		LEDS_Tick(count, flag);
		vTaskDelay(200);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask,
	(signed portCHAR *)"LedSecTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}

void buttonSecTask() {
	for(;;) {
		button_check();
		vTaskDelay(100);
	}
}

void StartbuttonPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(buttonSecTask,
	(signed portCHAR *)"buttonSecTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}

int main(void)
{
    DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	LEDS_Init();
	
    StartSecPulse(1);
	StartbuttonPulse(2);
    //RunSchedular
    vTaskStartScheduler();
}

