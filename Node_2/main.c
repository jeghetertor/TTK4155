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


int main(void)
{
	SystemInit();
	
	// Peripherals shit:
	PMC->PMC_WPMR |=  (0x504d43<<8);
	PMC->PMC_WPMR &= ~(1<<0);
	//CAN0->CAN_WPMR |= (0x43414E<<)
	CAN0->CAN_WPMR &= ~(1<<0);
	
	//PMC->PMC_PCER1 = 1;
	
	// LED blink Shit
	/// CONFIG PIN 19 AND 20 AS OUTPUT ///////
	PIOA->PIO_PER = PIO_PER_P19|PIO_PER_P20;	//ENABLE PORTS
	PIOA->PIO_OER = PIO_PER_P19|PIO_PER_P20;	//SET AS OUTPUT
	//////////////////////////////////////////
	
	configure_uart();
	init_can();
	
    while (1) 
    {
		uint8_t var;
		CAN_MESSAGE msg = {
			0,
			1,
			'l'
			};

		CAN_MESSAGE recived_msg;
		
		
		// LOOP DELAY

		can_send(&msg, 0);
		//can_receive(msg,0);
		
		
		/* LED BLINK
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

