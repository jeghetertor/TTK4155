#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef CMD_ADDRESS
#define CMD_ADDRESS  0x1000
#endif
#ifndef DATA_ADDRESS
#define DATA_ADDRESS  0x1200
#endif
#ifndef FONTS_H_
#include "fonts.h"
#endif
#ifndef OLED_H_
#include "OLED.h"
#endif


static FILE mystdout = FDEV_SETUP_STREAM(oled_print_char,NULL,_FDEV_SETUP_WRITE);

static volatile char *ext_ram_ = (char *) 0x1800;
//volatile char *oled_data_ext_ram = (char *) DATA_ADDRESS;
//Write command to OLED
void oled_write_cmd(uint8_t cmd){
	volatile char *oled_cmd_ext_ram = (char *) 0x1000;
	oled_cmd_ext_ram[0] = cmd;
}
//Write data to OLED
void oled_write_data(uint8_t data){
	volatile char *oled_data_ext_ram = (char *) 0x1200;
	
	oled_data_ext_ram[0] = data;
}

void oled_goto_line(uint8_t line){
	oled_write_cmd(0xb0+line);					// Line = Segment in this prog.
}

void oled_goto_column(uint16_t col){
	//uint8_t lower_nibble = col & 0b00001111;// Calculate lower nibble
	//uint8_t higher_nibble = (col>>4) + 16;	// Calculate higher nibble
	// Sets the page
	//higher_nibble = (0x10 + (col / 16));
	//lower_nibble = (0x00 + (col % 16));
	oled_write_cmd(col & 0b00001111);			// Sets the lower column start address
	oled_write_cmd((col>>4) + 16);			// Sets the higher column start address
	//oled_write_cmd(0x21);
	//oled_write_cmd(col);
	//oled_write_cmd(0xff);
}
//OLED position
void oled_page_addressing(void){
	oled_write_cmd(0x20);	// SET PAGE SEL.
	oled_write_cmd(0b10);
}
void oled_pos(uint8_t row, uint8_t col){
	oled_goto_line(row);
	oled_goto_column(col);
}

void oled_print_arrow(uint8_t row, uint8_t col){
	oled_pos(row,col);
	oled_write_data(0b00111100);
	oled_write_data(0b01100110);
	oled_write_data(0b11000011);
	oled_write_data(0b10011001);
	oled_write_data(0b10011001);
	oled_write_data(0b11000011);
	oled_write_data(0b01100110);
	oled_write_data(0b00111100);
	

	
}

void oled_reset(void){
	//oled_page_addressing();
	for(uint8_t i = 0; i <= 7; i++){

		oled_goto_line(i);
		oled_goto_column(0);
		for(uint8_t j = 0; j <= 127; j++){
			//oled_pos(i,j);
			oled_write_data(0x00);	// LIGHT UP
		}
	}
}

void oled_print_char(char character){

	char var = character - 32;
	char buffer;
	for(uint8_t i = 0; i < 8; i++){
		buffer = pgm_read_byte(&(font8[var][i]));
		oled_write_data(buffer);
	}

}

void oled_home(void){
	oled_pos(0,0);
}

//OLED init
uint8_t move_arrow(uint8_t current_line_number, Joy_Mode joy_direction, uint8_t max_menu_elements){
	uint8_t forward = 0;
	uint8_t backward = 0;
	
	switch(joy_direction){

		case UP: // UP
			if(current_line_number != 0){
				current_line_number--;
			}
			break;
		case DOWN:
			if( current_line_number != max_menu_elements - 1 ){
				current_line_number++;
			}
		break;
	}
	oled_print_arrow(current_line_number+2, 2);
	// struct Arrow_state arrow_state;
	// arrow_state.i = current_line_number;
	// arrow_state.forward = forward;
	// arrow_state.backward = backward;
	return current_line_number;
}

void oled_print(char* string, ... ){
	//FILE* temp = stdout; //temp location for stdout
	//stdout = &mystdout;	//change to oled printing
	va_list args;
	va_start(args, string);
	vfprintf(&mystdout, string, args);
	//printf(string);
	va_end(args);
	//stdout = temp; //change back to terminal printing
}

void oled_init(void)
{
	oled_write_cmd(0xae); // display off
	oled_write_cmd(0xa1); //segment remap
	oled_write_cmd(0xda); //common pads hardware: alternative
	oled_write_cmd(0x12);
	oled_write_cmd(0xc8); //common output scan direction:com63~com0
	oled_write_cmd(0xa8); //multiplex ration mode:63
	oled_write_cmd(0x3f);
	oled_write_cmd(0xd5); //display divide ratio/osc. freq. mode
	oled_write_cmd(0x80);
	oled_write_cmd(0x81); //contrast control
	oled_write_cmd(0x50);
	oled_write_cmd(0xd9); //set pre-charge period
	oled_write_cmd(0x21);
	oled_write_cmd(0x20); //Set Memory Addressing Mode
	oled_write_cmd(0x02);
	oled_write_cmd(0xdb); //VCOM deselect level mode
	oled_write_cmd(0x30);
	oled_write_cmd(0xad); //master configuration
	oled_write_cmd(0x00);
	oled_write_cmd(0xa4); //out follows RAM content
	oled_write_cmd(0xa7); //set normal display
	oled_write_cmd(0xaf); // display on	
}
	
void oled_setting_dark_mode(){
	oled_write_cmd(0xa6);
}

void oled_setting_light_mode(){
	oled_write_cmd(0xa7);
}