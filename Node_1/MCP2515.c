#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef MCP2515_H_
#include "MCP2515.h"
#endif
#ifndef SPI_H_
#include "SPI.h"
#endif
#ifndef MCP_register_definitions_H_
#include "MCP_register_definitions.h"
#endif
#include "CAN.h"

void mcp_interupt_enable(){
	
	GICR |= (1<<INT0);
	MCUCR |= (1<<ISC01);
}

void mcp_init(){
	uint8_t value ;
	SPI_init () ; // Initialize SPI
	//mcp_interupt_enable();

	mcp_reset() ; // Send reset - command

	// Self - test
	value = mcp_read(MCP_CANSTAT);
	if(( value & MODE_MASK ) != MODE_CONFIG ) {
		printf(" MCP2515 is NOT in configuration mode after reset !\n");
	}
	
	//ENABLE INTERRUPTS
	mcp_bit_modify(MCP_CANINTE, 0b00000011, 0xFF);
	
	uint8_t BRP = 8;
	uint8_t PS1 = 6;
	uint8_t PS2 = 5;
	uint8_t PROPAG = 1;
	
	uint8_t CN1_data = SJW4|(BRP -1);
	uint8_t CN2_data = BTLMODE | SAMPLE_3X | ((PS1 - 1) << 3) | (PROPAG - 1);
	uint8_t CN3_data = WAKFIL_DISABLE|(PS2-1);
	printf("CN1: %0x\nCN2: %0x\nCN3: %0x\n", CN1_data, CN2_data, CN3_data);
		
	// More initialization
	mcp_write(MCP_CNF1,CN1_data);
	mcp_write(MCP_CNF2,CN2_data);
	mcp_write(MCP_CNF3,CN3_data);
}

uint8_t mcp_read(uint8_t address){
	uint8_t result ;
	ss_enable();
	SPI_send( MCP_READ ); // Send read instruction
	SPI_send( address ); // Send address
	result = SPI_read() ; // Read result
	ss_disable();
	return result ;
}

void mcp_write(uint8_t data, uint8_t adress){
	ss_enable();
	SPI_send(MCP_WRITE);
	SPI_send(adress);
	SPI_send(data);
	ss_disable();
}

void mcp_request_send(uint8_t buffer_number){
	ss_enable();
	switch(buffer_number) {
		case 0  :
		SPI_send(0x81);
		break;
		
		case 1  :
		SPI_send(0x82);
		break;
		
		case 2 :
		SPI_send(0x84);
		break;
	
		default :
		break;
	}
	ss_disable();
	
}

uint8_t mcp_read_status(){
	ss_enable();
	SPI_send(MCP_READ_STATUS);
	uint8_t data = SPI_read();
	ss_disable();
	
	return data;
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
	ss_enable();
	SPI_send(MCP_BITMOD);
	SPI_send(address);
	SPI_send(mask);
	SPI_send(data);
	ss_disable();
}

void mcp_reset(){
	ss_enable();
	SPI_send(MCP_RESET);
	ss_disable();
}