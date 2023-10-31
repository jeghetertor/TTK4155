#include "solenoid.h"
#include "sam.h"
#include <stdbool.h>

void solenoid_init(){
	PIOC->PIO_OER |=  PIO_PER_P9;
}

void solenoid_control(bool state){
	if(state){
		PIOC->PIO_CODR |= PIO_PER_P9;
	}
	else{
		PIOC->PIO_SODR |= PIO_PER_P9;
	}
}