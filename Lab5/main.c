#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "UART.h"
#include "ADC.h"
#include "OLED.h"
#include "SRAM.h"
#include "Menu.h"


#define F_CPU 4915200UL
#define FOSC 4915200UL
#define BAUD 9600UL
#define MYUBRR (FOSC/(16UL*BAUD))-1



struct JoyAngle {
	uint8_t x;
	uint8_t y;
	};
struct Sliders {
	uint8_t left;
	uint8_t right;
	};


void latch_test(void){
	uint8_t byte = 0x1800;
	DDRA = byte;
	DDRE = 0b10;
	PORTE = 0b10;
	PORTA = byte;
	int i = 0;
	while(1){
		PORTA = byte;
		_delay_ms(5000);
		PORTA = 0b0;
		_delay_ms(5000);
		PORTA = byte;
		i++;
		if(i == 5){
			PORTE = 0b0;
		}
	}
}
void ADC_PWM_clock_init(void){
	DDRD |= (1<<PD4);
	TCCR3B |= (1 << CS30) | (1 << WGM32);
	TCCR3A |= (1 << COM3A0) | (1 << COM3B0);
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
/*
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
	uint8_t r_slider = adc_mem[3];*
	adc.x_axis = adc_mem[0];
	adc.y_axis = adc_mem[1];
	adc.l_slider = adc_mem[2];
	adc.r_slider = adc_mem[3];
	return adc;
	//printf("0- %02X 1- %02X 2- %02X 3- %02X\n",retreived_value1,retreived_value2,retreived_value3,retreived_value4);
	//printf("x- %03d y- %03d l_slider- %03d r_slider- %03d\n", x_axis, y_axis, l_slider, r_slider);
	//printf("v_l %f v_r %f\n",max_voltage, max_dec);
	
} */
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



	
int main(void){
	
	_delay_ms(200);
	SRAM_init();
	ADC_init();
	UART_init(MYUBRR);
	printf("Init program...\n");
	oled_init();
	oled_page_addressing();
	struct Arrow_state arrowPos;
	arrowPos.i = 0;
	
	menu_ptr init_menu = malloc(sizeof(menu_element));
	menu_ptr menu_play = add_menu_element(init_menu, "Play game");
	menu_ptr menu_settings = add_menu_element(init_menu, "Settings");
	
	menu_ptr menu_setting1 = add_menu_element(menu_settings, "Setting 1");
	menu_ptr menu_setting2 = add_menu_element(menu_settings, "Setting 2");
	menu_ptr menu_setting3 = add_menu_element(menu_settings, "Setting 3");
	
	menu_ptr menu_game1 = add_menu_element(menu_play, "Game 1");
	menu_ptr menu_game2 = add_menu_element(menu_play, "Game 2");
	
	menu_ptr menu_under1 = add_menu_element(menu_game1, "Under 1");
	menu_ptr menu_under2 = add_menu_element(menu_game2, "Under 2");
	
	menu_ptr current  = (menu_ptr*)init_menu;
	
	uint8_t max = current->num_children;
	
	_delay_ms(100);

	printf("Test\n");
	
	struct Offset_const offset_const = ADC_calibration();
	//struct ADC adc;
	uint8_t joy_direction = Joy_direction(offset_const);
	while(1){ // game loop
		if(Joy_direction(offset_const) != joy_direction){
		oled_reset();
		joy_direction = Joy_direction(offset_const);

		 // new frame
		arrowPos = move_arrow(arrowPos.i, joy_direction, max);

		oled_home();
		
		for(int i=0; i<8; i++){
			if(current->children[i] != NULL){
				oled_pos(i,20);
				printf("%s", current->children[i]->text);
				oled_print("%s", current->children[i]->text);
				_delay_ms(100);
			}
		}

		if(joy_direction == 2){
			if(current->children[arrowPos.i]->num_children > 0){
			current  = current->children[arrowPos.i];
			arrowPos.i = 0;
			max = current->num_children;
			}
			
		}
		if(joy_direction == 1){
			if(current->parent != NULL){
			current = current->parent;
			arrowPos.i = 0;
			max = current->num_children;
			}
		}
		


		_delay_ms(500);
		}
	}

	
	
	// TEST WRITING TO SPECIFIC PINS
	//bool pb1Val = PINB & (1 << PINB1);
	//bool pb2Val = PINB & (1 << PINB2);
	//printf("pb1: %1d pb2: %1d\n", pb1Val, pb2Val);
	//printf("Fin\n");
	
	return 0;
}