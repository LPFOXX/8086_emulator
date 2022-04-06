#pragma once

namespace lp::emul8086 {
	enum class FlagMasks : Word {
		CarryFlagMask = 1 << 0,
		ParityFlagMask = 1 << 2,
		AuxiliaryFlagMask = 1 << 4,
		ZeroFlagMask = 1 << 6,
		SignFlagMask = 1 << 7,
		TrapFlagMask = 1 << 8,
		InterruptFlagMask = 1 << 9,
		DirectionFlagMask = 1 << 10,
		OverflowFlagMask = 1 << 11,
	};
}
