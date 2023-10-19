/*
 * Node_2.c
 *
 * Created: 10.10.2023 08:23:00
 * Author : torhstau
 */ 
#include <stdlib.h>
#include <stdint.h>

#include "Structs.h"

#include "uart.h"
#include "printf-stdarg.h"
#include "sam.h"
#include "can_controller.h"
#include "can_interrupt.h"


/*struct ADC {
	int8_t x_axis;
	int8_t y_axis;
};*/



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
	//sei();
    while (1) 
    {
		uint8_t var;
		CAN_MESSAGE msg = {
			0,
			3,
			"lqr"
			};

		CAN_MESSAGE received_msg;
		
		
		// LOOP DELAY
		
		
		
		while(1){
			can_send(&msg, 0);
			//can_receive(&received_msg, 1);
			//printf("%d", received_msg.data[0]);
			CAN0_Handler();
			printf("x: %d  ", joy.x);
			printf("y: %d\n", joy.y);
			};
		
		
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

