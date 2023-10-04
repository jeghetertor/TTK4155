#ifndef CAN_H_
#define CAN_H_

typedef struct CAN_message{
	uint8_t ID;
	uint8_t length;
	char data[7];
	} CAN_message;
	
void CAN_init();
CAN_message CAN_receive(uint8_t buffer_number);
void CAN_transceive(CAN_message can_message, uint8_t buffer_number);

#endif