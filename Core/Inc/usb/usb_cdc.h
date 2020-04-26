/*
 * usb_cdc.h
 *
 *  Created on: 2020/04/26
 *      Author: yuki
 */

#ifndef INC_USB_USB_CDC_H_
#define INC_USB_USB_CDC_H_


void usb_cdc_init(void);
void usb_cdc_send(const char *str, int size);


#endif /* INC_USB_USB_CDC_H_ */
