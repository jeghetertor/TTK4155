/*
 * Node_2.c
 *
 * Created: 10.10.2023 08:23:00
 * Author : torhstau
 */ 

#include "uart.h"
#include "printf-stdarg.h"
#include "sam.h"
#include "can_controller.h"
#include "can_interrupt.h"


void delay_ms(int ms){
	for(int i = 0; i< ms*1000;i++){

	}
}

int main(void)
{
	SystemInit();
	/// CONFIG PIN 19 AND 20 AS OUTPUT ///////
	PIOA->PIO_PER = PIO_PER_P19|PIO_PER_P20;	//ENABLE PORTS
	PIOA->PIO_OER = PIO_PER_P19|PIO_PER_P20;	//SET AS OUTPUT
	//////////////////////////////////////////
	PMC->PMC_PCER1 = 1;
	// ENABLE 
	//PMC_PCR_EN-> 
	
	
	int iteration = 0;
	configure_uart();
	
	can_init_def_tx_rx_mb(BAUD_REGISTER_CAN);
	
	
	
	CAN_MESSAGE msg = {
		1,
		1,
		'a'
	};
	
	
	
    /* Replace with your application code */
    while (1) 
    {
		uint8_t var;
		CAN_MESSAGE can_msg = {
			0,
			1,
			'a'
			};
		//var = can_receive(&can_msg, 1);
		//printf(can_msg.data);
		can_send(&msg, 0);
		
		printf(msg.data);
		/*
		if(iteration < 50000){
			PIOA->PIO_CODR = PIO_PER_P19|PIO_PER_P20;
		}
		else{
			PIOA->PIO_SODR = PIO_PER_P19|PIO_PER_P20;
		}
		if(iteration > 100000){
			
			iteration = 0;
		}
		
		iteration = iteration + 1;
		*/
		
    }
}

