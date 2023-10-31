#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "sam.h"
#include "motor_driver.h"
#include "dac.h"
#include "timer.h"


void motor_init(){
	dac_init();
	// Enable ports
	PIOD->PIO_PER |= DIR|EN|SEL|NOT_OE|NOT_RST;
	PIOC->PIO_PER |= encoder0|encoder1|encoder2|encoder3|encoder4|encoder5|encoder6|encoder7;
	// Enable output
	PIOD->PIO_OER |= DIR|EN|SEL|NOT_OE|NOT_RST;
	// Enable input
	PIOC->PIO_ODR |= encoder0|encoder1|encoder2|encoder3|encoder4|encoder5|encoder6|encoder7;
	// Enable IO clock
	PMC->PMC_PCR |= (14<<0);
	PMC->PMC_PCER1 |= 1 <<(14);
	
	

	
}

void motor_openloop(int8_t x){
	bool direction;
	
	if(x>=0){
		direction = true;
	}else{
		direction = false;
	}
	int speed = (abs(x))*65;
	motor_write(direction,speed);
}

void motor_write(bool direction, int speed){
	if(direction){
		//printf("codr");
		PIOD->PIO_CODR |= DIR;
	}
	else{
		//printf("sodr");
		PIOD->PIO_SODR |= DIR;
	}
	//printf("speed: %d ", speed);
	dac_write(speed);
	
}

int motor_read_encoder(){
	int low_byte = 0;
	int high_byte = 0;
	int result = 0;
	// Set !OE low, to sample and hold the encoder value
	PIOD->PIO_CODR |= NOT_OE;
	// Set SEL low to output high byte
	PIOD->PIO_CODR |= SEL;
	// Wait approx. 20 microseconds for output to settle
	delay_ms(1);
	// Read MJ2 to get high byte
	high_byte = PIOD->PIO_PDSR;
	// Set SEL high to output low byte
	PIOD->PIO_SODR |= SEL;
	// Wait approx. 20 microseconds
	delay_ms(1);
	// Read MJ2 to get low byte
	low_byte = PIOD->PIO_PDSR;
	// Set !OE to high
	PIOD->PIO_SODR |= NOT_OE;
	
	// Reset encoder
	PIOD->PIO_CODR |= NOT_RST;
	delay_ms(1);
	PIOD->PIO_SODR |= NOT_RST;
	
	result = low_byte + (high_byte<<8);
	
	return result; 

}

void motor_start(){
	PIOD->PIO_SODR |= EN;
}

void motor_stop(){
	PIOD->PIO_CODR |= EN;
}