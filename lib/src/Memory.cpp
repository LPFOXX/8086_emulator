#include "Memory.hpp"

namespace lp::emul8086
{
	Byte& Memory::operator[](Address address) {
		return mMemory[address];
	}

	/*inline const Byte Memory::getByte(const Register CS, Register& IP) const
	{
		return mMemory[GET_MEM_ADDRESS(CS, IP++)];
	}*/

	/*inline const Byte Memory::getByte(const Address address) const
	{
		return mMemory[address];
	}*/

	const Word Memory::getWord(const Register CS, Register& IP) const
	{
		Byte lo = mMemory[GET_MEM_ADDRESS(CS, IP++)];
		Byte hi = mMemory[GET_MEM_ADDRESS(CS, IP++)];

		return (hi << 8) | lo;
	}

	/*inline const ModRmByte Memory::getModRmByte(const Register CS, Register& IP) const
	{
		return mMemory[(CS << 4) | IP++];
	}*/

	inline void Memory::setByte(Address memAddress, Byte byte)
	{
		mMemory[memAddress] = byte;
	}
}
