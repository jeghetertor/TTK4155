#include "timer.h"
#include "sam.h"

void init_timer(){
	TC0->TC_WPMR = (0x54494D<<8);
	TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN;
	TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK1;
	TC0->TC_CHANNEL[0].TC_RC = 0x348;
	//PMC->PMC_PCR |= (27<<0);
	PMC->PMC_PCER0 |= (1<<ID_TC0);
}
void delay_us(long us){
	//printf("Start");
	SysTick->CTRL = (1<<SysTick_CTRL_ENABLE_Pos);
	//SysTick->CTRL = (1<<SysTick_CTRL_CLKSOURCE_Pos) | (1<<SysTick_CTRL_ENABLE_Pos);
	//SysTick->CTRL &= ~(1<<SysTick_CTRL_COUNTFLAG_Pos);
	SysTick->LOAD = 84*us*2; //dont ask about this
	//printf("SYSTICK %d\n",SysTick->CTRL);
	
	while(SysTick->VAL > 100){ //pretend you dont see
		//printf("SYSTICK %d\n",SysTick->VAL);
		//printf("Waiting");
	}
	//printf("Finished");
	
}

long millis(){
	return RTT->RTT_VR;
}

void delay_ms(int duration){
	long start = millis();
	while(millis()<start+duration){};
}
