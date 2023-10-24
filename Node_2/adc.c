#include "adc.h"
#include "sam.h"

void adc_init(){
	PMC->PMC_PCR |= (37<<0);
	PMC->PMC_PCER1 |= 1 <<(37-32);
	ADC->ADC_CR = ADC_CR_START;
	ADC->ADC_MR = ADC_MR_FREERUN;
	ADC->ADC_CHER = ADC_CHDR_CH0;
}