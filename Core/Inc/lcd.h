/*
 * lcd.h
 *
 *  Created on: Apr 4, 2020
 *      Author: yuki
 */

#ifndef LCD_H_
#define LCD_H_

void lcd_cmd(uint8_t dat);
void lcd_data(uint8_t dat);
void lcd_str(char *str);
void lcd_init(void);




#endif /* LCD_H_ */
