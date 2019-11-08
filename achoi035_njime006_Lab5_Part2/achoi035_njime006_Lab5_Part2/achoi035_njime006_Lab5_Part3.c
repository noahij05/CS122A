/*	Name & E-mail: Alexander Choi, achoi035@ucr.edu
 *  Name & E-mail: Noah Jimenez, njime006@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab 5 Exercise 3
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */

#include <avr/io.h>
#include <stdlib.h>
#include "scheduler.h"

#define SER 0
#define RCLK1 2
#define SRCLK 3
#define SRCLR1 4
#define RCLK2 5
#define SRCLR2 6

#define UP 0x02
#define DOWN 0x01
#define UP_2 0x08
#define DOWN_2 0x04

#define MOLLY_LIGHTS 0
#define CURTAIN_LIGHTS 1
#define ALTERNATE_LIGHTS 2


unsigned char lights[] = {0, 0, 0, 0};
unsigned char light_1 = 0, light_2 = 0;
unsigned char prev_1 = 0, prev_2 = 0;
unsigned char inc1 = 0, dec1 = 0, clear1 = 0, inc2 = 0, dec2 = 0, clear2 = 0;

void transmit_data(unsigned char data, unsigned char id) {
	if(id == 0) {
		PORTC |= (1 << SRCLR1);
		PORTC &= ~(1 << RCLK1);
	} else if(id == 1) {
		PORTC |= (1 << SRCLR2);
		PORTC &= ~(1 << RCLK2);
	}
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
	if(id == 0) {
		PORTC |= (1 << RCLK1);
	} else if(id == 1) {
		PORTC |= (1 << RCLK2);
	}
	PORTC = 0;
}

enum ReadIN_states {Read_default, Read_up, Read_down, Read_both};

int ReadIN1_tick(int state) {
	unsigned char button_up = ~PINA & UP;
	unsigned char button_down = ~PINA & DOWN;
	
	switch(state) {
		default:
		case Read_default:
		if(button_up && button_down) {
			state = Read_both;
			clear1 = ~clear1;
			} else if(button_down) {
			state = Read_down;
			dec1 = 1;
			} else if(button_up) {
			state = Read_up;
			inc1 = 1;
			} else {
			state = Read_default;
		}
		break;
		case Read_both:
			if(button_up && button_down) {
				state = Read_both;
			} else if(button_down) {
				state = Read_down;
				dec1 = 1;
			} else if(button_up) {
				state = Read_up;
				inc1 = 1;
			} else {
				state = Read_default;
			}
		break;
		case Read_up:
			if(button_up && button_down) {
					state = Read_both;
					clear1 = ~clear1;
				} else if(button_down) {
					state = Read_down;
					dec1 = 1;
				} else if(button_up) {
					state = Read_up;
				} else {
					state = Read_default;
			}
		break;
		case Read_down:
			if(button_up && button_down) {
					state = Read_both;
					clear1 = ~clear1;
				} else if(button_down) {
					state = Read_down;
				} else if(button_up) {
					state = Read_up;
					inc1 = 1;
				} else {
				state = Read_default;
			}
		break;
	}
	return state;
}

int ReadIN2_tick(int state) {
	unsigned char button_up = ~PINA & UP_2;
	unsigned char button_down = ~PINA & DOWN_2;
	
	switch(state) {
		default:
		if(button_up && button_down) {
			state = Read_both;
			clear2 = ~clear2;
			} else if(button_down) {
			state = Read_down;
			dec2 = 1;
			} else if(button_up) {
			state = Read_up;
			inc2 = 1;
			} else {
			state = Read_default;
		}
		break;
		case Read_default:
		if(button_up && button_down) {
			state = Read_both;
			} else if(button_down) {
			state = Read_down;
			dec2 = 1;
			} else if(button_up) {
			state = Read_up;
			inc2 = 1;
			} else {
			state = Read_default;
		}
		break;
		case Read_both:
		if(button_up && button_down) {
			state = Read_both;
			} else if(button_down) {
			state = Read_down;
			dec2 = 1;
			} else if(button_up) {
			state = Read_up;
			inc2 = 1;
			} else {
			state = Read_default;
		}
		break;
		case Read_up:
		if(button_up && button_down) {
			state = Read_both;
			clear2 = ~clear2;
			} else if(button_down) {
			state = Read_down;
			dec2 = 1;
			} else if(button_up) {
			state = Read_up;
			} else {
			state = Read_default;
		}
		break;
		case Read_down:
		if(button_up && button_down) {
			state = Read_both;
			clear2 = ~clear2;
			} else if(button_down) {
			state = Read_down;
			} else if(button_up) {
			state = Read_up;
			inc2 = 1;
			} else {
			state = Read_default;
		}
		break;
	}
	return state;
}

int updateVal_tick(int state) {
	state = 0;
	
	if(inc1 && light_1 >= 2) {
		light_1 = 0;
	} else if(inc1 && light_1 < 2) {
		light_1++;
	} 
	if(dec1 && light_1 == 0) {
		light_1 = 2;
	} else if(dec1 && light_1 > 0) {
		light_1--;
	}
	if(clear1 && light_1 != 3) {
		prev_1 = light_1;
		light_1 = 3;
	} else if(clear1) {
		
		}else  if(light_1 == 3) {
		light_1 = prev_1;
	}
	
	inc1 = 0;
	dec1 = 0;
	
	
	if(inc2 && light_2 >= 2) {
		light_2 = 0;
		} else if(inc2 && light_2 < 2) {
		light_2++;
	}
	if(dec2 && light_2 == 0) {
		light_2 = 2;
		} else if(dec2 && light_2 > 0) {
		light_2--;
	}
	if(clear2 && light_2 != 3) {
		prev_2 = light_2;
		light_2 = 3;
	} else if(clear2) {
		
	}else if(light_2 == 3) {
		light_2 = prev_2;
	}
	
	inc2 = 0;
	dec2 = 0;
	
	return state;
}

int outPut_tick(int state) {
	state = 0;
	
	transmit_data(lights[light_1], 0);
	transmit_data(lights[light_2], 1);
	
	return state;
}

enum MollyLights_states {Molly_up, Molly_down, Molly_init};
	
int MollyLights_tick(int state) {
	switch(state) {
		default:
			state = Molly_init;
			break;
		case Molly_init:
			state = Molly_up;
			break;
		case Molly_up:
			if(lights[MOLLY_LIGHTS] == 0x80) {
				state = Molly_down;
			} else {
				state = Molly_up;
			}
			break;
		case Molly_down:
			if(lights[MOLLY_LIGHTS] == 0x01) {
				state = Molly_up;
			}  else {
				state = Molly_down;
			}
			break;
	}
	
	switch(state) {
		case Molly_init:
			lights[MOLLY_LIGHTS] = 0x01;
			break;
		case Molly_up:
			lights[MOLLY_LIGHTS] <<= 1;
			break;
		case Molly_down:
			lights[MOLLY_LIGHTS] >>= 1;
			break;
	}
	return state;
}

enum Curtain_states {Curtain_in_init, Curtain_in, Curtain_out_init, Curtain_out};
	
int Curtain_tick(int states) {
	switch(states) {
		default:
			states = Curtain_in_init;
			break;
		case Curtain_in_init:
			states = Curtain_in;
			break;
		case Curtain_in:
			if(lights[CURTAIN_LIGHTS] == 0xFF) {
				states = Curtain_out_init;
			} else {
				states = Curtain_in;
			}
			break;
		case Curtain_out_init:
			states = Curtain_out;
			break;
		case Curtain_out:
			if(lights[CURTAIN_LIGHTS] == 0x00) {
				states = Curtain_in_init;
			} else {
				states = Curtain_out;
			}
			break;
	}
	
	switch(states) {
		case Curtain_in_init:
			lights[CURTAIN_LIGHTS] = 0x81;
			break;
		case Curtain_out_init:
			lights[CURTAIN_LIGHTS] = 0xE7;
			break;
		case Curtain_in:
			lights[CURTAIN_LIGHTS] |= (lights[CURTAIN_LIGHTS] >> 1) | (lights[CURTAIN_LIGHTS] << 1);
			break;
		case Curtain_out:
			lights[CURTAIN_LIGHTS] = ((lights[CURTAIN_LIGHTS] & 0x0F) >> 1) | ((lights[CURTAIN_LIGHTS] & 0xF0) << 1);
			break;
	}

    return states;
}

enum Alternate_states {Alternate_init, Alternate_go};
	
int Alternate_tick(int state) {
	switch(state) {
		default:
			state = Alternate_init;
			break;
		case Alternate_go:
		case Alternate_init:
			state = Alternate_go;
			break;
	}
	
	switch(state){
		case Alternate_init:
			lights[ALTERNATE_LIGHTS] = 0x55;
			break;
		case Alternate_go:
			lights[ALTERNATE_LIGHTS] = ~lights[ALTERNATE_LIGHTS];
			break;
	}
	return state;
}

int main(void)
{
	DDRA = 0x00; PINA = 0xFF;
	DDRC = 0xFF; PINC = 0x00;
	
	struct task readIN1 = {-1, 50, 50, &ReadIN1_tick};
	struct task updateVal = {-1, 50, 50, &updateVal_tick};
	struct task outPut = {-1, 50, 50, &outPut_tick};
	struct task MollyLights = {-1, 100, 100, &MollyLights_tick};	
	struct task Curtain = {-1, 100, 100, &Curtain_tick};
	struct task Alternate = {-1, 150, 150, &Alternate_tick};
	struct task readIN2 = {-1, 50, 50, &ReadIN2_tick};
		
	tasksPeriodGCD = 50;
	
	tasksNum = 7;
	tasks = malloc(sizeof(struct task) * tasksNum);
	tasks[0] = readIN1;
	tasks[1] = updateVal;
	tasks[2] = outPut;
	tasks[3] = MollyLights;
	tasks[4] = Curtain;
	tasks[5] = Alternate;
	tasks[6] = readIN2;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}
