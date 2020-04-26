#pragma once
#include <stdint.h>

typedef struct {
	const USBDeviceDescriptor* dev_desc;
	const uint8_t* conf_desc;
	const uint8_t* hid_report_desc[2];
	uint8_t hid_report_desc_length[2];
	uint8_t report_length[2];
} USBDeviceDeps;

// Struct for USB Peripheral & EP {{{
struct PacketBuffer {
	uint16_t addr_tx;
	uint16_t count_tx;
	uint16_t addr_rx;
	uint16_t count_rx;
};
enum struct EP_fsm {
	RESET, SETUP, IN, REPETITIVE_IN, OUT, STATUS_IN, STATUS_OUT,
	LENGTH
};
struct DataInfo {
	uint16_t ptr;
	uint16_t completed_length;
	uint16_t whole_length;
};
struct USBSetupPacket {
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
};
struct USBEndpointInfo {
	uint16_t packet_size;
	PacketBuffer* pb_ptr;
	DataInfo xfer_info;
	DataInfo recv_info;
};
struct USB_EP0Info {
	EP_fsm fsm;
	USBSetupPacket last_setup;
};
// }}}

// Vars for PMA buffer {{{
struct PMAInfo {
	struct PMAOffsets {
		uint16_t setup_packet;
		uint16_t device_desc;
		uint16_t config_desc;
		uint16_t string_desc[4];
		uint16_t report_desc[2];
		uint16_t report[2];
		uint16_t cdc_data;
	} offsets;
//	PMAOffsets offsets;
	uint16_t sp;
};
// }}}

typedef struct {
	int packet_size;
	void (*init)(int);
	void (*tx_handler)(int);
	void (*rx_handler)(int);
} USBEP_Handles;
typedef struct {
	USBDeviceDeps dep;
	int nEndpoints;
	USBEP_Handles ep[4];
} USB_InitType;
