#include "stm32f0xx.h"
#include "adc.h"

uint32_t adc_data;

void dma_adc_init(void) { // using DMA1_1
	adc_init();
	RCC->AHBENR |= RCC_AHBENR_DMA1EN; // supply clock to DMA1
	DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR; // peripheral addresss
	DMA1_Channel1->CMAR = (uint32_t)&adc_data; // memory address
	DMA1_Channel1->CNDTR = 1; // 1 data
	DMA1_Channel1->CCR |= DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1 | DMA_CCR_CIRC;
	// Peripheral: 32bit, Memory: 32bit, Read from Peripheral, circular mode.

	DMA1_Channel1->CCR |= DMA_CCR_EN | DMA_CCR_MINC;
//	DMA1_Channel1->CCR |= DMA_CCR_EN | DMA_CCR_TCIE;
//	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	ADC1->CFGR1 |= ADC_CFGR1_DMAEN | ADC_CFGR1_DMACFG; // enable DMA as Circular Mode.
	ADC1->CFGR1 |= ADC_CFGR1_CONT; // continuous single convert mode
	ADC1->CHSELR |= 1; // ch0
	ADC1->CR |= ADC_CR_ADSTART; // start a/d convert
}
