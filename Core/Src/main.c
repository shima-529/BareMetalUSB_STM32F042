#include "stm32f0xx.h"
#include "sysclk_systick.h"
#include "timer.h"
#include "usart.h"
#include "adc.h"
#include "dma.h"
#include "flash.h"
#include "spi.h"
#include "i2c.h"
#include "lcd.h"
#include "usb/usb_cdc.h"
#include "usb/usb_hid.h"
#include <stdio.h>
#include <ctype.h>

typedef struct {
	uint8_t keycode;
	char ch;
	uint8_t modifier;
} Key;
static const Key keycode[] = {
		{4, 'A', 2},
		{5, 'B', 2},
		{6, 'C', 2},
		{7, 'D', 2},
		{8, 'E', 2},
		{9, 'F', 2},
		{10, 'G', 2},
		{11, 'H', 2},
		{12, 'i', 2},
		{13, 'J', 2},
		{14, 'K', 2},
		{15, 'L', 2},
		{16, 'M', 2},
		{17, 'N', 2},
		{18, 'O', 2},
		{19, 'P', 2},
		{20, 'Q', 2},
		{21, 'R', 2},
		{22, 'S', 2},
		{23, 'T', 2},
		{24, 'U', 2},
		{25, 'V', 2},
		{26, 'W', 2},
		{27, 'X', 2},
		{28, 'Y', 2},
		{29, 'Z', 2},
		{4, 'a', 0},
		{5, 'b', 0},
		{6, 'c', 0},
		{7, 'd', 0},
		{8, 'e', 0},
		{9, 'f', 0},
		{10, 'g', 0},
		{11, 'h', 0},
		{12, 'i', 0},
		{13, 'j', 0},
		{14, 'k', 0},
		{15, 'l', 0},
		{16, 'm', 0},
		{17, 'n', 0},
		{18, 'o', 0},
		{19, 'p', 0},
		{20, 'q', 0},
		{21, 'r', 0},
		{22, 's', 0},
		{23, 't', 0},
		{24, 'u', 0},
		{25, 'v', 0},
		{26, 'w', 0},
		{27, 'x', 0},
		{28, 'y', 0},
		{29, 'z', 0},
		{30, '1', 0},
		{31, '2', 0},
		{32, '3', 0},
		{33, '4', 0},
		{34, '5', 0},
		{35, '6', 0},
		{36, '7', 0},
		{37, '8', 0},
		{38, '9', 0},
		{39, '0', 0},
		{40, '\n', 0},
		{0x89, '|', 2},
		{33, '$', 2},
		{0x2D, '-', 0},
		{0x2D, '=', 2},
		{35, '&', 2},
		{36, '\'', 2},
		{0x2c, ' ', 0}
};
inline static Key ascii_to_keycode(const char ch) {
	for(int i = 0; i < sizeof(keycode); i++) {
		if( keycode[i].ch == ch ) {
			return keycode[i];
		}
	}
	return (Key){0, 0, 0};
}

extern int8_t __attribute__((aligned(2))) report[2][8];
void usb_mouse_star(void) {
	usb_hid_mouse_init();

	ms_wait(3000);

	const int r = 20;
	const int sin_18_by_10000 = 3090;
	const int cos_18_by_10000 = 9511;
	const int sin_36_by_10000 = 5878;
	const int cos_36_by_10000 = 8090;
	const int sin_72_by_10000 = 9511;
	const int cos_72_by_10000 = 3090;

	const int8_t dr[][2] = { // (y, x)
			{r  * cos_18_by_10000 / 10000, -r * sin_18_by_10000 / 10000},
			{-r * sin_36_by_10000 / 10000, r  * cos_36_by_10000 / 10000},
			{0, -r},
			{r  * sin_36_by_10000 / 10000, r  * cos_36_by_10000 / 10000},
			{-r * sin_72_by_10000 / 10000, -r * cos_72_by_10000 / 10000}
	};
	while(1) {
		for(int i = 0; i < 5; i++ ) {
			report[0][2] = dr[i][0];
			report[0][1] = dr[i][1];
			ms_wait(200);
			report[0][1] = report[0][2] = 0;
			ms_wait(10);
		}
	}
}

void usb_keyboard_fuck(void) {
	usb_hid_keyboard_init();

	const char mes[] = "fuck";
	while(1) {
		const char *p = mes;
		while( *p ) {
			Key k = ascii_to_keycode(*p);
			report[0][0] = k.modifier;
			report[0][2] = k.keycode;
			ms_wait(10);
			p++;
		}
	}
}
void usb_keyboard_shellgei(void) {
	usb_hid_keyboard_init();
	ms_wait(3000);

	const char mes[] = "seq 1000 | factor | awk 'NF==2&&$0=$2' | xargs\n";
	const char *p = mes;
	while( *p ) {
		Key k = ascii_to_keycode(*p);
		report[0][0] = k.modifier;
		report[0][2] = k.keycode;
		ms_wait(10);
		report[0][0] = 0;
		report[0][2] = 0;
		ms_wait(8);
		p++;
	}
	while(1);
}

void usb_composite(void) {
	usb_hid_composite_init();
	ms_wait(3000);

	enum {
		MOUSE, KEYBOARD
	};

	const char mes[] = "seq 1000 | factor | awk 'NF==2&&$0=$2' | xargs\n";
	const char *p = mes;
	while( *p ) {
		Key k = ascii_to_keycode(*p);
		report[KEYBOARD][0] = k.modifier;
		report[KEYBOARD][2] = k.keycode;
		ms_wait(10);
		report[KEYBOARD][0] = 0;
		report[KEYBOARD][2] = 0;
		ms_wait(8);
		p++;
	}
	const int r = 20;
	const int sin_18_by_10000 = 3090;
	const int cos_18_by_10000 = 9511;
	const int sin_36_by_10000 = 5878;
	const int cos_36_by_10000 = 8090;
	const int sin_72_by_10000 = 9511;
	const int cos_72_by_10000 = 3090;

	const int8_t dr[][2] = { // (y, x)
			{r  * cos_18_by_10000 / 10000, -r * sin_18_by_10000 / 10000},
			{-r * sin_36_by_10000 / 10000, r  * cos_36_by_10000 / 10000},
			{0, -r},
			{r  * sin_36_by_10000 / 10000, r  * cos_36_by_10000 / 10000},
			{-r * sin_72_by_10000 / 10000, -r * cos_72_by_10000 / 10000}
	};
	while(1) {
		for(int i = 0; i < 5; i++ ) {
			report[MOUSE][2] = dr[i][0];
			report[MOUSE][1] = dr[i][1];
			ms_wait(200);
			report[MOUSE][1] = report[MOUSE][2] = 0;
			ms_wait(10);
		}
	}
}

void usb_cdc_echo_fuck(void) {
	usb_cdc_init();

	while(1) {
		ms_wait(1000);
		usb_cdc_send("Fuck", 4);
	}

}

int main(void) {
	sysclk_HSI48_init();
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODER1_Msk;
	GPIOB->MODER |= GPIO_MODE_OUTPUT_PP << GPIO_MODER_MODER1_Pos;
	usart_init();
//	setbuf(stdin, NULL);
//	setbuf(stdout, NULL);

//	usb_keyboard_shellgei();
//	usb_mouse_star();
//	usb_composite();
	usb_cdc_echo_fuck();

	while(1) {

	}
}
