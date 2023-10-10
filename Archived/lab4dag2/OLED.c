
#include <stdio.h>
#ifndef CMD_ADDRESS
#define CMD_ADDRESS  0x1000
#endif
#ifndef DATA_ADDRESS
#define DATA_ADDRESS  0x1200
#endif
#ifndef FONTS_H_
#include "fonts.h"
#endif
void oled_print_char(char character);
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
	oled_write_data(0b00011000);
	oled_write_data(0b00011000);
	oled_write_data(0b01111110);
	oled_write_data(0b00111100);
	oled_write_data(0b00011000);
	oled_write_data(0b00010000);
	

	
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

//OLED init
int add(int a, int b){
	return a+b;
}
void oled_print(char* string){
	FILE* temp = stdout; //temp location for stdout
	stdout = &mystdout;	//change to oled printing
	printf(string);
	stdout = temp; //change back to terminal printing
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