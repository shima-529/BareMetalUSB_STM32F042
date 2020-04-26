#include <usb/usb.hpp>
#include "stm32f0xx.h"
#include <algorithm>
#include <cstdio>
#include <usb/utils.hpp>

#include <usb_desc/usb_desc_struct.h>
#include <usb_desc/usb_desc.h>
#include <usb/usb_struct.h>
#define printf(...)

/* Pointer Notation
 * - type = uintptr_t : This is an pointer for PMA.
 *                      Local offset of PMA is stored.
 * - type = void * or others
 *                    : This is an pointer for main memory.
 */

// External Descriptors {{{
// }}}
alignas(2) static uint8_t line_coding_info[7];

// Some macros {{{
enum USBSetupPacketRequest {
	GET_STATUS = 0, CLEAR_FEATURE = 1, SET_FEATURE = 3,
	SET_ADDRESS = 5, GET_DESCRIPTOR = 6, SET_DESCRIPTOR = 7,
	GET_CONFIGURATION = 8, SET_CONFIGURATION = 9,
	GET_INTERFACE = 10, SET_INTERFACE = 11, SYNCH_FRAME = 12,

	CDC_SET_LINECODING = 0x20,
	CDC_GET_LINECODING = 0x21,
	CDC_SET_CONTROLLINESTATE = 0x22,
	CDC_SEND_BREAK = 0x23,
	CDC_SERIALSTATE = 0xA1
};
enum USBDescriptorType {
	DEVICE = 1, CONFIGURATION = 2, STRING = 3, INTERFACE = 4,
	ENDPOINT = 5, DEVICE_QUALIFIER = 6,
	OTHER_SPEED_CONFIGURATION = 7, INTERFACE_POWER = 8,
};
enum USBHIDDescriptorType {
	HID_REPORT = 0x22,
};
// }}}

PMAInfo pma_allocation_info;

USBEndpointInfo ep_info[4];
static USB_EP0Info ep0_data;
static USB_InitType init;

// Init global Funcs {{{
void usb_init(USB_InitType init) {
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;

	USB->CNTR &= ~USB_CNTR_PDWN;
	for(volatile int i=0; i<100; i++); // wait for voltage to be stable
	USB->CNTR &= ~USB_CNTR_FRES;
	USB->BCDR |= USB_BCDR_DPPU; // pull-up DP so as to notify the connection to the host

	USB->CNTR = USB_CNTR_RESETM; // enable reset interrupt
	NVIC_SetPriority(USB_IRQn, 0); // Highest Priority
	NVIC_EnableIRQ(USB_IRQn);

	::init = init;
}
// }}}

// Fundamental xfer/recv Funcs {{{
static void usb_ep_set_status(int endp, uint16_t status, uint16_t mask) {
	const auto reg = USB_EPnR(endp);
	USB_EPnR(endp) = (reg & (USB_EPREG_MASK | mask)) ^ status;
}
void usb_ep_send(int ep_num, uint16_t addr, uint16_t size) {
	ep_info[ep_num].pb_ptr->addr_tx = addr;
	ep_info[ep_num].pb_ptr->count_tx = size;
	usb_ep_set_status(ep_num, USB_EP_TX_VALID, USB_EPTX_STAT);
}
void usb_ep_receive(int ep_num, uint16_t addr, uint16_t size) {
	if( ep_info[ep_num].pb_ptr->addr_rx == 0 ) {
		ep_info[ep_num].pb_ptr->addr_rx = addr;
		if( size > 62 ) {
			ep_info[ep_num].pb_ptr->count_rx = ((size / 32) - 1) << 10;
			ep_info[ep_num].pb_ptr->count_rx |= 1 << 15;
		}else{
			ep_info[ep_num].pb_ptr->count_rx = size << 10;
		}
	}
	usb_ep_set_status(ep_num, USB_EP_RX_VALID, USB_EPRX_STAT);
}
// }}}

static void search_and_set_descriptor(uint16_t wValue) { // {{{
	const auto desc_type = (wValue >> 8) & 0xFF;
	auto& xfer_info = ep_info[0].xfer_info;
	auto& offsets = pma_allocation_info.offsets;

	switch( desc_type ) {
	case DEVICE:
	{
		const auto len = init.dep.dev_desc->bLength;
		if( offsets.device_desc == 0 ) {
			offsets.device_desc = Utils::alloc(len);
			Utils::pma_in(offsets.device_desc, init.dep.dev_desc, len);
		}
		xfer_info.ptr = offsets.device_desc;
		xfer_info.whole_length = len;
		break;
	}
	case CONFIGURATION:
	{
		const auto len = init.dep.conf_desc[2] | (init.dep.conf_desc[3] << 8);
		if( offsets.config_desc == 0 ) {
			offsets.config_desc = Utils::alloc(len);
			Utils::pma_in(offsets.config_desc, init.dep.conf_desc, len);
		}
		xfer_info.ptr = offsets.config_desc;
		xfer_info.whole_length = len;
		break;
	}
	case STRING:
	{
		const auto desc_no = wValue & 0xFF;
		const auto len = string_desc[desc_no].bLength;
		if( offsets.string_desc[desc_no] == 0 ) {
			offsets.string_desc[desc_no] = Utils::alloc(len);
			Utils::pma_in(offsets.string_desc[desc_no], &string_desc[desc_no], len);
		}
		xfer_info.ptr = offsets.string_desc[desc_no];
		xfer_info.whole_length = len;
		break;
	}
	case HID_REPORT:
	{
		const auto index = ep0_data.last_setup.wIndex;
		const auto len = init.dep.hid_report_desc_length[index];
		if( offsets.report_desc[index] == 0 ) {
			offsets.report_desc[index] = Utils::alloc(len);
			Utils::pma_in(offsets.report_desc[index], init.dep.hid_report_desc[index], len);
		}
		xfer_info.ptr = offsets.report_desc[index];
		xfer_info.whole_length = len;
		break;
	}
	default:
		break;
	}
	xfer_info.completed_length = 0;
} // }}}

static uint16_t linecoding_adddr;
// EP0 Handler/Preparer {{{
static void usb_ep0_handle_setup() {
	static uint16_t device_state = 0;
	static uint16_t state_addr = 0;
	static uint8_t controllin_state = 0;
	const auto& ep = ep_info[0];
	auto& xfer_info = const_cast<decltype(ep_info[0])&>(ep).xfer_info;
	const auto& last_setup = ep0_data.last_setup;
	if( last_setup.bRequest == GET_DESCRIPTOR ) {
		search_and_set_descriptor(last_setup.wValue);
		usb_ep_send(0, xfer_info.ptr, std::min({last_setup.wLength, ep.packet_size, xfer_info.whole_length}));
	}
	if( last_setup.bRequest == GET_CONFIGURATION ) {
		if( state_addr == 0 ) {
			state_addr = Utils::alloc(1);
		}
		Utils::pma_in(state_addr, &device_state, 1);
		xfer_info.ptr = state_addr;
		xfer_info.whole_length = 1;
		usb_ep_send(0, xfer_info.ptr, xfer_info.whole_length);
	}
	if( last_setup.bRequest == SET_CONFIGURATION ) {
		device_state = last_setup.wValue;
	}
	if( last_setup.bRequest == CDC_SET_CONTROLLINESTATE ) {
		controllin_state = last_setup.wValue;
		(void)controllin_state;
	}
	if( last_setup.bRequest == CDC_SET_LINECODING ) {
		if( linecoding_adddr == 0 ) {
			linecoding_adddr = Utils::alloc(7);
		}
		usb_ep_receive(0, linecoding_adddr, 7);
	}
}
static void usb_ep0_handle_status() {
	if( ep0_data.last_setup.bRequest == SET_ADDRESS ) {
		USB->DADDR = USB_DADDR_EF | (ep0_data.last_setup.wValue & 0x7F);
	}
	if( ep0_data.last_setup.bRequest == CDC_SET_LINECODING ) {
		Utils::pma_out(line_coding_info, linecoding_adddr, 7);
		printf("BaudRate: %d\n", line_coding_info[0] + (line_coding_info[1] << 8) + (line_coding_info[2] << 16) + (line_coding_info[3] << 24));
		printf("bCharFormat: %d\n", line_coding_info[4]);
		printf("bParityType: %d\n", line_coding_info[5]);
		printf("bDataBits: %d\n", line_coding_info[6]);
	}
}

static void usb_ep0_prepare_for_setup() {
	if( pma_allocation_info.offsets.setup_packet == 0 ) {
		pma_allocation_info.offsets.setup_packet = Utils::alloc(64);
	}
	usb_ep_receive(0, pma_allocation_info.offsets.setup_packet, sizeof(USBSetupPacket));
}
static void usb_ep0_prepare_for_next_in() {
	// Now data is left to be xferred.
	// This means the last IN transaction is completed but the data length is not enough.
	auto& xfer_info = ep_info[0].xfer_info;
	xfer_info.completed_length += ep_info[0].pb_ptr->count_tx;

	const auto next_pos = xfer_info.completed_length + xfer_info.ptr;
	usb_ep_send(0, next_pos, xfer_info.whole_length - ep_info[0].packet_size);
}
static void usb_ep0_prepare_for_status() {
	if( ep0_data.fsm == EP_fsm::STATUS_OUT ) {
		usb_ep_receive(0, ep_info[0].recv_info.ptr, 0);
	}
	if( ep0_data.fsm == EP_fsm::STATUS_IN ) {
		usb_ep_send(0, ep_info[0].xfer_info.ptr, 0);
	}
}
static void usb_ep0_handle_current_transaction() {
	const auto now_state = ep0_data.fsm;
	switch(now_state) {
		case EP_fsm::SETUP:
			usb_ep0_handle_setup();
			break;
		case EP_fsm::STATUS_IN:
		case EP_fsm::STATUS_OUT:
			usb_ep0_handle_status();
			break;
		default:
			break;
	}
}
static void usb_ep0_prepare_for_next_transaction() {
	const auto next_state = ep0_data.fsm;
	switch(next_state) {
		case EP_fsm::REPETITIVE_IN:
			usb_ep0_prepare_for_next_in();
			break;
		case EP_fsm::STATUS_IN:
		case EP_fsm::STATUS_OUT:
			usb_ep0_prepare_for_status();
			break;
		case EP_fsm::SETUP:
			usb_ep0_prepare_for_setup();
			break;
		default:
			break;
	}
}
// }}}

static EP_fsm next_state_ep0() { // {{{
	const auto& ep = ep_info[0];
	const auto& last_setup = ep0_data.last_setup;
	switch(ep0_data.fsm) {
		case EP_fsm::SETUP:
			if( last_setup.bmRequestType & 0x80 ) {
				return (last_setup.wLength != 0) ? EP_fsm::IN : EP_fsm::STATUS_IN;
			}else{
				return (last_setup.wLength != 0) ? EP_fsm::OUT : EP_fsm::STATUS_IN;
			}
			break;
		case EP_fsm::IN:
		case EP_fsm::REPETITIVE_IN:
			{
				const auto& xfer_info = ep.xfer_info;

				const bool cond = (ep_info[0].pb_ptr->count_tx == ep_info[0].packet_size);
				const auto length_to_be_xferred = cond ? xfer_info.whole_length - xfer_info.completed_length : 0;
				if( length_to_be_xferred > ep.packet_size ) {
					return EP_fsm::REPETITIVE_IN;
				}
				return EP_fsm::STATUS_OUT;
				break;
			}
		case EP_fsm::OUT:
			return EP_fsm::STATUS_IN;
			break;
		default:
			break;
	}
	return EP_fsm::SETUP;
} // }}}

extern "C" void USB_IRQHandler() {
	auto flag = USB->ISTR;

	if( flag & USB_ISTR_RESET ) {
		printf("\nRESET\n");
		USB->ISTR &= ~USB_ISTR_RESET;
		USB->CNTR |= USB_CNTR_CTRM | USB_CNTR_RESETM;
		pma_allocation_info = {};
		USB->BTABLE = Utils::alloc(sizeof(PacketBuffer) * (init.nEndpoints + 1));
		USB->DADDR = USB_DADDR_EF; // enable the functionality

		// initialize EP0
		ep0_data.fsm = EP_fsm::RESET;
		ep_info[0].packet_size = 64;
		ep_info[0].pb_ptr = reinterpret_cast<PacketBuffer *>(static_cast<uintptr_t>(USB->BTABLE) + USB_PMAADDR);
		ep_info[0].pb_ptr->addr_rx = 0;
		USB->EP0R = USB_EP_CONTROL;
		usb_ep0_prepare_for_setup();

		// Other EPs
		for(int i = 0; i < init.nEndpoints; i++) {
			const int ep_num = i + 1;
			init.ep[i].init(ep_num);
		}
	}

	while( (flag = USB->ISTR) & USB_ISTR_CTR ) {
		const auto ep_num = flag & USB_ISTR_EP_ID;
		const auto epreg = USB_EPnR(ep_num);

		if( ep_num == 0 ) {
			if( epreg & USB_EP_CTR_RX ) {
				// DIR == 1 : IRQ by SETUP transaction. CTR_RX is set.
				USB->EP0R = epreg & ~USB_EP_CTR_RX & USB_EPREG_MASK;
				if( epreg & USB_EP_SETUP ) {
					ep0_data.fsm = EP_fsm::SETUP;
					Utils::pma_out(&ep0_data.last_setup, pma_allocation_info.offsets.setup_packet, sizeof(USBSetupPacket));
					Utils::Dump::setup_packet();
				}
				Utils::Dump::fsm();
				usb_ep0_handle_current_transaction();
			}
			if( epreg & USB_EP_CTR_TX ) {
				// DIR = 0 : IRQ by IN direction. CTR_TX is set.
				USB->EP0R = epreg & ~USB_EP_CTR_TX & USB_EPREG_MASK;
				Utils::Dump::fsm();
				usb_ep0_handle_current_transaction();
			}
			ep0_data.fsm = next_state_ep0();
			usb_ep0_prepare_for_next_transaction();
		}else{
			if( epreg & USB_EP_CTR_TX ) {
				USB_EPnR(ep_num) = epreg & ~USB_EP_CTR_TX & USB_EPREG_MASK;
				init.ep[ep_num - 1].tx_handler(ep_num);
			}
			if( epreg & USB_EP_CTR_RX ) {
				USB_EPnR(ep_num) = epreg & ~USB_EP_CTR_RX & USB_EPREG_MASK;
				init.ep[ep_num - 1].rx_handler(ep_num);
			}
		}
	}
}
