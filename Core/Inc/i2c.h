/*
 * i2c.h
 *
 *  Created on: Apr 7, 2020
 *      Author: yuki
 */

#ifndef I2C_H_
#define I2C_H_

void i2c_init(void);
void i2c_send(uint8_t addr, uint8_t *dat, int size);




#endif /* I2C_H_ */
