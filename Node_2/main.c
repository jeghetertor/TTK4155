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

long position = 0;
int error_sum = 5;
int lastEncoderReading;
int currentEncoderReading;
void PID(int r, int y,double Kp,double Ki, int* error_sum, double T){
	r = map(r,0,255,-100,100);
	int satp = 100;
	int satm = -satp;
	int e = r - y;
	*error_sum += e*T;
	int u_p = Kp*e;
	int u_i = 0;
	//int u_i = Ki*(*error_sum);
	int u = u_p+u_i;
	if (u > satp){
		u = satp;
		
	}else if (u < satm){
		u = satm;
	}
	//motor_write(direction,abs(u));
	motor_openloop(u,350);
	printf("r: %d, y: %d, u_p: %d, u_i: %d\n", r, y, u_p, u_i);
}



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
	solenoid_init();
	
	//sei();
	goal.current_goal = 0;
	goal.debounce = 0;
	goal.old_goal = 0;
	goal.scored_flag = 0;
	
    while (1) 
    {
		uint8_t var;
		CAN_MESSAGE msg = {
			0,
			3,
			"lqr"
			};

		CAN_MESSAGE received_msg;
		
		//PIOD->PIO_CODR |= NOT_RST;
		//delay_us(20);
		//PIOD->PIO_SODR |= NOT_RST;
		//while(1){
		//	printf("%d \n",motor_read_encoder());
		//}
		
		// LOOP DELAY
		lastEncoderReading = motor_read_encoder();
		while(1){
			//printf("\nBefore\n %d", 0);
			
			//printf("After\n %d", 0);
			//printf("timer: %d \n", ini);
			msg.data[0] = goal.current_goal;
			msg.data_length = 1;
			can_send(&msg, 0);
			//can_receive(&received_msg, 1);
			//printf("%d", received_msg.data[0]);
			CAN0_Handler();
			//printf("x: %d  ", joy.x);
			
			//printf("y: %d\n", joy.y);
			servo_set_position(joy.x);
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
	
			//motor_openloop(joy.x);
			int16_t val = 0;
			//for (int i = 0; i<100; i++){
			//	val += motor_read_encoder();
			//};
			//val = val/100;
			//printf("%d\n", motor_read_encoder());
			//currentEncoderReading = motor_read_encoder();
			//if(abs(currentEncoderReading-lastEncoderReading) < 20){
			//	lastEncoderReading = currentEncoderReading;
			//}
			//printf("last: %d, current: %d\n", lastEncoderReading,currentEncoderReading);
			PID(joy.r_slider,motor_read_encoder(),0.35, 1.0, &error_sum, 0.01);
			
			//
			//printf("Encoder: %d\n", motor_read_encoder());
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

