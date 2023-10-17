#ifndef CAN_H_
#define CAN_H_

#define BAUD_phase1_pos 3
#define BAUD_phase2_pos 0
#define BAUD_propag_pos 0
#define BAUD_sjw_pos 6
#define BAUD_brp_pos 0
#define BAUD_bitmode_pos 7

typedef struct CAN_message{
	uint8_t ID;
	uint8_t length;
	char data[8];
	} CAN_message;
	
void CAN_init();
void CAN_receive(uint8_t buffer_number, CAN_message* can_message);
void CAN_transmit(CAN_message *can_message, uint8_t buffer_number);
void CAN_test();

#endif