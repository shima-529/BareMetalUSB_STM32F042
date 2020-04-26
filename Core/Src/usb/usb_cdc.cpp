#include <usb/usb.hpp>
#include "stm32f0xx.h"
#include <algorithm>
#include "usb/utils.hpp"

extern "C" {
#include "usb_desc/usb_desc.h"
alignas(2) char cdc_buf[64];
}

namespace {
namespace Init {
void ep1(int) {
	// initialize EP1
	ep_info[1].packet_size = 64;
	ep_info[1].pb_ptr = reinterpret_cast<PacketBuffer *>(static_cast<uintptr_t>(USB->BTABLE) + USB_PMAADDR + sizeof(PacketBuffer) * 1);
	ep_info[1].pb_ptr->addr_tx = Utils::alloc(64);
	ep_info[1].pb_ptr->count_tx = 64;
	USB_EPnR(1) = USB_EP_INTERRUPT | 0x01;
}
void ep2(int) {
	// initialize EP2
	ep_info[2].packet_size = 64;
	ep_info[2].pb_ptr = reinterpret_cast<PacketBuffer *>(static_cast<uintptr_t>(USB->BTABLE) + USB_PMAADDR + sizeof(PacketBuffer) * 2);
	ep_info[2].pb_ptr->addr_rx = Utils::alloc(64);
	ep_info[2].pb_ptr->count_rx = 64;
	USB_EPnR(2) = USB_EP_BULK | 0x02;
}
void ep3(int) {
	// initialize EP3
	ep_info[3].packet_size = 64;
	ep_info[3].pb_ptr = reinterpret_cast<PacketBuffer *>(static_cast<uintptr_t>(USB->BTABLE) + USB_PMAADDR + sizeof(PacketBuffer) * 3);
	pma_allocation_info.offsets.cdc_data = Utils::alloc(64);
	USB_EPnR(3) = USB_EP_BULK | 0x03;
}
}
namespace Handler {
namespace Tx {
	void null_handler(int){}
}
namespace Rx {
	void null_handler(int){}
}
}
}


static const USB_InitType ep_handle {
	{&dev_desc_cdc, conf_desc_cdc,
		{nullptr, nullptr}, {0, 0}, {0, 0},
	},
	3, // endpoint number except EP0
	{
			{64, Init::ep1, Handler::Tx::null_handler, Handler::Rx::null_handler},
			{64, Init::ep2, Handler::Tx::null_handler, Handler::Rx::null_handler},
			{64, Init::ep3, Handler::Tx::null_handler, Handler::Rx::null_handler},
	}
};

#ifdef __cplusplus
extern "C" {
#endif
void usb_cdc_init(void) {
	usb_init(ep_handle);
}
void usb_cdc_send(const char *str, int size) {
	std::copy(str, str + size, cdc_buf);
	const auto addr = pma_allocation_info.offsets.cdc_data;
	Utils::pma_in(addr, cdc_buf, size);
	usb_ep_send(3, addr, size);
}


#ifdef __cplusplus
}
#endif
