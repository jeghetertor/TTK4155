#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef MCP2515_H_
#include "MCP2515.h"
#endif
#ifndef SPI_H_
#include "SPI.h"
#endif
#ifndef MCP_register_definitions_H_
#include "MCP_register_definitions.h"
#endif


void mcp_init(){
	uint8_t value ;
	SPI_init () ; // Initialize SPI
	mcp_reset() ; // Send reset - command

	
	// Self - test
	value = mcp_read(MCP_CANSTAT);
	if(( value & MODE_MASK ) != MODE_CONFIG ) {
		printf(" MCP2515 is NOT in configuration mode after reset !\n");
	}

	// More initialization
	
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

void mcp_write(uint8_t data){
	ss_enable();
	SPI_send(MCP_WRITE);
	SPI_send(data);
	ss_disable();
}

void mcp_request_send(){
	ss_enable();
	//SPI_send(M)
	
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