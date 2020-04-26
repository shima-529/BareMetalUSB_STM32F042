#pragma once

typedef struct {
	uint8_t bFunctionalLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint16_t bcdCDC;
} USBCDCHeaderFunctionalDescriptor;

typedef struct {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bControlInterface;
	uint8_t bSubordinateInterface[5];
} USBCDCUnionFunctionalDescriptor;

typedef struct {
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t iCountryCodeRelDate;
	uint16_t wCountryCode[5];
} USBCDCCountrySelectionFunctionalDescriptor;

typedef struct {
	uint8_t bFunctionalLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
} USBCDCACMCallManagementFunctionalDescriptor;

typedef struct {
	uint8_t bFunctionalLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubType;
	uint8_t bmCapabilities;
} USBCDCACMFunctionalDescriptor;

enum {
	CS_INTERFACE = 0x24,
	CS_ENDPOINT = 0x25
};
