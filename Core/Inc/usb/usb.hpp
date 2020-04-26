/*
 * usb.h
 *
 *  Created on: Apr 9, 2020
 *      Author: yuki
 */

#ifndef INC_USB_H_
#define INC_USB_H_
#include "usb_desc/usb_desc_struct.h"
#include "usb_struct.h"

void usb_init(USB_InitType init);
void usb_ep_send(int ep_num, uint16_t addr, uint16_t size);
void usb_ep_receive(int ep_num, uint16_t addr, uint16_t size);

extern USBEndpointInfo ep_info[4];
extern PMAInfo pma_allocation_info;

#endif /* INC_USB_H_ */
