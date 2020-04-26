/*
 * utils.hpp
 *
 *  Created on: 2020/04/26
 *      Author: yuki
 */

#ifndef INC_USB_UTILS_HPP_
#define INC_USB_UTILS_HPP_

namespace Utils {
	uint16_t alloc(int size);
	void pma_out(void* dest, const uintptr_t src, int size);
	void pma_in(uintptr_t dest, const void* src, int size);
	namespace Dump {
		void setup_packet();
		void fsm();
	}
}

#define SUPPRESS_WARN(x) (void)(x)
#define USB_EPnR(n) *reinterpret_cast<volatile uint16_t *>(reinterpret_cast<uintptr_t>(&USB->EP0R) + 4 * (n))



#endif /* INC_USB_UTILS_HPP_ */
