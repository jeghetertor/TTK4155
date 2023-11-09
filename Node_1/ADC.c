#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef ADC_H_
#include "ADC.h"
#endif 

void ADC_init(void){
	//DDRD |= (1<<PD4);
	//TCCR3B |= (1 << CS30) | (1 << WGM32);
	//TCCR3A |= (1 << COM3A0) | (1 << COM3B0);
	DDRB |= (1<<PB0);
	TCCR0 |= (1 <<WGM01) | (1 << COM00) | (1 << CS00);
	//OCR0 = 0;
	
	// ALTERNATE SOLUTION
	/*char sreg = SREG;
	cli();
	unsigned char duty;
	duty = 10;
	
	TCCR3A = (1<<WGM31) | (1<<COM3A1) | (1<<CS30);
	TCCR3A |= (1<<COM3A1 | 1<<WGM31);
	TCCR3A &= ~(1<<COM3A0);
	TCCR3B &= ~(1<<WGM33 | 0b111 << CS30);
	TCCR3B |= (1<<WGM32 |1<< WGM33 | 1<<CS30);
	TCCR3B |= (0<<CS32);
	ICR3 = 2*duty;
	
	
	//TCCR3A |= (1<<COM3A1 | 1<<WGM31);
	//TCCR3A &= ~(1<<COM3A0);
	//TCCR3B &= ~(1<<WGM33 | 0b111 << CS30);
	//TCCR3B |= (1<<WGM32 |1<< WGM33 | 0b01<<CS30);
	//ICR3 = 2*10;
	SREG = sreg;
	while(TCNT1);
	*/
	//OCR3A = duty;
	//DDD5 = 1;
	//sei();
}
struct JoyAngle read_joy_angle(void){
	volatile char *adc_mem = (char *) 0x1400;
	adc_mem[0] = 0x04;
	struct JoyAngle angles;
	_delay_ms(10);
	uint8_t x_axis = adc_mem[0];
	uint8_t y_axis = adc_mem[1];
	angles.x = 100*adc_mem[0]/255;
	angles.y = 100*adc_mem[1]/255;
	return angles;
}


//void ADC_PWM_clock_init(){}
struct Sliders read_slider(void){
	volatile char *adc_mem = (char *) 0x1400;
	adc_mem[0] = 0x04;
	struct Sliders slider;
	_delay_ms(10);
	uint8_t l_slider = adc_mem[2];
	uint8_t r_slider = adc_mem[3];
	slider.left = 100*adc_mem[2]/255;
	slider.right = 100*adc_mem[3]/255;
	return slider;
}
struct ADC  ADC_read(void){
	volatile char *adc_mem = (char *) 0x1400;
	adc_mem[0] = 0x04;

	struct ADC adc;
	

	adc.x_axis = adc_mem[0];
	adc.y_axis = adc_mem[1];

	adc.l_slider = adc_mem[2];
	adc.r_slider = adc_mem[3];
	return adc;
}

struct ADC ADC_output(struct Offset_const offset_const){
	struct ADC adc = ADC_read();
	int max_x = 255 - offset_const.offset_x;
	int min_x = - offset_const.offset_x;
	int max_y = 255 - offset_const.offset_y;
	int min_y = - offset_const.offset_y;

	if(adc.x_axis>=offset_const.offset_x){ //brainlet løsning
		adc.x_axis = (100*(adc.x_axis-offset_const.offset_x)/(max_x));
	}else if (adc.x_axis<offset_const.offset_x){
		
		adc.x_axis = (100*(adc.x_axis-offset_const.offset_x)/(-min_x));
	};
	if(adc.y_axis>=offset_const.offset_y){
		adc.y_axis = (100*(adc.y_axis-offset_const.offset_y)/(max_y));
		}else if (adc.y_axis<offset_const.offset_y){
			
		adc.y_axis = (100*(adc.y_axis-offset_const.offset_y)/(-min_y));
	};
	return adc;
}
int max(int x, int y){
	if(x > y){
		return x;
		}else{
		return y;
	}
}

struct Offset_const ADC_calibration(void){
	struct ADC adc = ADC_read();
	struct Offset_const offset_consts;
	offset_consts.offset_x = adc.x_axis;
	offset_consts.offset_y = adc.y_axis;
	printf("VALUES READ! x:%d y:%d\n", adc.x_axis, adc.y_axis);
	printf("CALIBRATED! x:%d y:%d\n", offset_consts.offset_x, offset_consts.offset_y);
	return offset_consts;
}

Joy_Mode Joy_direction(struct Offset_const offset_const, struct ADC adc){
	Joy_Mode joy_dir;
	int neutral_threshold = 25;
	
	if(neutral_threshold >= abs(adc.x_axis) &&
		neutral_threshold >= abs(adc.y_axis)
		){
		joy_dir = NEUTRAL;
		return joy_dir; // enum neutral
	}
	
	int max_1 = max(abs(adc.x_axis), abs(adc.y_axis));
	if(max_1 == abs(adc.x_axis)){
		if(adc.x_axis < 0){
			joy_dir = LEFT;
			return joy_dir; // LEFT
		}
		joy_dir = RIGHT;
		return joy_dir; // RIGHT
	}
	if(adc.y_axis < 0){
		joy_dir = DOWN;
		return joy_dir; // DOWN
	}
	joy_dir = UP;
	return joy_dir;
}

