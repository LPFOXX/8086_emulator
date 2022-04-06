#pragma once

#include <array>

#include "primitives.hpp"
#include "ModRmByte.hpp"

namespace lp::emul8086 {
	class Memory {
	public:
		Memory() = default;
		virtual ~Memory() = default;

		Byte& operator[](uint32_t address);

		inline const Byte getByte(const Register CS, Register& IP) const
		{
			return mMemory[GET_MEM_ADDRESS(CS, IP++)];
		}

		inline const Byte getByte(const Address address) const
		{
			return mMemory[address];
		}

		const Word getWord(const Register CS, Register& IP) const;

		const ModRmByte getModRmByte(const Register CS, Register& IP) const
		{
			return mMemory[(CS << 4) | IP++];
		}

		void setByte(Address memAddress, Byte byte);

	private:
		std::array<Byte, 1024 * 1024> mMemory = { 0 };
	};
}