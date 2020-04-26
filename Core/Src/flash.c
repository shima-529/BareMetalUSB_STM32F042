#include "stm32f0xx.h"
#include <stdio.h>

void flash_init(void) {
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

void flash_erase_sector(int num) {
	while( FLASH->SR & FLASH_SR_BSY );
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = num * 0x400 + FLASH_BASE;
	FLASH->CR |= FLASH_CR_STRT;
	while( FLASH->SR & FLASH_SR_BSY );
	FLASH->CR &= ~FLASH_CR_PER;
	FLASH->SR &= ~FLASH_SR_EOP;
}

void flash_write(uint16_t *addr, uint16_t data) {
	while( FLASH->SR & FLASH_SR_BSY );
	FLASH->CR |= FLASH_CR_PG;
	__NOP();
	*addr = data;
	__NOP();
	while( FLASH->SR & FLASH_SR_BSY );
	FLASH->SR &= ~FLASH_SR_EOP;
	FLASH->CR &= ~FLASH_CR_PG;
}
