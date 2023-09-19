#include <avr/io.h>
#include <stdio.h>
#ifndef CMD_ADDRESS
#define CMD_ADDRESS  0x1000
#endif
#ifndef DATA_ADDRESS
#define DATA_ADDRESS  0x1200
#endif
//#ifndef FONTS_H_
//#include "fonts.h"
//#endif


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

void oled_goto_column(uint8_t col){
		uint8_t lower_nibble = col & 0b00001111;// Calculate lower nibble
		uint8_t higher_nibble = (col>>4) + 16;	// Calculate higher nibble
		// Sets the page
		oled_write_cmd(lower_nibble);			// Sets the lower column start address
		oled_write_cmd(higher_nibble);			// Sets the higher column start address
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
	oled_write_data(0b00011000);
	oled_write_data(0b00011000);
	oled_write_data(0b01111110);
	oled_write_data(0b00111100);
	oled_write_data(0b00011000);
	
}

void oled_reset(void){
	for(uint8_t i = 0; i <= 7; i++){
		oled_goto_line(i);
		oled_goto_column(0);
		for(uint8_t j = 0; i <= 127; j++){
			oled_write_data(0b0);	// LIGHT UP
		}
	}
}

///void oled_print(char* character){
///	char var = character - 32;
///	char buffer[8];
///	for(uint8_t i = 0; i < 8; i++){
///		//strcpy_P(buffer, (PGM_P)pgm_read_word(&(font8[var][i])));
///		oled_write_data(buffer[i]);
///	}
///}

//OLED init
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
	oled_write_cmd(0xa6); //set normal display
	oled_write_cmd(0xaf); // display on
}