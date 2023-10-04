#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>


#ifndef CAN_H_
#include "CAN.h"
#endif
#ifndef MCP_register_definitions_H_
#include "MCP_register_definitions.h"
#endif

void CAN_init(){
	// initier i loopback
	ss_enable();
	SPI_send(MODE_LOOPBACK);
	ss_disable();
	
}

CAN_message CAN_receive(uint8_t buffer_number){
	ss_enable();
	uint8_t ID_lowerbyte;
	uint8_t ID_higherbyte;
	CAN_message can_message;
	switch(buffer_number) {
		case 1  :

		ID_lowerbyte = mcp_read(MCP_RXB0SIDL);
		ID_higherbyte = mcp_read(MCP_RXB0SIDH);
		can_message.ID = ID_higherbyte << 3 + ID_lowerbyte >> 5;
		can_message.length = mcp_read(MCP_TXB0DLC);
		//mcp_write(0x66);
		//can_message.data[i] = mcp_read(MCP_RXB0SIDL);
		for(uint8_t i = 0; i<can_message.length;i++){
			mcp_read(can_message.data[i],MCP_RXB0D0+i);
		}
		
		break;
		/*	
		case 2  :
	
		SPI_send(MCP_RXB1SIDH);
		can_message.ID = SPI_read();
		SPI_send(0x76);
		can_message.data = SPI_read();
		break;
		*/
		default :
		printf("Invalid buffer!\n");
		break;
		}
		ss_disable();
	return can_message;
}

void CAN_transceive(CAN_message can_message, uint8_t buffer_number){
	//uint8_t instruction = 0b01000000 | buffer_number;
	//mcp_write(instruction);
	uint8_t ID_lowerbyte = (can_message.ID&0x7f8) >> 3;
	uint8_t ID_higherbyte = can_message.ID&0x7 << 5;
	mcp_write(ID_higherbyte, MCP_TXB0SIDH);
	mcp_write(ID_lowerbyte, MCP_TXB0SIDL);
	mcp_write(can_message.length, MCP_TXB0DLC);
	for(uint8_t i = 0; i<can_message.length;i++){
		mcp_write(can_message.data[i],MCP_TXB0D0+i);		
	}
	
}
void CAN_test(){
	CAN_message my_msg;
	my_msg.data = "123";
	my_msg.ID = 0b1;
	my_msg.length = 3;
	CAN_transceive(my_msg, 1);
	my_msg = CAN_receive(1);
	
}