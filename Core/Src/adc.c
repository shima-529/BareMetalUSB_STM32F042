#include "stm32f0xx.h"

void adc_init(void) {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODE_ANALOG << GPIO_MODER_MODER0_Pos;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_0; // 24MHz
	ADC1->SMPR = ADC_SMPR_SMP_2;
	ADC1->CR |= ADC_CR_ADEN;
}

uint16_t get_adc(int ch) {
	ADC1->CHSELR = 1 << ch;
	ADC1->CR |= ADC_CR_ADSTART;
	while( !(ADC1->ISR & ADC_ISR_EOC) );
	return ADC1->DR;
}
