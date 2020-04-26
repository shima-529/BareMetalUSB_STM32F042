#include <usb/usb.hpp>
#include "stm32f0xx.h"
#include <algorithm>
#include "usb/utils.hpp"

extern "C" {
#include "usb_desc/usb_desc.h"
alignas(2) int8_t report[2][8] = {};
}

static const USBDeviceDeps deps[] = {
		{ // Mouse
				&dev_desc_hid,
				conf_desc_mouse,
				{ReportDescriptorForMouse},
				{sizeof(ReportDescriptorForMouse)},
				{3},
		},
		{ // Kbd
				&dev_desc_hid,
				conf_desc_keyb,
				{ReportDescriptorForKeyBoard},
				{sizeof(ReportDescriptorForKeyBoard)},
				{8},
		},
		{ // Composite
				&dev_desc_hid,
				conf_desc_keyb_mouse,
				{ReportDescriptorForMouse, ReportDescriptorForKeyBoard},
				{sizeof(ReportDescriptorForMouse), sizeof(ReportDescriptorForKeyBoard)},
				{3, 8},
		},
};

enum DevType {
	MOUSE, KBD, COMPOSITE
} dev_type;

namespace {
namespace Init {
void ep(int ep_num) {
	// initialize EP1
	ep_info[ep_num].packet_size = 64;
	ep_info[ep_num].pb_ptr = reinterpret_cast<PacketBuffer *>(static_cast<uintptr_t>(USB->BTABLE) + USB_PMAADDR + sizeof(PacketBuffer) * ep_num);
	USB_EPnR(ep_num) = USB_EP_INTERRUPT | ep_num;
	pma_allocation_info.offsets.report[ep_num - 1] = Utils::alloc(deps[dev_type].report_length[ep_num - 1]); // size of report, in this case.
	Utils::pma_in(pma_allocation_info.offsets.report[ep_num - 1], const_cast<int8_t *>(report[ep_num - 1]), deps[dev_type].report_length[ep_num - 1]);
	usb_ep_send(ep_num, pma_allocation_info.offsets.report[ep_num - 1], deps[dev_type].report_length[ep_num - 1]);
}
}
namespace Handler {
namespace Tx {
	void ep_handler(int ep_num){
		const auto report_length = deps[dev_type].report_length[ep_num - 1];
		Utils::pma_in(pma_allocation_info.offsets.report[ep_num - 1], const_cast<int8_t *>(report[ep_num - 1]), report_length);
		usb_ep_send(ep_num, pma_allocation_info.offsets.report[ep_num - 1], report_length);
	}
}
namespace Rx {
	void null_handler(int){}
}
}
}

extern "C" {
void usb_hid_mouse_init() {
	const USB_InitType ep_handle {
		deps[0],
		1, // endpoint number except EP0
		{
				{64, Init::ep, Handler::Tx::ep_handler, Handler::Rx::null_handler},
		}
	};
	dev_type = MOUSE;
	usb_init(ep_handle);
}
void usb_hid_keyboard_init() {
	const USB_InitType ep_handle {
		deps[1],
		1, // endpoint number except EP0
		{
				{64, Init::ep, Handler::Tx::ep_handler, Handler::Rx::null_handler},
		}
	};
	dev_type = KBD;
	usb_init(ep_handle);
}
void usb_hid_composite_init() {
	const USB_InitType ep_handle {
		deps[2],
		2, // endpoint number except EP0
		{
				{64, Init::ep, Handler::Tx::ep_handler, Handler::Rx::null_handler},
				{64, Init::ep, Handler::Tx::ep_handler, Handler::Rx::null_handler},
		}
	};
	dev_type = COMPOSITE;
	usb_init(ep_handle);
}
}

