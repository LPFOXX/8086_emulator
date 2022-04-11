#pragma once

#include "primitives.hpp"

namespace lp::emul8086 {
	enum class Registers : Byte {
		// 8bits
		// Lo
		AL = 0b000,
		BL = 0b011,
		CL = 0b001,
		DL = 0b010,
		// Hi
		AH = 0b100,
		BH = 0b111,
		CH = 0b101,
		DH = 0b110,

		// 16bits
		AX = 0b000,
		BX = 0b011,
		CX = 0b001,
		DX = 0b010,
		SP = 0b100,
		BP = 0b101,
		SI = 0b110,
		DI = 0b111
	};
}
