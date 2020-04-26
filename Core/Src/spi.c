#include "stm32f0xx.h"

static void spi_io_init() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // IO portA clock enable
	GPIOA->MODER |= (GPIO_MODE_AF_PP << GPIO_MODER_MODER4_Pos) // SSEL is driven by GPIO output.
			| (GPIO_MODE_AF_PP << GPIO_MODER_MODER5_Pos)
			| (GPIO_MODE_AF_PP << GPIO_MODER_MODER6_Pos)
			| (GPIO_MODE_AF_PP << GPIO_MODER_MODER7_Pos);
	GPIOA->AFR[0] |= (GPIO_AF0_SPI1 << GPIO_AFRL_AFRL4_Pos) // PA4 -> NSS
			| (GPIO_AF0_SPI1 << GPIO_AFRL_AFRL5_Pos) // PA5 -> SCK
			| (GPIO_AF0_SPI1 << GPIO_AFRL_AFRL6_Pos) // PA6 -> MISO
			| (GPIO_AF0_SPI1 << GPIO_AFRL_AFRL7_Pos); // PA7 -> MOSI
}

void spi_init(uint8_t data_len) {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	spi_io_init();
	SPI1->CR1 |= SPI_CR1_MSTR | (7 << SPI_CR1_BR_Pos); // Master, 16MHz
	SPI1->CR2 |= SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_FRXTH;
	SPI1->CR2 &= ~SPI_CR2_DS_Msk;
	SPI1->CR2 |= ((uint8_t)(data_len - 1) << SPI_CR2_DS_Pos);
	SPI1->CR1 |= SPI_CR1_SPE; // enable
}

void spi_send(uint16_t dat) {
	while( !(SPI1->SR & SPI_SR_TXE) );
	SPI1->DR = dat;
	while( SPI1->SR & SPI_SR_BSY );
}
