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

void init_rt(){
	
}

int main(void){
	cli();
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

	printf("\n\n\n\n\n\n\n\n\n\nBOOOT\n");
	
	main_menu();
	
	return 0;
}

ISR(INT0_vect){
	CAN_receive(1, &msg);
	printf("%d\n", msg.ID);
	
	mcp_bit_modify(MCP_CANINTF, MCP_RX1IF, 0);
	mcp_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	//_delay_ms(100);
}