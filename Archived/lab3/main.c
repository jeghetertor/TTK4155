#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include "UART.h"
#include "ADC.h"

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

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
		printf("Cycle [%4d]\n", i);
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}
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
	
	MCUCR |= (1 << 7); //Enabling XMEM
	SFIOR = (1 << XMM2); //mask pc4-pc7
	ADC_init();
	UART_init(MYUBRR);
	DDRB |= (0<<PB1) | (0<<PB2);
	struct Offset_const offset_const = ADC_calibration();
	while(1){
		struct ADC adc = ADC_output(offset_const);
		uint8_t joy_dir = Joy_direction(offset_const);
		
		printf(" %02d ", joy_dir);
		
		//printf("Offset x: %02d   Offset y: %02d\n", offset_const.offset_x, offset_const.offset_y);
		//struct JoyAngle joyAngle = read_joy_angle();
		//struct Sliders slider = read_slider();
		
		//printf("right:%3d left:%3d x:%3d y:%3d\n",adc.r_slider,adc.l_slider,adc.x_axis,adc.y_axis);
		//bool pb1Val = PINB & (1 << PINB1);
		//bool pb2Val = PINB & (1 << PINB2);
		//printf("pb1: %1d pb2: %1d\n", pb1Val, pb2Val);
	}
	
	//OCR0 = 255;
	//uint16_t test = 0x1800
	//uint16_t adr = 0x11F;
	
	//volatile char *ext_ram = (char *) 0x00;
	//uint16_t ram;
	/*while (1)
	{
		ext_ram[adr] = 0x00;
		_delay_ms(100);
	}*/
	//ext_ram[2047] = 0x7FF;
	
	//delay_ms(500);
	//printf("D funker aa importere!\n");
	//byte |= (1 << 1:)
	//latch_test(); 
	//printf("Init test\n");
	//SRAM_test();	

	return 0;
}