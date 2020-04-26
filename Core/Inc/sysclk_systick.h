/*
 * sysclk_systick.h
 *
 *  Created on: Apr 7, 2020
 *      Author: yuki
 */

#ifndef INC_SYSCLK_SYSTICK_H_
#define INC_SYSCLK_SYSTICK_H_

void ms_wait(unsigned int msec);
void sysclk_HSI48_init(void);
void sysclk_PLL48_init(void);




#endif /* INC_SYSCLK_SYSTICK_H_ */
