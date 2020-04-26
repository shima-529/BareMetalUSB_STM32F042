#include "stm32f0xx.h"

void ms_wait(unsigned int msec) {
	SysTick->LOAD = 6000 - 1;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	for(int i = 0; i < msec; i++) {
		while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void sysclk_HSI48_init(void) {
	RCC->CR2 |= RCC_CR2_HSI48ON;
	while( !(RCC->CR2 & RCC_CR2_HSI48RDY) );
	RCC->CFGR |= RCC_CFGR_SW_HSI48;
	while( (RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI48 );
}

void sysclk_PLL48_init(void) {
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2; // In: 4MHz
	RCC->CFGR |= RCC_CFGR_PLLMUL12; // 48MHz
	RCC->CR |= RCC_CR_PLLON;
	while( !(RCC->CR & RCC_CR_PLLRDY) );
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while( (RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL );
}
