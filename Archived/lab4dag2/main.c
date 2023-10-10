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

void write_cmd(uint8_t cmd){
	volatile char *oled_cmd_ext_ram = (char *) 0x1000;
	printf("oled wr\n\r");
	oled_cmd_ext_ram[0] = cmd;
}
//Write data to OLED
void write_data(uint8_t data){
	volatile char *oled_data_ext_ram = (char *) 0x1200;
	oled_data_ext_ram[0] = data;
}

void init_prg(void)
{
	write_cmd(0xae); // display off
	write_cmd(0xa1); //segment remap
	write_cmd(0xda); //common pads hardware: alternative
	write_cmd(0x12);
	write_cmd(0xc8); //common output scan direction:com63~com0
	write_cmd(0xa8); //multiplex ration mode:63
	write_cmd(0x3f);
	write_cmd(0xd5); //display divide ratio/osc. freq. mode
	write_cmd(0x80);
	write_cmd(0x81); //contrast control
	write_cmd(0x50);
	write_cmd(0xd9); //set pre-charge period
	write_cmd(0x21);
	write_cmd(0x20); //Set Memory Addressing Mode
	write_cmd(0x02);
	write_cmd(0xdb); //VCOM deselect level mode
	write_cmd(0x30);
	write_cmd(0xad); //master configuration
	write_cmd(0x00);
	write_cmd(0xa4); //out follows RAM content
	write_cmd(0xa6); //set normal display
	write_cmd(0xaf); // display on
	
	
}

uint8_t arrowPos = 1;
	
int main(void){
	
	_delay_ms(200);
	SRAM_init();
	ADC_init();
	UART_init(MYUBRR);
	printf("Init program...\n");
	oled_init();
	oled_page_addressing();

	
	_delay_ms(100);

	printf("Test\n");
	//for(uint8_t i=0 ; i<100 ; i++){
	//oled_print_arrow(2,2);
	//	//oled_write_data(i);
	//	printf("%d\n",i);
	//	_delay_ms(100);
	//}
	//printf("Fin\n");
	//
	//uint16_t adr = 0x0;
	//
	//
	
	// INIT:
	
	oled_pos(2,2);
	oled_print_char('8');
	oled_print_char('0');
	oled_print_char('0');
	oled_print_char('8');
	oled_print_arrow(0,0);
	oled_reset();
	
	uint8_t move_arrow(uint8_t i, struct Offset_const offset_const){
		//oled_pos(1,i);
		switch(Joy_direction(offset_const)){
			case 4:
			if(i>0){i--;}
			break;
			case 3:
			if(i<7){i++;}
			break;
	
		}
		oled_print_arrow(i,1);
		return i;
	}
	struct Offset_const offset_const = ADC_calibration();
	//struct ADC adc;

	while(1){ // game loop
		oled_reset(); // new frame
		oled_pos(4,20);

		oled_print("hello there!");
		oled_pos(5,20);
		printf("to terminal!");
		arrowPos = move_arrow(arrowPos, offset_const);
		//struct ADC adc = ADC_read();
		_delay_ms(500);
		printf("%d\n", arrowPos);
		//printf("%d\n", adc.x_axis);
		
	}
	//for(int i=0;i<127;i++)
	//{
	//	//ext_ram[0] = 0xFF;
	//	oled_pos(2,i);
	//	oled_write_data(0xFF); // display on
	//	_delay_ms(100);
	//	//int result = add(4,2);
	//}
	//printf("Fin\n");
	
	//printf("Fin\n");


	
	//DDRB |= (0<<PB1) | (0<<PB2);
	//struct Offset_const offset_const = ADC_calibration();

	//SRAM_test();
	
	
	// TEST WRITING TO SPECIFIC PINS
	//bool pb1Val = PINB & (1 << PINB1);
	//bool pb2Val = PINB & (1 << PINB2);
	//printf("pb1: %1d pb2: %1d\n", pb1Val, pb2Val);
	//printf("Fin\n");
	
	return 0;
}