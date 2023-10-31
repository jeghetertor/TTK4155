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
#include <stdbool.h>


void goal_scored(Goal *goal, int IR_val){
	if(goal->debounce > 50){
		if((IR_val < 2300)&&(goal->scored_flag == 0)){
			goal->scored_flag = 1;
			goal->current_goal++;
			goal->debounce = 0;				
		}
	}
	if((IR_val > 2300) && goal->scored_flag == 1){
		goal->scored_flag = 0;
	}
	goal->debounce++;
}

void goal_reset(int *goal){
	goal = 0;
}

int main(void)
{
	
	SystemInit();
	// RTT PRESCALER
	RTT->RTT_MR = 27;
	
	// Peripherals shit:
	PMC->PMC_WPMR |=  (0x504d43<<8);
	PMC->PMC_WPMR &= ~(1<<0);
	//CAN0->CAN_WPMR |= (0x43414E<<)
	CAN0->CAN_WPMR &= ~(1<<0);
	
	//PMC->PMC_PCER1 = 1;
	
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
	motor_start();
	solenoid_init();

	//sei();
	Goal goal = {0,0,100};
	
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
			msg.data[0] = goal.current_goal;
			msg.data_length = 1;
			can_send(&msg, 0);
			//can_receive(&received_msg, 1);
			//printf("%d", received_msg.data[0]);
			CAN0_Handler();
			//printf("x: %d  ", joy.x);
			
			//printf("y: %d\n", joy.y);
			servo_set_position(joy.r_slider - 128);
			// -joy.x
			//printf("%d\n",ADC->ADC_CDR[0]);
			goal_scored(&goal, ADC->ADC_CDR[0]);
			//printf("Goals: %d   ADCval: %d\n", goal.current_goal, ADC->ADC_CDR[0]);
			//PIOD->PIO_CODR |= DIR;
			
			// SOLENOID

			//dac_write(6535);
			//int encoder_val = motor_read_encoder();
			//printf("Encoder val %d\n", encoder_val);
			
			PIOD->PIO_CODR = NOT_OE;
			//printf("x %d\n", joy.x);
			motor_openloop(joy.x);
			printf("Slider: %d\n", joy.r_slider);
			if(joy.y>20){
				printf("LAUNCH");
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

