#pragma once
#include <stdint.h>
typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t bcdUSB;
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} USBDeviceDescriptor;
enum {
	DEVICE_DESCRIPTOR_LENGTH = 18,
	DEVICE_DESCRIPTOR_TYPE = 1
};

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterface;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;
} USBConfigurationDescriptor;
enum {
	CONFIG_DESCRIPTOR_LENGTH = 9,
	CONFIG_DESCRIPTOR_TYPE = 2
};

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} USBInterfaceDescriptor;
enum {
	INTERFACE_DESCRIPTOR_LENGTH = 9,
	INTERFACE_DESCRIPTOR_TYPE = 4
};

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
} USBEndpointDescriptor;
enum {
	ENDPOINT_DESCRIPTOR_LENGTH = 7,
	ENDPOINT_DESCRIOTPR_TYPE = 5,
};

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wLANGID[20];
} USBStringDescriptor;
enum {
	STRING_DESCRIPTOR_TYPE = 3
};

typedef enum {
	USB_DEVICECLASS_IN_INTERFACE_DESCRIPTOR = 0,
	USB_DEVICECLASS_COMMUNICATION = 2,
	USB_DEVICECLASS_HUB = 9,
	USB_DEVICECLASS_DIAGNOSTIC_DEVICE = 220,
	USB_DEVICECLASS_WIRELES_DEVICE = 224,
	USB_DEVICECLASS_MISC_DEVICE = 239,
	USB_DEVICECLASS_VENDOR_SPECIFIC_DEVICE = 255
} USBDeviceClass;

typedef enum {
	USB_BCD_USB1_0 = 0x100,
	USB_BCD_USB1_1 = 0x110,
	USB_BCD_USB2_0 = 0x200
} USBBcd;
