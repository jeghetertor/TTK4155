#include "dac.h"
#include "sam.h"

void dac_init(){
	PMC->PMC_PCR |= (38<<0);
	PMC->PMC_PCER1 |= 1 <<(38-32);
	DACC->DACC_MR = DACC_MR_WORD_HALF | DACC_MR_USER_SEL_CHANNEL1;
	
	DACC->DACC_CHER = (1<<1);
	
	//int data_dacc = DACC->DACC_CDR;

	/*
	PMC->PMC_PCR |= (37<<0);
	PMC->PMC_PCER1 |= 1 <<(37-32);
	ADC->ADC_CR = ADC_CR_START;
	ADC->ADC_MR = ADC_MR_FREERUN;
	ADC->ADC_CHER = ADC_CHDR_CH0;
	*/
}

void dac_write(int val){
	DACC->DACC_CDR = val;
}
