#ifndef MENU_H_
#include "Menu.h"
#endif
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
#include "SPI.h"
#include "MCP2515.h"
#include "CAN.h"
#include "MCP_register_definitions.h"
#define DEBOUNCE_CONST 0



void print_something(){
	printf("HELLOOOO\n");
}

menu_element* add_menu_element(menu_element* parent, char * text, void (* function)()){
	menu_element* new_element = malloc(sizeof(menu_element));
	new_element->text = text;
	new_element->parent = parent;
	new_element->function = function;
	int num_child = 0;
	while(parent->children[num_child] != NULL){
		num_child++;
	}
	parent->num_children++;
	parent->children[num_child] = new_element;
	return new_element;
}



void play_game1(){
	long score = 0;
	bool game_running = true;
	oled_reset();
	oled_pos(3, 0);
	oled_print("Dont");
	oled_pos(4, 0);
	oled_print("touch");
	oled_pos(6, 0);
	oled_print("joystick!");
	_delay_ms(20000);

	CAN_message reset_msg = {
		1,
		1,
		'r'
	};
	CAN_message msg;
	CAN_transmit(&reset_msg,0);
	oled_reset();
	CAN_message my_msg = {
		0,
		1,
		't'
	};
	
	struct ADC adc = ADC_read();
	uint8_t flag;
	struct Offset_const offset_const = ADC_calibration(); // Calibrating ADC
	while(game_running){
		//struct ADC adc_raw = ADC_read();
		//printf("x_raw %d y_raw %d\n", adc_raw.x_axis, adc_raw.y_axis);
		bool exit = PINB & (1 << PINB2);
		
		adc = ADC_output(offset_const);
		CAN_receive(1,&msg);
		oled_pos(3, 10);
		oled_print("Lives: %d", 5-msg.data[0]);
		oled_pos(1, 10);
		oled_print("Score: %d", (int) (score/10));
		oled_pos(7, 2);
		oled_print("Stop");
		my_msg.length = 3;
		my_msg.data[0] = adc.x_axis;
		my_msg.data[1] = adc.y_axis;
		my_msg.data[2] = adc.r_slider;
		CAN_transmit(&my_msg,0);
		//printf("x:%d \n" adc.x_axis);
		//printf("data: %d\n",adc.x_axis);
		flag = mcp_read(MCP_CANINTF);
		if(flag&MCP_RX0IF){
			//printf("buffer0 receive\n");
			CAN_receive(0,&msg);
			
		}
		else if(flag&MCP_RX1IF){
			//printf("buffer1 receive\n");
			CAN_receive(1,&msg);
			//printf("%d",msg.data[0]);
		}
		if((msg.data[0] > 4)|exit){
			game_running = false;
		}
		score ++;
		//printf("%d\n", adc.x_axis);
		//printf("Offset const: %d \n", offset_const.offset_x);
	}
	
	oled_reset();
	oled_pos(1, 10);
	oled_print("You lose!");
	oled_pos(4, 10);
	oled_print("Final score:");
	oled_pos(6, 10);
	oled_print("%dpts.", score);
	_delay_ms(20000);
	CAN_transmit(&reset_msg,0);
}

void main_menu(){
	const struct Offset_const offset_const = ADC_calibration(); // Calibrating ADC
	//CAN_test();
	uint8_t flag;
	CAN_message msg;
	CAN_message my_msg = {
		0,
		1,
		't'
	};
	CAN_message difficulty_msg = {2,1,0};
	// INITIATING MENU ELEMENTS
	menu_element* init_menu = malloc(sizeof(menu_element));

	menu_element* menu_play = add_menu_element(init_menu, "Play game",play_game1);
	menu_element* menu_settings = add_menu_element(init_menu, "Settings",NULL);
	
	//menu_element* menu_setting1 = add_menu_element(menu_settings, "Setting 1",NULL);
	menu_element* menu_setting2 = add_menu_element(menu_settings, "Invert Colors",NULL);
	
	menu_element* menu_setting_light = add_menu_element(menu_setting2, "Light Mode", oled_setting_light_mode);
	menu_element* menu_setting_dark = add_menu_element(menu_setting2, "Dark Mode", oled_setting_dark_mode);

	//menu_element* menu_setting3 = add_menu_element(menu_settings, "Setting 3",print_something);
	
	//menu_element* menu_game1 = add_menu_element(menu_play, "Start game",NULL);
	menu_element* menu_game_settings = add_menu_element(menu_settings, "Game settings",NULL);

	menu_element* menu_game_settings_difficulty = add_menu_element(menu_game_settings, "Difficulty", NULL);	
	
	menu_element* empty_element = add_menu_element(menu_game_settings_difficulty, "",NULL);	
	
	
	menu_element* current  = init_menu;
	
	uint8_t max_menu_elements = current->num_children;


	unsigned long iteration = DEBOUNCE_CONST;
	
	bool oled_inverted_colors = false;
	int8_t difficulty = 1;
	bool difficulty_change = false;
	//struct ADC adc;
	struct ADC adc;
	adc = ADC_output(offset_const);
	uint8_t joy_direction = Joy_direction(offset_const, adc);

	uint8_t current_line_number = 0;

	
	
	while(1){ // game loop
		struct ADC adc_raw = ADC_read();
		//printf("x_raw %d y_raw %d\n", adc_raw.x_axis, adc_raw.y_axis);
		adc = ADC_output(offset_const);
		//printf("Joy dir: %d  Xaxis: %d \n", joy_direction, adc.x_axis);
		
		if((Joy_direction(offset_const, adc) != joy_direction && iteration >= DEBOUNCE_CONST) ||
		iteration == DEBOUNCE_CONST
		){
			iteration = 0;
			difficulty_change = true;
			oled_reset();
			joy_direction = Joy_direction(offset_const, adc);

			// new frame
			current_line_number = move_arrow(current_line_number, joy_direction, max_menu_elements);
			

			oled_home(); // reset cursor
			const uint8_t LINES = 8;
			
			
			
			for(int i=0; i<LINES; i++){
				if(current->children[i] != NULL){
					oled_pos(i+2,20);
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
		if(current->text == "Difficulty"){	
			bool plus = PINB & (1 << PINB1);
			bool minus = PINB & (1 << PINB2);
			if((difficulty<5) & (plus)){
				difficulty++;
				difficulty_change = true;
			}
			if((minus) & (difficulty>1)){
				difficulty--;
				difficulty_change = true;
			}
			if(difficulty_change == true){
				oled_reset();
				oled_pos(3, 0);
				oled_print("Difficulty: %d", difficulty);
				
				oled_pos(7, 2);
				oled_print("-");
				
				oled_pos(7, 120);
				oled_print("+");
				_delay_ms(200);
				difficulty_change = false;
				difficulty_msg.data[0] = difficulty;
				CAN_transmit(&difficulty_msg, 0);
			}
		}
		else{
			difficulty_change = true;
		}
		// Used for debounce timer
		iteration++;

	}
}