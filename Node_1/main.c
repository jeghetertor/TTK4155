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


void ADC_PWM_clock_init(void){
	DDRD |= (1<<PD4);
	TCCR3B |= (1 << CS30) | (1 << WGM32);
	TCCR3A |= (1 << COM3A0) | (1 << COM3B0);
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
	
	DDRB |= (0<<PB1) | (0<<PB2); // Set buttons as input
	
	sei();

	printf("\n\n\n\n\n\n\n\n\n\nBOOOT\n");
	
	main_menu();
	
	return 0;
}

ISR(INT0_vect){
	CAN_receive(1, &msg);	
	mcp_bit_modify(MCP_CANINTF, MCP_RX1IF, 0);
	mcp_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	//_delay_ms(100);
}