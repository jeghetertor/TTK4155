#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "UART.h"
#include "ADC.h"
#include "OLED.h"
#include "SRAM.h"
#include "Menu.h"
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"
#include "MCP_register_definitions.h"
#define DEBOUNCE_CONST 0
#define F_CPU 4915200UL
#define FOSC 4915200UL
#define BAUD 9600UL
#define MYUBRR (FOSC/(16UL*BAUD))-1

CAN_message msg;

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


void print_something(){
	printf("HELLOOOO\n");
}

int main(void){
	cli();
	_delay_ms(200);
	SRAM_init();
	ADC_init();
	UART_init(MYUBRR);
	printf("\nInit program...\n");
	oled_init();
	oled_page_addressing();
	oled_reset();
	oled_home();
	mcp_init();

	CAN_init();
	sei();
	
	uint8_t ID_lowerbyte;
	uint8_t ID_higherbyte;
	
	ID_lowerbyte = mcp_read(MCP_RXB0SIDL);
	ID_higherbyte = mcp_read(MCP_RXB0SIDH);
	printf("HIGHRX MAIN %0x, LOWRX %0x\n", ID_higherbyte, ID_lowerbyte);
			
	//CAN_test();
	uint8_t flag;
	CAN_message my_msg = {
		0,
		1,
		't'
	};
	
	//while(1){
	//	CAN_transmit(&my_msg, 0);
	//	printf("%d\n", my_msg.data[0]);
	//	_delay_ms(100);
	//}



	
	// INITIATING MENU ELEMENTS
	menu_element* init_menu = malloc(sizeof(menu_element));

	menu_element* menu_play = add_menu_element(init_menu, "Play game",NULL);
	menu_element* menu_settings = add_menu_element(init_menu, "Settings",NULL);
	
	menu_element* menu_setting1 = add_menu_element(menu_settings, "Setting 1",NULL);
	menu_element* menu_setting2 = add_menu_element(menu_settings, "Invert Colors",NULL);
	
	menu_element* menu_setting_light = add_menu_element(menu_setting2, "Light Mode", oled_setting_light_mode);
	menu_element* menu_setting_dark = add_menu_element(menu_setting2, "Dark Mode", oled_setting_dark_mode);

	menu_element* menu_setting3 = add_menu_element(menu_settings, "Setting 3",print_something);
	
	menu_element* menu_game1 = add_menu_element(menu_play, "Game 1",NULL);
	menu_element* menu_game2 = add_menu_element(menu_play, "Game 2",NULL);
	
	menu_element* menu_under1 = add_menu_element(menu_game1, "Under 1",NULL);
	menu_element* menu_under2 = add_menu_element(menu_game2, "Under 2",NULL);
	
	menu_element* current  = init_menu;
	
	uint8_t max_menu_elements = current->num_children;
	
	_delay_ms(100);

	unsigned long iteration = DEBOUNCE_CONST;
	const struct Offset_const offset_const = ADC_calibration();
	bool oled_inverted_colors = false;
	//struct ADC adc;
	struct ADC adc;
	adc = ADC_output(offset_const);
	uint8_t joy_direction = Joy_direction(offset_const);

	uint8_t current_line_number = 0;	

	while(1){ // game loop
			adc = ADC_output(offset_const);
			
			if(Joy_direction(offset_const) != joy_direction
				& iteration > DEBOUNCE_CONST){
				iteration = 0;
				oled_reset();
				joy_direction = Joy_direction(offset_const);

				 // new frame
				current_line_number = move_arrow(current_line_number, joy_direction, max_menu_elements);

				oled_home(); // reset cursor
				const uint8_t LINES = 8;
				
				for(int i=0; i<LINES; i++){
					if(current->children[i] != NULL){
						oled_pos(i,20);
						oled_print("%s", current->children[i]->text);
					}
				}
				switch(joy_direction){
				case(RIGHT): // Right
					// if current menu item has sub menus
					if(current->children[current_line_number]->function != NULL){
						current->children[current_line_number]->function();
					}
					if(current->children[current_line_number]->num_children > 0){
						current  = current->children[current_line_number];
						current_line_number = 0;
						max_menu_elements = current->num_children;
					}
					
					break;
				
				// go back
				case(LEFT): // Left
					if(current->parent != NULL){
						current = current->parent;
						current_line_number = 0;
						max_menu_elements = current->num_children;
					}
				}
				}
			my_msg.length = 2;
			my_msg.data[0] = (int8_t) adc.x_axis;
			my_msg.data[1] = (int8_t) adc.y_axis;
			CAN_transmit(&my_msg,0);
			
			printf("data: %d\n", (int8_t) my_msg.data[0]);
			flag = mcp_read(MCP_CANINTF);
			if(flag&MCP_RX0IF){
				printf("buffer0");
				CAN_receive(0,&msg);
				
			}
			else if(flag&MCP_RX1IF){
				printf("buffer1");
				CAN_receive(1,&msg);
				//printf("data: %0x",msg.data[0]);
				//printf("data: %0x",msg.data[0]);
				//printf("data: %0x %0x %0x \n",msg.data[0],msg.data[1],msg.data[2]);
			}
			 // Used for debounce timer

			iteration++;

	// TEST WRITING TO SPECIFIC PINS
	//bool pb1Val = PINB & (1 << PINB1);
	//bool pb2Val = PINB & (1 << PINB2);
	//printf("pb1: %1d pb2: %1d\n", pb1Val, pb2Val);
	//printf("Fin\n");
	}
	return 0;
}

ISR(INT0_vect){
	CAN_receive(1, &msg);
	printf("%d\n", msg.ID);
	
	mcp_bit_modify(MCP_CANINTF, MCP_RX1IF, 0);
	mcp_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	//_delay_ms(100);
}