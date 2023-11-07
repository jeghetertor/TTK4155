#include "pwm.h"
#include "sam.h"

void pwm_init(){
	//printf("123");
	//printf("WPMR: %0x",PIOC->PIO_WPMR);
	// WRONG SCHEMATIC PIN45 -> CHANNEL5, PIN44 -> CHANNEL6
	//PMC->PMC_PCR |= (36<<0);
	PMC->PMC_PCER1 |= 1 <<(36-32);
	PIOC->PIO_WPMR |= (0x50494F<<8);
	PIOC->PIO_WPMR &= ~(1<<0); // DISABLE WRITE PROTECTION
	
	PIOC->PIO_PDR |= PIO_PDR_P18; //DISABLE PIO control pin 18
	PIOC->PIO_ABSR |= PIO_ABSR_P18; // ENABLE peripheral B
	
	//PMC->PMC_PCR = PMC_PCR_EN;
	
	PWM->PWM_WPCR |= (0x50574D<<8); //DISABLE WRITE PROTECTON
	PWM->PWM_WPCR |= ~(1<<0);
	PWM->PWM_CH_NUM[6].PWM_CMR = PWM_CMR_CPRE_MCK_DIV_128;
	PWM->PWM_CH_NUM[6].PWM_CPRD = 13125; // 50 hertz
	//REG_PWM_CPRD6 = (13125); //might need to bitshift? says only first 16 bits are significant
	//REG_PWM_CMR6 = (0b0111<<0) | (8<<0) | (9<<0); //MCK/128 CALG = 0 CPOL = 0
	
	//REG_PWM_CDTY6 = (12141); // 7.5% duty cycle
	PWM->PWM_CH_NUM[6].PWM_CDTY = 12141;
	PWM->PWM_ENA |= (1<<6);
	printf("CPRD: %d",PWM->PWM_CH_NUM[6].PWM_CMR);
	//PWM->PWM_ENA |= (1<<6);
}
long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void servo_set_position(int pos){
	
	//if(duty < 0.05 || duty > 0.1){
	//	duty = 0.075;
	//}
	
	//duty = (-duty*20E-3 +20E-3)656250 =
	
	long foo;
	foo = map(pos, -100,100,11812,12468);
	if((foo <= 12468) && (foo >= 11812)){
		PWM->PWM_CH_NUM[6].PWM_CDTYUPD = foo;
	}
}