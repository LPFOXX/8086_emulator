#pragma once

#include <tuple>

#include "primitives.hpp"

namespace lp::emul8086 {
	enum class Mod : Byte {
		NoDisplacement = 0b00,
		ByteDisplacement = 0b01,
		WordDisplacement = 0b10,
		RegisterValue = 0b11,
	};

	enum class Rm : Byte {
		BXPlusSI = 0b000,
		BXPlusDI = 0b001,
		BPPlusSI = 0b010,
		BPPlusDI = 0b011,
		RM_SI = 0b100,
		RM_DI = 0b101,
		RM_BP = 0b110,
		RM_BX = 0b111,
	};

	union ModRmByte {
		ModRmByte(Byte b) : Mod(b >> 6), Reg((b >> 3) & 0b00000111), Rm(b & 0b00000111) {}

		ModRmByte(Byte mod, Byte reg, Byte rm) : Mod(mod), Reg(reg), Rm(rm) {}

		explicit operator Byte() const {
			Byte b = Mod;
			b <<= 3;
			b |= Reg;
			b <<= 3;
			b |= Rm;
			return b;
		}

		struct {
			Byte Mod : 2;    // register mode: 2bits
			Byte Reg : 3;    // register		: 3 bits
			Byte Rm : 3;     // register/memory operand : 3 bits
		};
	};
}
