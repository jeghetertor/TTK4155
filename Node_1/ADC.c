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
struct ADC  ADC_read(void){
	volatile char *adc_mem = (char *) 0x1400;
	adc_mem[0] = 0x04;
	//_delay_ms(1);
	//adc_mem[1] = 0x01;
	//_delay_ms(1);
	//adc_mem[2] = 0x02;
	//_delay_ms(1);
	//adc_mem[3] = 0x03;
	//_delay_ms(50);
	struct ADC adc;
	
	/*uint8_t x_axis = adc_mem[0];
	uint8_t y_axis = adc_mem[1];
	uint8_t l_slider = adc_mem[2];
	uint8_t r_slider = adc_mem[3];*/
	adc.x_axis = adc_mem[0];
	adc.y_axis = adc_mem[1];

	adc.l_slider = adc_mem[2];
	adc.r_slider = adc_mem[3];
	return adc;
	//printf("0- %02X 1- %02X 2- %02X 3- %02X\n",retreived_value1,retreived_value2,retreived_value3,retreived_value4);
	//printf("x- %03d y- %03d l_slider- %03d r_slider- %03d\n", x_axis, y_axis, l_slider, r_slider);
	//printf("v_l %f v_r %f\n",max_voltage, max_dec);
	
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
	struct Offset_const offset_init;
	offset_init.offset_x = 0;
	offset_init.offset_y = 0;
	struct ADC adc = ADC_read();
	struct Offset_const offset_consts;
	offset_consts.offset_x = adc.x_axis;
	offset_consts.offset_y = adc.y_axis;
	return offset_consts;
}

Joy_Mode Joy_direction(struct Offset_const offset_const){
	Joy_Mode joy_dir;
	struct ADC adc = ADC_output(offset_const); // move this out. Take this as argument
	int neutral_threshold = 3;
	
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
