/*
 * usb_hid.h
 *
 *  Created on: 2020/04/26
 *      Author: yuki
 */

#ifndef INC_USB_USB_HID_H_
#define INC_USB_USB_HID_H_

extern int8_t report[2][8];

void usb_hid_mouse_init(void);
void usb_hid_keyboard_init(void);
void usb_hid_composite_init(void);




#endif /* INC_USB_USB_HID_H_ */
