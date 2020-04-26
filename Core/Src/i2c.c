#include "stm32f0xx.h"
#include "i2c.h"

static void i2c_io_init(void) {
	GPIOB->MODER |= (GPIO_MODE_AF_PP << GPIO_MODER_MODER6_Pos); // PB6 as SCL
	GPIOB->AFR[0] |= (GPIO_AF1_I2C1 << (4 * 6));
	GPIOB->PUPDR |= (GPIO_PULLUP << GPIO_PUPDR_PUPDR6_Pos);
	GPIOB->OTYPER |= GPIO_OTYPER_OT_6;
	GPIOB->MODER |= (GPIO_MODE_AF_PP << GPIO_MODER_MODER7_Pos); // PB7 as SDA
	GPIOB->AFR[0] |= (GPIO_AF1_I2C1 << (4 * 7));
	GPIOB->PUPDR |= (GPIO_PULLUP << GPIO_PUPDR_PUPDR7_Pos);
	GPIOB->OTYPER |= GPIO_OTYPER_OT_7;
}

void i2c_init(void) { // For Akizuki LCD Module AQMxxx
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	i2c_io_init();
	I2C1->CR2 |= I2C_CR2_AUTOEND; // Master Mode: automatically send STOP condition
	I2C1->TIMINGR |= (0 << I2C_TIMINGR_PRESC_Pos); // no prescale
	I2C1->TIMINGR |= (1 << I2C_TIMINGR_SCLDEL_Pos); // Data setup time > 180ns
	I2C1->TIMINGR |= (1 << I2C_TIMINGR_SDADEL_Pos); // Data hold time
	I2C1->TIMINGR |= (7 << I2C_TIMINGR_SCLH_Pos); // High Frequency
	I2C1->TIMINGR |= (11 << I2C_TIMINGR_SCLL_Pos); // Low Frequency

	I2C1->CR1 |= I2C_CR1_PE; // peripheral enable

}

// fundamental function(hindered to other files)
void i2c_send(uint8_t addr, uint8_t *dat, int size) {
	while( I2C1->ISR & I2C_ISR_BUSY );
	I2C1->CR2 |= (addr << I2C_CR2_SADD_Pos); // slave address
	I2C1->CR2 |= (0 << I2C_CR2_ADD10_Pos); // 7bit
	I2C1->CR2 |= ((addr & 1) << I2C_CR2_RD_WRN_Pos); // write mode
	I2C1->CR2 |= (size << I2C_CR2_NBYTES_Pos); // data size
	I2C1->CR2 |= I2C_CR2_START;
	for(uint8_t i=0; i<size; i++) {
		while( !(I2C1->ISR & I2C_ISR_TXE) );
		I2C1->TXDR = dat[i];
	}
	while( !(I2C1->ISR & I2C_ISR_STOPF) ); // wait until STOP condition is issued
}
