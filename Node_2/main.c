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
#include "adc.h"
#include "pwm.h"
#include "dac.h"
#include "motor_driver.h"
#include "timer.h"
#include "solenoid.h"
#include "regulator.h"
#include "goal_logic.h"
#include <stdbool.h>

long position = 0;
int error_sum = 5;
int lastEncoderReading;
int currentEncoderReading;


int main(void)
{
	SystemInit();
	// RTT PRESCALER
	RTT->RTT_MR = 27;
	
	// Peripherals enabeling of PMC:
	PMC->PMC_WPMR |=  (0x504d43<<8);
	PMC->PMC_WPMR &= ~(1<<0);
	//CAN0->CAN_WPMR |= (0x43414E<<)
	CAN0->CAN_WPMR &= ~(1<<0);

	// LED blink Shit
	/// CONFIG PIN 19 AND 20 AS OUTPUT ///////
	//PIOA->PIO_PER = PIO_PER_P19|PIO_PER_P20;	//ENABLE PORTS
	//PIOA->PIO_OER = PIO_PER_P19|PIO_PER_P20;	//SET AS OUTPUT
	//////////////////////////////////////////
	
	configure_uart();
	init_can();
	pwm_init();
	adc_init();
	motor_init();
	solenoid_init();
	
	//sei();
	goal.current_goal = 0;
	goal.debounce = 0;
	goal.old_goal = 0;
	goal.scored_flag = 0;
	
	difficulty = 1;
    while (1) 
    {
		CAN_MESSAGE goal_msg = {
			0,
			1,
			0
			};
			
		// LOOP DELAY
		lastEncoderReading = motor_read_encoder();
		while(1){
			goal_msg.data[0] = goal.current_goal;
			can_send(&goal_msg, 0);
			CAN0_Handler();
			servo_set_position(joy.x);
			goal_scored(&goal, ADC->ADC_CDR[0]);
			PID(joy.r_slider,motor_read_encoder(),0.25, 1.0, &error_sum, 0.01,difficulty);
			
			if(joy.y>20){
				solenoid_control(true);
			}
			else{
				solenoid_control(false);
			}
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

