#include <stdint.h>
#include "i2c.h"

const static uint8_t lcd_init_cmds[] = {
		0x38, // function set
		0x39, // function set
		0x14, // internal OSC frequency
		0x73, // contrast set
		0x56, // Power/IOCON/contrast control
		0x6c, // follower control
		0x38, // function set
		0x01, // clear display
		0x0c, // display ON/OFF control
};


const uint8_t ADDR= 0x7c;

void lcd_cmd(uint8_t dat) {
		uint8_t arr[] = {0x00, dat};
		i2c_send(ADDR, arr, 2);
}
void lcd_data(uint8_t dat) {
		uint8_t arr[] = {0x40, dat};
		i2c_send(ADDR, arr, 2);
}
void lcd_str(char *str) {
	while( *str != '\0' ) {
		lcd_data(*str);
		str++;
	}
}

void lcd_init(void) {
	for(int i=0; i< 9; i++){
		lcd_cmd(lcd_init_cmds[i]);
		for(volatile int j=0; j<30000; j++);
	}
}
