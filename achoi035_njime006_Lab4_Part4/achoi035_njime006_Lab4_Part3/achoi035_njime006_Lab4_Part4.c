/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 4 Exercise 1
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
enum LEDState {INIT,L0,L1,L2,L3,L4,L5,L6,L7,L22,L33,L44,LOP,L55,L66,L77} led_state;

void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		PORTD = 0;
		break;
		case L0:
		PORTD = 1;
		break;
		case L1:
		PORTD = 2;
		break;
		case L2:
		PORTD = 4;
		break;
		case L3:
		PORTD = 8;
		break;
		case L4:
		PORTD = 16;
		break;
		case L5:
		PORTD = 32;
		break;
		case L6:
		PORTD = 64;
		break;
		case L7:
		PORTD = 128;
		break;
		case L22:
		PORTD = 64;
		break;
		case L33:
		PORTD = 32;
		break;
		case L44:
		PORTD = 16;
		break;
		case LOP:
		PORTD = 8;
		break;
		case L55:
		PORTD = 4;
		break;
		case L66:
		PORTD = 2;
		break;
		case L77:
		PORTD = 1;
		break;
		default:
		PORTD = 0;
		break;
	}
	//Transitions
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
			led_state = L22;
		break;
		
		case L22:
		led_state = L33;
		break;
		case L33:
		led_state = L44;
		break;
		case L44:
		led_state = LOP;
		break;
		case LOP:
		led_state = L55;
		break;
		case L55:
		led_state = L66;
		break;
		case L66:
		led_state = L77;
		break;
		case L77:
		led_state = L0;
		break;
		default:
		led_state = INIT;
		break;
	}
}

void LedSecTask() {
    LEDS_Init();
    for(;;) { 	
        LEDS_Tick();
        vTaskDelay(10); 
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
 
int main(void) { 
    DDRA = 0x00; PORTA=0xFF;
    DDRD = 0xFF;
    //Start Tasks  
    StartSecPulse(1);
    //RunSchedular 
    vTaskStartScheduler(); 
    return 0; 
}


