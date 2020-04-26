/*
 * usb_desc.h
 *
 *  Created on: 2020/04/26
 *      Author: yuki
 */

#ifndef INC_USB_DESC_USB_DESC_H_
#define INC_USB_DESC_USB_DESC_H_

extern const USBDeviceDescriptor dev_desc_hid;
extern const USBDeviceDescriptor dev_desc_cdc;
extern const USBStringDescriptor string_desc[];
extern const __attribute__((aligned(2))) uint8_t conf_desc_mouse[];
extern const __attribute__((aligned(2))) uint8_t conf_desc_keyb[];
extern const __attribute__((aligned(2))) uint8_t conf_desc_keyb_mouse[];
extern const uint8_t __attribute__((aligned(2))) conf_desc_cdc[];
extern const uint8_t __attribute__((aligned(2))) ReportDescriptorForMouse[50];
extern const uint8_t __attribute__((aligned(2))) ReportDescriptorForKeyBoard[63];




#endif /* INC_USB_DESC_USB_DESC_H_ */
