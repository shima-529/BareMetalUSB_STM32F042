#include <usb/usb.hpp>
#include "stm32f0xx.h"
#include <cstdint>
#include <cstdio>
#include "usb_desc/usb_desc_struct.h"
#include "usb/usb_struct.h"
#define printf(...)
extern PMAInfo pma_allocation_info;

namespace Utils {
	uint16_t alloc(int size) {
		const auto ret = pma_allocation_info.sp;
		pma_allocation_info.sp += size;
		if( size & 1 ) pma_allocation_info.sp++;
		return ret;
	}
	void pma_out(void* dest, const uintptr_t src, int size) {
		uintptr_t src_global_addr = src + USB_PMAADDR;
		auto* src_p = (uint16_t *)src_global_addr;
		auto* dest_p = (uint8_t *)dest;
		if( size & 1 ) size++;
		for(int i = 0; i < size / 2; i++) {
			dest_p[2 * i] = src_p[i] & 0xFF;
			dest_p[2 * i + 1] = src_p[i] >> 8;
		}
	}
	void pma_in(uintptr_t dest, const void* src, int size) {
		uintptr_t dest_global_addr = dest + USB_PMAADDR;
		auto* src_p = (uint8_t *)src;
		auto* dest_p = (uint16_t *)dest_global_addr;
		if( size & 1 ) size++;
		for(int i = 0; i < size / 2; i++) {
			dest_p[i] = src_p[2 * i] | (src_p[2 * i + 1] << 8);
		}
	}
	namespace Dump {
		void setup_packet() {
			printf("bmResuestType: 0x%02x\n", ep_info[0].last_setup.bmRequestType);
			printf("bRequest: 0x%02x\n", ep_info[0].last_setup.bRequest);
			printf("wValue: 0x%x\n", ep_info[0].last_setup.wValue);
			printf("wIndex: 0x%x\n", ep_info[0].last_setup.wIndex);
			printf("wLength: 0x%x\n", ep_info[0].last_setup.wLength);
		}
		void fsm() {
			const char* table[static_cast<int>(EP_fsm::LENGTH)] = {
				"ST_RESET",
				"SETUP",
				"IN",
				"REPETITIVE_IN",
				"OUT",
				"STATUS_IN",
				"STATUS_OUT",
			};
			(void)table;
			printf("** %s\n", table[static_cast<int>(ep_info[0].fsm)]);
		}
	}
}
