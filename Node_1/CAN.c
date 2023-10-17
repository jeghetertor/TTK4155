#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


#ifndef CAN_H_
#include "CAN.h"
#endif
#ifndef MCP_register_definitions_H_
#include "MCP_register_definitions.h"
#endif
#ifndef MCP2515_H_
#include "MCP2515.h"
#endif

void CAN_init(){
	// initier i loopback
	mcp_bit_modify(MCP_CANCTRL, 0b11100000, MODE_NORMAL);
	// Usikker
	mcp_bit_modify(MCP_CANCTRL, 0b00000100, 1); // CLKOUT disabled (?)
	
	//TEMP?
	mcp_bit_modify(MCP_CANINTF, MCP_RX1IF, 0);
	mcp_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);

	
}

void CAN_receive(uint8_t buffer_number, CAN_message* can_message){
	uint8_t ID_lowerbyte;
	uint8_t ID_higherbyte;
	
	switch(buffer_number) {
		case 0 :
		ID_higherbyte = mcp_read(MCP_RXB0SIDH);
		ID_lowerbyte = mcp_read(MCP_RXB0SIDL);
		can_message->ID = (ID_higherbyte << 3) + (ID_lowerbyte >> 5);
		can_message->length = mcp_read(MCP_RXB0DLC);
		for(uint8_t i = 0; i<can_message->length;i++){
			can_message->data[i] = mcp_read(MCP_RXB0D0+i);
			
		}
		//can_message.data[0] = mcp_read(MCP_RXB0D0);
		
		break;
		case 1  :
		ID_higherbyte = mcp_read(MCP_RXB1SIDH);
		ID_lowerbyte = mcp_read(MCP_RXB1SIDL);
		can_message->ID = (ID_higherbyte << 3) + (ID_lowerbyte >> 5);
		can_message->length = mcp_read(MCP_RXB1DLC);
		for(uint8_t i = 0; i<can_message->length;i++){
			can_message->data[i] = mcp_read(MCP_RXB1D0+i);
		}
		break;
		
		default :
		printf("Invalid buffer!\n");
		break;
		}
}

void CAN_transmit(CAN_message *can_message, uint8_t buffer_number){
	//uint8_t instruction = 0b01000000 | buffer_number;
	//mcp_write(instruction);
	
	uint8_t ID_lowerbyte = (can_message->ID&0x7) << 5;
	
	uint8_t ID_higherbyte = (can_message->ID&0x7F8) >> 3;
	
	
	mcp_write(ID_higherbyte, MCP_TXB0SIDH);
	mcp_write(ID_lowerbyte, MCP_TXB0SIDL);
	mcp_write(can_message->length, MCP_TXB0DLC);
	for(uint8_t i = 0; i<can_message->length;i++){
		mcp_write(can_message->data[i],MCP_TXB0D0+i);	
	}
	mcp_request_send(0);
}
void CAN_test(){
	CAN_message my_msg = {
		0b10,
		3,
		"TRE"
	};
	CAN_message my_msg2;

	//CAN_transmit(&my_msg, 1);
	//_delay_ms(10);
	//my_msg2 = CAN_receive(1);
	
	//printf("Data:\n");
	//printf("Length: %d\n", my_msg2.length);
	//printf("ID: %d\n", my_msg2.ID);
	//printf("Data: %s", my_msg2.data);
	_delay_ms(100);
}