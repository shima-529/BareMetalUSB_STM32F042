/*
 * flash.h
 *
 *  Created on: 2020/04/06
 *      Author: yuki
 */

#ifndef FLASH_H_
#define FLASH_H_


void flash_init(void);
void flash_erase_sector(int num);
void flash_write(uint16_t *addr, uint16_t data);



#endif /* FLASH_H_ */
