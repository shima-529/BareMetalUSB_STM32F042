#include <stdint.h>
#include <usb_desc/usb_desc_struct.h>

#ifdef __cplusplus
extern "C" {
#endif
const USBDeviceDescriptor dev_desc_hid = {
		.bLength = DEVICE_DESCRIPTOR_LENGTH,
		.bDescriptorType = DEVICE_DESCRIPTOR_TYPE,
		.bcdUSB = USB_BCD_USB2_0, // USB2.0
		.bDeviceClass = 0, // refer to the IF desc
		.bDeviceSubClass = 0, // unused
		.bDeviceProtocol = 0, // unused
		.bMaxPacketSize0 = 64,
		.idVendor = 0xdead,
		.idProduct = 0xbeef,
		.bcdDevice = 0x100,
		.iManufacturer = 2,
		.iProduct = 3,
		.iSerialNumber = 1,
		.bNumConfigurations = 1
};
const USBDeviceDescriptor dev_desc_cdc = {
		.bLength = DEVICE_DESCRIPTOR_LENGTH,
		.bDescriptorType = DEVICE_DESCRIPTOR_TYPE,
		.bcdUSB = USB_BCD_USB2_0, // USB2.0
		.bDeviceClass = 2, // CDC
		.bDeviceSubClass = 0, // unused
		.bDeviceProtocol = 0, // unused
		.bMaxPacketSize0 = 64,
		.idVendor = 0xdead,
		.idProduct = 0xbeef,
		.bcdDevice = 0x100,
		.iManufacturer = 2,
		.iProduct = 3,
		.iSerialNumber = 1,
		.bNumConfigurations = 2
};

// For USB HID(Mouse)
const uint8_t __attribute__((aligned(2))) conf_desc_mouse[] = {
		// Configuration Descriptor
		CONFIG_DESCRIPTOR_LENGTH,
		CONFIG_DESCRIPTOR_TYPE,
		34 & 0xFF, // .wTotalLength
		34 >> 8, // same above
		1, // .bNumInterfaces
		1, // .bConfigurationValue,
		0, // .iConfiguration
		0x80, // .bmAttributes
		0x32, // MaxPower

		// Interface Descriptor
		INTERFACE_DESCRIPTOR_LENGTH,
		INTERFACE_DESCRIPTOR_TYPE,
		0, // bInterfaceNumber
		0, // bAlternateSetting
		1, // bNumEndpoints
		0x03, // .bInterfaceClass
		0x01, // .bInterfaceSubClass(Boot Interface Subclass)
		0x02, // bInterfaceProtocol(Mouse)
		0, // iInterface

		// HID Class Descriptor
		0x09, // bLength
		0x21, // bDescriptorType
		0x00, // bcdHID
		0x10, // L> 1.00
		0, // bCountryCode
		1, // bNumDescriptors
		0x22, // bDescriptorType
		50, // wDescriptorLength(The length of Report Desc)
		0x00,

		// Endpoint Descriptor
		ENDPOINT_DESCRIPTOR_LENGTH,
		ENDPOINT_DESCRIOTPR_TYPE,
		0x81, // .bEndpointAddress(IN, EP1)
		0x03, // Interrupt Transfer
		64, // .wMacPacketSize
		0,
		0x0A // bInterval
};
// For USB HID(Keyboard)
const uint8_t __attribute__((aligned(2))) conf_desc_keyb[] = {
		// Configuration Descriptor
		CONFIG_DESCRIPTOR_LENGTH,
		CONFIG_DESCRIPTOR_TYPE,
		34 & 0xFF, // .wTotalLength
		34 >> 8, // same above
		1, // .bNumInterfaces
		1, // .bConfigurationValue,
		0, // .iConfiguration
		0x80, // .bmAttributes
		0x32, // MaxPower

		// Interface Descriptor
		INTERFACE_DESCRIPTOR_LENGTH,
		INTERFACE_DESCRIPTOR_TYPE,
		0, // bInterfaceNumber
		0, // bAlternateSetting
		1, // bNumEndpoints
		0x03, // .bInterfaceClass
		0x01, // .bInterfaceSubClass(Boot Interface Subclass)
		0x01, // bInterfaceProtocol(KeyBoard)
		0, // iInterface

		// HID Class Descriptor
		0x09, // bLength
		0x21, // bDescriptorType
		0x00, // bcdHID
		0x10, // L> 1.00
		0, // bCountryCode
		1, // bNumDescriptors
		0x22, // bDescriptorType
		63, // wDescriptorLength(The length of Report Desc)
		0x00,

		// Endpoint Descriptor
		ENDPOINT_DESCRIPTOR_LENGTH,
		ENDPOINT_DESCRIOTPR_TYPE,
		0x81, // .bEndpointAddress(IN, EP1)
		0x03, // Interrupt Transfer
		64, // .wMacPacketSize
		0,
		0x0A // bInterval
};
// For USB HID Composite Device
const uint8_t __attribute__((aligned(2))) conf_desc_keyb_mouse[] = {
		// Configuration Descriptor
		CONFIG_DESCRIPTOR_LENGTH,
		CONFIG_DESCRIPTOR_TYPE,
		59 & 0xFF, // .wTotalLength
		59 >> 8, // same above
		2, // .bNumInterfaces
		1, // .bConfigurationValue,
		0, // .iConfiguration
		0x80, // .bmAttributes
		0x32, // MaxPower

		// Interface Descriptor(Mouse)
		INTERFACE_DESCRIPTOR_LENGTH,
		INTERFACE_DESCRIPTOR_TYPE,
		0, // bInterfaceNumber
		0, // bAlternateSetting
		1, // bNumEndpoints
		0x03, // .bInterfaceClass
		0x01, // .bInterfaceSubClass(Boot Interface Subclass)
		0x02, // bInterfaceProtocol(Mouse)
		0, // iInterface

		// HID Class Descriptor
		0x09, // bLength
		0x21, // bDescriptorType
		0x00, // bcdHID
		0x10, // L> 1.00
		0, // bCountryCode
		1, // bNumDescriptors
		0x22, // bDescriptorType
		50, // wDescriptorLength(The length of Report Desc)
		0x00,

		// Endpoint Descriptor
		ENDPOINT_DESCRIPTOR_LENGTH,
		ENDPOINT_DESCRIOTPR_TYPE,
		0x81, // .bEndpointAddress(IN, EP1)
		0x03, // Interrupt Transfer
		64, // .wMacPacketSize
		0,
		0x0A, // bInterval

		// Interface Descriptor(Keyboard)
		INTERFACE_DESCRIPTOR_LENGTH,
		INTERFACE_DESCRIPTOR_TYPE,
		1, // bInterfaceNumber
		0, // bAlternateSetting
		1, // bNumEndpoints
		0x03, // .bInterfaceClass
		0x01, // .bInterfaceSubClass(Boot Interface Subclass)
		0x01, // bInterfaceProtocol(KeyBoard)
		0, // iInterface

		// HID Class Descriptor
		0x09, // bLength
		0x21, // bDescriptorType
		0x00, // bcdHID
		0x10, // L> 1.00
		0, // bCountryCode
		1, // bNumDescriptors
		0x22, // bDescriptorType
		63, // wDescriptorLength(The length of Report Desc)
		0x00,

		// Endpoint Descriptor
		ENDPOINT_DESCRIPTOR_LENGTH,
		ENDPOINT_DESCRIOTPR_TYPE,
		0x82, // .bEndpointAddress(IN, EP2)
		0x03, // Interrupt Transfer
		64, // .wMacPacketSize
		0,
		0x0A // bInterval
};
const uint8_t __attribute__((aligned(2))) conf_desc_cdc[] = {
		// Configuration Descriptor
		CONFIG_DESCRIPTOR_LENGTH,
		CONFIG_DESCRIPTOR_TYPE,
		67 & 0xFF, // .wTotalLength
		67 >> 8, // same above
		2, // .bNumInterfaces
		1, // .bConfigurationValue,
		0, // .iConfiguration
		0xC0, // .bmAttributes
		0x32, // MaxPower

		// Interface Descriptor for Communications Class =======
		INTERFACE_DESCRIPTOR_LENGTH,
		INTERFACE_DESCRIPTOR_TYPE,
		0, // bInterfaceNumber
		0, // bAlternateSetting
		1, // bNumEndpoints
		0x02, // .bInterfaceClass(Communication Interface Class)
		0x02, // .bInterfaceSubClass(ACM)
		0x01, // bInterfaceProtocol(Common AT Commands)
		0, // iInterface

		 /* Functional Descriptor[0] ----------------------------------------------- */
		  /* Header Functional Descriptor                                             */
		  0x05,   /* bLength: Endpoint Descriptor size                                */
		  0x24,   /* bDescriptorType: CS_INTERFACE                                    */
		  0x00,   /* bDescriptorSubtype: Header Func Desc                             */
		  0x10,   /* bcdCDC: spec release number                                      */
		  0x00,

		  /* Functional Descriptor[1] ----------------------------------------------- */
		  /* Call Management Functional Descriptor                                    */
		  0x05,   /* bFunctionLength                                                  */
		  0x24,   /* bDescriptorType: CS_INTERFACE                                    */
		  0x01,   /* bDescriptorSubtype: Call Management Func Desc                    */
		  0x00,   /* bmCapabilities: D0+D1 (no call management supported)             */
		  0x01,   /* bDataInterface: Interface[1]                                     */

		  /* Functional Descriptor[2] ----------------------------------------------- */
		  /* ACM Functional Descriptor                                                */
		  0x04,   /* bFunctionLength                                                  */
		  0x24,   /* bDescriptorType: CS_INTERFACE                                    */
		  0x02,   /* bDescriptorSubtype: Abstract Control Management desc             */
		  0x06,   /* bmCapabilities: *                                                */

		  /* Functional Descriptor[3] ----------------------------------------------- */
		  /* Union Functional Descriptor                                              */
		  0x05,   /* bFunctionLength                                                  */
		  0x24,   /* bDescriptorType: CS_INTERFACE                                    */
		  0x06,   /* bDescriptorSubtype: Union func desc                              */
		  0x00,   /* bMasterInterface: Communication class interface = Interface[0]   */
		  0x01,   /* bSlaveInterface0: Data Class Interface = Interface[1]            */

		// Endpoint Descriptor
		ENDPOINT_DESCRIPTOR_LENGTH,
		ENDPOINT_DESCRIOTPR_TYPE,
		0x81, // .bEndpointAddress(IN, EP1)
		0x03, // Interrupt Transfer
		64, // .wMacPacketSize
		0,
		0x10, // bInterval

		// Interface Descriptor for Data Class ================
		INTERFACE_DESCRIPTOR_LENGTH,
		INTERFACE_DESCRIPTOR_TYPE,
		1, // bInterfaceNumber
		0, // bAlternateSetting
		2, // bNumEndpoints
		0x0A, // .bInterfaceClass(CDC)
		0x00, // .bInterfaceSubClass(None)
		0x00, // bInterfaceProtocol(None)
		0, // iInterface

		// Endpoint Descriptor
		ENDPOINT_DESCRIPTOR_LENGTH,
		ENDPOINT_DESCRIOTPR_TYPE,
		0x02, // .bEndpointAddress(OUT, EP2)
		0x02, // Bulk Transfer
		64, // .wMacPacketSize
		0,
		0x00, // bInterval
		// Endpoint Descriptor
		ENDPOINT_DESCRIPTOR_LENGTH,
		ENDPOINT_DESCRIOTPR_TYPE,
		0x83, // .bEndpointAddress(IN, EP3)
		0x02, // Bulk Transfer
		64, // .wMacPacketSize
		0,
		0x00, // bInterval
};

const USBStringDescriptor string_desc[4] = {
		{ // Language Information
				.bLength = 4 + 2,
				.bDescriptorType = STRING_DESCRIPTOR_TYPE,
				.wLANGID = {0x411, 0x409} // ja-JP, en-US
		},
		{ // Serial Number
				.bLength = 4 * 2 + 2,
				.bDescriptorType = STRING_DESCRIPTOR_TYPE,
				.wLANGID = {'T', 'E', 'S', 'T'}
		},
		{ // Manufacturer
				.bLength = 10 * 2 + 2,
				.bDescriptorType = STRING_DESCRIPTOR_TYPE,
				.wLANGID = {'S', 'h', 'i', 't', ' ', 'C', 'o', 'r', 'p', '.'}
		},
		{ // Product
				.bLength = 15 * 2 + 2,
				.bDescriptorType = STRING_DESCRIPTOR_TYPE,
				.wLANGID = {'U', 'n', 'c', 'o', 'm', 'm', 'u', 'n', 'i', 'c', 'a', 't', 'i', 'o', 'n'}
		},
};

const uint8_t __attribute__((aligned(2))) ReportDescriptorForMouse[50] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};

const uint8_t __attribute__((aligned(2))) ReportDescriptorForKeyBoard[63] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};


#ifdef __cplusplus
}
#endif
