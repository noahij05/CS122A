/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 4 Exercise 2
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
enum LEDState1 {INIT,L0,L2} led_state1;

void LEDS_Init1(){
	led_state1 = INIT;
}

void LEDS_Tick1(){
	//Actions
	switch(led_state1){
		case INIT:
            PORTD = 0;
            break;
		case L0:
            PORTD = 1;
            break;
		case L2:
            PORTD = 0;
            break;
		/*case L4:
            PORTD = 16;
            break;*/
		default:
            PORTD = 0;
            break;
	}
	//Transitions
	switch(led_state1){
		case INIT:
			led_state1 = L0;
            break;
		case L0:
			led_state1 = L2;
            break;
		
		case L2:
			led_state1 = L0;
            break;
		/*	
		case L4:
			led_state = L0;
            break;*/
		default:
			led_state1 = INIT;
            break;
	}
}

enum LEDState2 {INITT,L00,L22} led_state2;

void LEDS_Init(){
	led_state2 = INITT;
}

void LEDS_Tick2(){
	//Actions
	switch(led_state2){
		case INITT:
            PORTD = 0;
            break;
		case L00:
            PORTD = 4;
            break;
		case L22:
            PORTD = 0;
            break;
		/*case L4:
            PORTD = 16;
            break;*/
		default:
            PORTD = 0;
            break;
	}
	//Transitions
	switch(led_state2){
		case INITT:
			led_state2 = L00;
            break;
		case L00:
			led_state2 = L22;
            break;
		
		case L22:
			led_state2 = L00;
            break;
		/*	
		case L4:
			led_state = L0;
            break;*/
		default:
			led_state2 = INITT;
            break;
	}
}

void LedSecTask() {
    LEDS_Init();
    for(;;) { 	
        LEDS_Tick1();
        vTaskDelay(1000); 
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


