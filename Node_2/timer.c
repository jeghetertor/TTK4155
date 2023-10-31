#include "timer.h"
#include "sam.h"

long millis(){
	return RTT->RTT_VR;
}

void delay_ms(int duration){
	long start = millis();
	while(millis()<start+duration){};
}