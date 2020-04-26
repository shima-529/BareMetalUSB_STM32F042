#include "stm32f0xx.h"

void usart_init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER9_Msk;
	GPIOA->MODER &= ~GPIO_MODER_MODER10_Msk;
	GPIOA->MODER |= GPIO_MODE_AF_PP << GPIO_MODER_MODER9_Pos;
	GPIOA->MODER |= GPIO_MODE_AF_PP << GPIO_MODER_MODER10_Pos;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH1_Msk;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH2_Msk;
	GPIOA->AFR[1] |= GPIO_AF1_USART1 << GPIO_AFRH_AFRH1_Pos;
	GPIOA->AFR[1] |= GPIO_AF1_USART1 << GPIO_AFRH_AFRH2_Pos;

	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
//	USART1->BRR = 48e6 / 115200;
	USART1->BRR = 48e6 / 230400;
	USART1->CR1 |= USART_CR1_UE;
}


void usart_send(char ch) {
	while( !(USART1->ISR & USART_ISR_TXE) );
	USART1->TDR = ch;
}

void usart_str(char *str) {
	while( *str ) {
		if( *str == '\n' ) {
			usart_send('\r');
		}
		usart_send(*str);
		str++;
	}
}

char usart_get(void) {
	while( !(USART1->ISR & USART_ISR_RXNE) );
	return USART1->RDR;
}

int __io_putchar(int ch) {
	if( ch == '\n' ) {
		usart_send('\r');
	}
	usart_send(ch);
	return 1;
}
